#pragma once

class MainWindow abstract
{
public:
	static HINSTANCE hInstance;
	static HWND hWndMain;
	static TCHAR path[MAX_PATH];

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
private:
	static BOOL OnCreate(HWND, LPCREATESTRUCT);
	static void OnDestroy(HWND);

	static void CorrectAspectRatio();
	static BOOL OnWindowPosChanging(HWND, LPWINDOWPOS);
	static void OnMove(HWND, int, int);
	static void OnSize(HWND, UINT, int, int);

	static void OnMidiError(LPCTSTR);
	static void OnMidiSuccess();
	static bool OpenMidiFile(LPCTSTR);
	static void OpenMediaFile(LPCTSTR);
	static void OnDropFiles(HWND, HDROP);
	static void OnCommand(HWND, int, HWND, UINT);

	static void OnPaint(HWND);

	static void OnMouseMove(HWND, int, int, UINT);
	static void OnContextMenu(HWND, HWND, int, int);
private:
	static HMENU hContextMenu_, hContextSubMenu_;
	static int dlgWidth_, width_, height_;
	static bool toRotate_;
};