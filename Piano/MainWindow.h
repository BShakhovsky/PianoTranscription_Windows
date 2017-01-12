#pragma once

class MainWindow abstract
{
public:
	static HINSTANCE hInstance;
	static HWND hWndMain;

	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
private:
	static BOOL OnCreate(HWND, LPCREATESTRUCT);

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
private:
	static int dlgWidth_, width_, height_;
	const static float cameraX_, cameraY_, cameraZ_;
	static std::wstring path_;
};