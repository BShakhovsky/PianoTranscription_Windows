#include "stdafx.h"
#include "Spectrogram.h"
#include "MainWindow.h"
#include "resource.h"
#include "Piano 2 Midi/PianoToMidi/PianoToMidi_Win.h"

using namespace std;

LPCTSTR	Spectrogram::mediaFile = nullptr;
string Spectrogram::midiFile = "";

std::unique_ptr<PianoToMidi_Win> Spectrogram::media = nullptr;

BOOL Spectrogram::OnInitDialog(const HWND hDlg, const HWND, const LPARAM)
{
	assert(mediaFile && "Unknown audio file name");
	midiFile = "";
	media = make_unique<PianoToMidi_Win>(hDlg, IDB_CALC_SPECTR, IDL_SPECTR_TITLE,
		IDB_CONVERT, IDC_CNN_PROG, IDE_SPECTR_LOG, IDP_SPECTR_PIC);
	ShowWindow(hDlg, SW_SHOWMAXIMIZED);
	media->FFmpegDecode(mediaFile);
	return true;
}

void Spectrogram::OnDestroyDialog(const HWND) { media = nullptr; }

void Spectrogram::OnSize(const HWND, const UINT, const int cx, const int cy)
{
	if (media) media->OnSize(cx, cy);
}

void Spectrogram::OnPaint(const HWND) { if (media) media->OnPaint(); }

void Spectrogram::OnCommand(const HWND hDlg, const int id, const HWND, const UINT)
{
	switch (id)
	{
	case IDOK: case IDCANCEL:	EndDialog(hDlg, id);				break;
	case IDB_CALC_SPECTR:		media->Spectrum(MainWindow::path);	break;
	case IDB_CONVERT:			midiFile = media->Convert(mediaFile);
	}
}

INT_PTR CALLBACK Spectrogram::Main(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG,	OnInitDialog);
		HANDLE_MSG(hDlg, WM_DESTROY,	OnDestroyDialog);

		HANDLE_MSG(hDlg, WM_SIZE,		OnSize);
		HANDLE_MSG(hDlg, WM_PAINT,		OnPaint);
		HANDLE_MSG(hDlg, WM_COMMAND,	OnCommand);

	case WM_ENTERSIZEMOVE:	if (media) media->OnEnterSizeMove(); break;
	case WM_EXITSIZEMOVE:	if (media) media->OnExitSizeMove();
	}
	return false;
}