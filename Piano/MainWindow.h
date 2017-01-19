#pragma once

class MainWindow abstract
{
public:
	static HINSTANCE hInstance;
	static HWND hWndMain;

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
	static void OpenMidiFile(LPCTSTR);
	static void OnDropFiles(HWND, HDROP);
	static void OnCommand(HWND, int, HWND, UINT);

	static void OnPaint(HWND);

	static void OnContextMenu(HWND, HWND, int, int);
private:
	static HMENU hContextMenu_, hContextSubMenu_;
	static int dlgWidth_, width_, height_;
	static std::wstring path_;
};