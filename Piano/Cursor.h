#pragma once

class Cursor : private boost::noncopyable
{
public:
	Cursor() :
		hCursorOld_(SetCursor(LoadCursor(nullptr, IDC_WAIT)))
	{}
	~Cursor()
	{
		SetCursor(hCursorOld_);
	}
private:
	HCURSOR hCursorOld_;
};