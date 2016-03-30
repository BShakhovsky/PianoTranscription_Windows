#pragma once

class CanvasGdi : boost::noncopyable
{
	CanvasGdi() = delete;
public:
	explicit CanvasGdi(HWND hWnd)
		: hWnd_(hWnd),
		hDC_(BeginPaint(hWnd_, &ps_))
	{}
	~CanvasGdi()
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
};