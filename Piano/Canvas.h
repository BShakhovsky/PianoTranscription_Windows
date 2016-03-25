#pragma once

class Canvas : boost::noncopyable
{
	Canvas() = delete;
public:
	explicit Canvas(HWND hWnd)
		: hWnd_(hWnd),
		hDC_(BeginPaint(hWnd_, &ps_))
	{}
	~Canvas()
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