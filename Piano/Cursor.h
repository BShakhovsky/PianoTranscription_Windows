#pragma once

class Cursor
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

	Cursor(const Cursor&) = delete;
	const Cursor& operator=(const Cursor&) = delete;
};