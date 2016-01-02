#include "stdafx.h"
#include "Win32.h"
#include "MidiParser_Facade.h"
#include "MidiError.h"
#include "Keyboard.h"

using namespace std;
using boost::lexical_cast;

shared_ptr<MidiParser_Facade> gMidi(nullptr);
Keyboard gKeyboard;
wstring gErrBuf;

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

/*
void CALLBACK OnTimer(HWND hWnd, UINT, UINT_PTR id, DWORD)
{
	static auto start(time(nullptr));
	static auto index(1);
	const auto wait(start * 1'000 + gMidi->GetMilliSeconds().at(1).at(index)
		- gMidi->GetMilliSeconds().at(1).at(index - 1) - time(nullptr));
	if (wait > 0)
	{
//		gKeyboard.ReleaseAllKeys(hdc);
//		gKeyboard.PressKey(gMidi->GetNotes().at(1).at(index));
		start = time(nullptr);
		++index;
		if (index >= gMidi->GetNotes().at(1).size()) KillTimer(hWnd, id);
	}
}
*/

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
//				SetTimer(hWnd, 0, 10, OnTimer);
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
void OnDestroy(HWND)
{
	PostQuitMessage(0);
}
void OnSize(HWND hWnd, UINT, int cx, int cy)
{
	gKeyboard.UpdateSize(hWnd, cx, cy);
}
void OnLButtonDown(HWND hWnd, BOOL fDoubleClick, int, int, UINT keyFlags)
{
	UNREFERENCED_PARAMETER(hWnd);
	UNREFERENCED_PARAMETER(fDoubleClick);
	UNREFERENCED_PARAMETER(keyFlags);
	static size_t index(0);
	gKeyboard.ReleaseAllKeys();
	gKeyboard.PressKey(gMidi->GetNotes().at(1).at(index));
	++index;
	InvalidateRect(hWnd, nullptr, false);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_COMMAND,	OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT,		OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY,	OnDestroy);
		HANDLE_MSG(hWnd, WM_SIZE,		OnSize);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN, OnLButtonDown);
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
	ShowWindow(hWnd, nCmdShow);
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