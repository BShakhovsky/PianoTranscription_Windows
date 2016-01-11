#pragma once

class Canvas : private boost::noncopyable
{
	Canvas() = delete;
public:
	explicit Canvas(HWND hWnd)
		: hWnd_(hWnd),
		hdc_(BeginPaint(hWnd_, &ps_))
	{}
	~Canvas()
	{
		EndPaint(hWnd_, &ps_);
	}

	operator HDC () const
	{
		return hdc_;
	}
private:
	PAINTSTRUCT ps_;
	HWND hWnd_;
	HDC hdc_;
};