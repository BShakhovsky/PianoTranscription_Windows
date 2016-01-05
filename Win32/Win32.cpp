#include "stdafx.h"
#include "Win32.h"
#include "MidiParser_Facade.h"
#include "MidiError.h"
#include "Keyboard.h"
#include "Sound.h"

using namespace std;
using boost::lexical_cast;

const auto ASPECT_RATIO(6);

shared_ptr<MidiParser_Facade> gMidi(nullptr);
shared_ptr<Sound> gSound(nullptr);
Keyboard gKeyboard;
wstring gErrBuf;
UINT gTimerTick(USER_TIMER_MINIMUM);
vector<time_t> gStarts;
vector<unsigned> gPrevTimes;
vector<size_t> gIndexes;

vector<size_t> gTracks;

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
		break;
	}
	return FALSE;
}

bool PlayTrack(size_t trackNo, DWORD dwTime)
{
	if (static_cast<time_t>(dwTime + gPrevTimes.at(trackNo)) - gStarts.at(trackNo) -
		static_cast<time_t>(gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo))) > 0)
	{
		gStarts.at(trackNo) = dwTime;
		gPrevTimes.at(trackNo) = gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo));
		gKeyboard.ReleaseAllKeys();
		do
		{
			gKeyboard.PressKey(gMidi->GetNotes().at(trackNo).at(gIndexes.at(trackNo)));
			gSound->AddNote(gMidi->GetNotes().at(trackNo).at(gIndexes.at(trackNo)));
			++gIndexes.at(trackNo);
		} while (gIndexes.at(trackNo) < gMidi->GetNotes().at(trackNo).size() &&
			gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo))
			- gMidi->GetMilliSeconds().at(trackNo).at(gIndexes.at(trackNo) - 1) <= gTimerTick);

		if (gIndexes.at(trackNo) >= gMidi->GetNotes().at(trackNo).size())
			gStarts.at(trackNo) = USER_TIMER_MAXIMUM;
		return true;
	}
	return false;
}
void CALLBACK OnTimer(HWND hWnd, UINT, UINT_PTR id, DWORD dwTime)
{
	const auto update(accumulate(gTracks.cbegin(), gTracks.cend(), false,
		[dwTime](bool x, size_t y) { return x || PlayTrack(y, dwTime); }));
	if (any_of(gStarts.cbegin(), gStarts.cend(),
		[](time_t val) {return val == USER_TIMER_MAXIMUM; })) KillTimer(hWnd, id);
	if (update) InvalidateRect(hWnd, nullptr, false);
}

BOOL OnCreate(HWND hWnd, LPCREATESTRUCT)
{
	gSound = make_shared<Sound>(hWnd);
	return true;
}
inline BOOL OnWindowPosChanging(HWND, LPWINDOWPOS pos)
{
	pos->cy = pos->cx / ASPECT_RATIO;
	return false;
}
inline void OnSize(HWND hWnd, UINT, int cx, int cy)
{
	gKeyboard.UpdateSize(hWnd, cx, cy);
	gKeyboard.ReleaseAllKeys();
}
void OnCommand(HWND hWnd, int id, HWND, UINT)
{
	switch (id)
	{
	case IDM_OPEN:
	{
		OPENFILENAME fileName{ sizeof fileName, hWnd };
		fileName.lpstrFilter = TEXT("MIDI files (*.mid, *.midi)\0*.mid*\0All files\0*.*\0");
		TCHAR buf[MAX_PATH] = TEXT("");
		fileName.lpstrFile = buf;
		fileName.nMaxFile = sizeof buf / sizeof *buf;
		fileName.Flags = OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&fileName))
		{
			char errBuf[0xFF] = "";
			setvbuf(stderr, errBuf, _IOFBF, sizeof errBuf / sizeof *errBuf);
			try
			{
				gMidi = make_shared<MidiParser_Facade>(fileName.lpstrFile);
				gIndexes.assign(gMidi->GetNotes().size(), 0);
				gStarts.assign(gMidi->GetMilliSeconds().size(), -USER_TIMER_MAXIMUM / 2);
				gPrevTimes.assign(gMidi->GetMilliSeconds().size(), 0);
				gTracks = { 2, 3 };
				SetTimer(hWnd, 0, gTimerTick, OnTimer);
			}
			catch (const MidiError& e)
			{
				MessageBox(hWnd, lexical_cast<std::wstring>(e.what()).c_str(),
					TEXT("Error"), MB_ICONHAND);
			}
			setvbuf(stderr, nullptr, _IOFBF, 2);
			gErrBuf = lexical_cast<wstring>(errBuf);
			InvalidateRect(hWnd, nullptr, true);
		}
		else MessageBox(hWnd, fileName.lpstrFile,
			TEXT("Cannot open the following file:"), MB_ICONEXCLAMATION);
	}
	break;
	case IDM_ABOUT:
		DialogBox(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
		break;
	}
}
void OnPaint(HWND hWnd)
{
	gSound->Play();

	PAINTSTRUCT ps;
	const auto hdc(BeginPaint(hWnd, &ps));
	gKeyboard.Draw(hdc);
	RECT rect{ 0 };
	GetClientRect(hWnd, &rect);
	if (gMidi)
	{
		const wstring text(gMidi->GetLog().cbegin(), gMidi->GetLog().cend());
		DrawText(hdc, text.c_str(), static_cast<int>(text.length()), &rect, DT_EXPANDTABS);
	}
	if (!gErrBuf.empty())
	{
		rect.left = rect.right / 2;
		const auto oldColor(SetTextColor(hdc, RGB(0xFF, 0, 0)));
		DrawText(hdc, gErrBuf.c_str(), static_cast<int>(gErrBuf.length()), &rect, 0);
		SetTextColor(hdc, oldColor);
	}
	EndPaint(hWnd, &ps);
}
inline void OnDestroy(HWND)
{
	PostQuitMessage(0);
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE,				OnCreate);
		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING,	OnWindowPosChanging);
		HANDLE_MSG(hWnd, WM_SIZE,				OnSize);
		HANDLE_MSG(hWnd, WM_COMMAND,			OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT,				OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY,			OnDestroy);
	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}

int APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE, _In_ LPWSTR, _In_ int nCmdShow)
{
	const auto szWindowClass(TEXT("MainWindow"));
	WNDCLASSEXW wcex{ sizeof wcex };
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_WIN32));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);

	const auto hWnd(CreateWindowW(szWindowClass, TEXT("Piano Fingers"), WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, HWND_DESKTOP, nullptr, hInstance, nullptr));
	if (!hWnd) return FALSE;
	ShowWindow(hWnd, SW_SHOWMAXIMIZED);
#ifdef _DEBUG
	UNREFERENCED_PARAMETER(nCmdShow);
#else
	ShowWindow(hWnd, nCmdShow);
#endif
	UpdateWindow(hWnd);

    const auto hAccelTable(LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_WIN32)));
	MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0))
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    return static_cast<int>(msg.wParam);
}