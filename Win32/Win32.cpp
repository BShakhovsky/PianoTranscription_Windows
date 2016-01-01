#include "stdafx.h"
#include "Win32.h"
#include "MidiParser_Facade.h"
#include "MidiError.h"
#include "Keyboard.h"

using namespace std;

shared_ptr<MidiParser_Facade> gMidi(nullptr);
Keyboard gKeyboard;

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
			try
			{
				gMidi = make_shared<MidiParser_Facade>(fileName.lpstrFile);
				MessageBox(hWnd,
					boost::lexical_cast<std::wstring>(gMidi->GetNotes().at(1).at(1)).c_str(),
					TEXT("Test"), 0);
			}
			catch (const MidiError& e)
			{
				MessageBox(hWnd, boost::lexical_cast<std::wstring>(e.what()).c_str(),
					TEXT("Error"), MB_ICONHAND);
			}
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
	gKeyboard.ReleaseAllKeys(hdc);
	EndPaint(hWnd, &ps);
}
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_COMMAND,	OnCommand);
		HANDLE_MSG(hWnd, WM_PAINT,		OnPaint);
		HANDLE_MSG(hWnd, WM_DESTROY,
			[](HWND) { PostQuitMessage(0); });
		HANDLE_MSG(hWnd, WM_SIZE,
			[](HWND, UINT, int cx, int cy) { gKeyboard.SetWidthHeight(cx, cy); });
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