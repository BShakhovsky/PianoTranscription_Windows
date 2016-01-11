#pragma once

#include "resource.h"

class Piano abstract
{
public:
	static std::unique_ptr<class MidiParser_Facade> midi;
	static std::unique_ptr<class Keyboard> keyboard;
	static std::unique_ptr<class Sound_Facade> sound;

	static std::vector<size_t> indexes, tracks;

	static int Main(HINSTANCE, int);
private:
	static constexpr LPCTSTR szWindowClass_ = TEXT("MainWindow");
	static ATOM MyRegisterClass(HINSTANCE);
	static BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
	{
		const auto hWnd(CreateWindowEx(WS_EX_ACCEPTFILES, szWindowClass_, TEXT("Piano Fingers"),
			WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, CW_USEDEFAULT, 0,
			HWND_DESKTOP, nullptr, hInstance, nullptr));
		if (!hWnd) return FALSE;

		ShowWindow(hWnd, SW_SHOWMAXIMIZED);
#ifdef _DEBUG
		UNREFERENCED_PARAMETER(nCmdShow);
#else
		ShowWindow(hWnd, nCmdShow);
#endif
		UpdateWindow(hWnd);

		return TRUE;
	}
};