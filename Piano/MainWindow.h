#pragma once

class MainWindow abstract
{
public:
	static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
private:
	static BOOL OnCreate(HWND, LPCREATESTRUCT);
	static void OnDestroy(HWND)
	{
		PostQuitMessage(0);
	}

	static BOOL OnWindowPosChanging(HWND, LPWINDOWPOS pos)
	{
		pos->cy = pos->cx / 6;
		return false;
	}
	static void OnMove(HWND, int, int);
	static void OnSize(HWND, UINT, int, int);

	static void OpenMidiFile(HWND, LPCTSTR);
	static void OnDropFiles(HWND, HDROP);
	static void OnCommand(HWND, int, HWND, UINT);

	static void OnPaint(HWND);
private:
	static HWND controls_;
	static int dlgWidth_;
};