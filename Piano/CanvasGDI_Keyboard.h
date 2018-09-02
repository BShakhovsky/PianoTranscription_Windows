#pragma once

class CanvasGdi_Keyboard
{
	CanvasGdi_Keyboard() = delete;
public:
	explicit CanvasGdi_Keyboard(HWND hWnd)
		: hWnd_(hWnd),
		hDC_(BeginPaint(hWnd_, &ps_))
	{}
	~CanvasGdi_Keyboard()
	{
		EndPaint(hWnd_, &ps_);
	}

	operator HDC () const
	{
		return hDC_;
	}
private:
	const HWND hWnd_;
	PAINTSTRUCT ps_;
	const HDC hDC_;

	CanvasGdi_Keyboard(const CanvasGdi_Keyboard&) = delete;
	const CanvasGdi_Keyboard& operator=(const CanvasGdi_Keyboard&) = delete;
};