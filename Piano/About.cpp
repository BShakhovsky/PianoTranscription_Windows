#include "stdafx.h"
#include "About.h"

inline BOOL OnInitDialog(HWND, HWND, LPARAM)
{
	return true;
}

inline void OnCommand(HWND hDlg, int id, HWND, UINT)
{
	if (id == IDOK || id == IDCANCEL) EndDialog(hDlg, id);
}

INT_PTR CALLBACK About(const HWND hDlg, const UINT message, const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG, OnInitDialog);
		HANDLE_MSG(hDlg, WM_COMMAND, OnCommand);
	default: return false;
	}
}