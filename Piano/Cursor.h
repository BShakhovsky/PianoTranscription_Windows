#pragma once

class Cursor : boost::noncopyable
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
	const HCURSOR hCursorOld_;
};