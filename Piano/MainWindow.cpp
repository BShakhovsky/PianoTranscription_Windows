#include "stdafx.h"
#include "MainWindow.h"
#include "Controls.h"
#include "Spectrogram.h"
#include "About.h"
#include "Piano.h"
#include "MidiParser.h"
#include "MidiError.h"
#include "CanvasGdi_Keyboard.h"
#include "Keyboard\IKeyboard.h"

using namespace std;
using namespace boost;

HINSTANCE MainWindow::hInstance = nullptr;
HWND MainWindow::hWndMain = nullptr;
TCHAR MainWindow::path[] = TEXT("");

HMENU MainWindow::hContextMenu_ = nullptr, MainWindow::hContextSubMenu_ = nullptr;
int MainWindow::dlgWidth_ = 0, MainWindow::width_ = 0, MainWindow::height_ = 0;
bool MainWindow::toRotate_ = false;

BOOL MainWindow::OnCreate(const HWND hWnd, const LPCREATESTRUCT)
{
	hContextMenu_ = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_CONTEXT_MENU));
	hContextSubMenu_ = GetSubMenu(hContextMenu_, 0);

	GetCurrentDirectory(ARRAYSIZE(path), path);

	Piano::keyboard = make_shared<Keyboard3D>(hWnd, path);
	CheckMenuRadioItem(GetMenu(hWnd), IDM_2D, IDM_3D, IDM_3D, MF_BYCOMMAND);

	// Potentially throwing function passed to extern C function
	// Undefined behavior may occur if this function throws
#pragma warning(suppress:5039)
	CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_CONTROLS), hWnd, Controls::Main);
	FORWARD_WM_COMMAND(hWnd, IDM_OPEN, nullptr, 0, SendMessage);
	ShowWindow(Controls::hDlgControls, SW_SHOWNORMAL);
	RECT rect{ 0 };
	GetWindowRect(Controls::hDlgControls, &rect);
	dlgWidth_ = rect.right - rect.left;

	return true;
}
void MainWindow::OnDestroy(HWND)
{
	DestroyMenu(hContextMenu_);
	PostQuitMessage(0);
}

void MainWindow::CorrectAspectRatio()
{
	RECT rect{ 0 };
	GetWindowRect(hWndMain, &rect);
	SetWindowPos(hWndMain, HWND_TOP, 0, 0, rect.right - rect.left, height_, SWP_NOMOVE | SWP_NOZORDER);
	OnMove(hWndMain, 0, 0);
}
BOOL MainWindow::OnWindowPosChanging(const HWND hWnd, const LPWINDOWPOS pos)
{
	float aspectRatio(0);

	if (Piano::keyboard)
		if (typeid(*Piano::keyboard) == typeid(Keyboard2D))
			aspectRatio = 8;
		else if (typeid(*Piano::keyboard) == typeid(Keyboard3D))
			aspectRatio = 3.5f;
		else assert(!"Wrong keyboard class");

	RECT rect{ 0 };
	GetWindowRect(hWnd, &rect);
	const auto height(rect.bottom - rect.top);

	GetClientRect(hWnd, &rect);
	height_ = pos->cy = static_cast<int>(pos->cx / aspectRatio) + height + rect.top - rect.bottom;

	return false;
}
void MainWindow::OnMove(const HWND hWnd, const int, const int)
{
	RECT rect{ 0 };

	GetWindowRect(hWnd, &rect);
	const auto left((rect.left + rect.right - dlgWidth_) / 2), bottom(rect.bottom);
	
	GetWindowRect(Controls::hDlgControls, &rect);
	static const auto height(rect.bottom - rect.top);
	
	GetClientRect(Controls::hDlgControls, &rect);
	
	SetWindowPos(Controls::hDlgControls, HWND_TOP, left, bottom
		+ rect.bottom - rect.top - height - 2, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}
void MainWindow::OnSize(const HWND, const UINT, const int cx, const int cy)
{
	width_ = cx;
	height_ = cy;
	Piano::keyboard->UpdateSize(static_cast<UINT>(width_), static_cast<UINT>(height_));
	if (typeid(*Piano::keyboard) == typeid(Keyboard2D))			Piano::keyboard->ReleaseKeys();
	else if (typeid(*Piano::keyboard) == typeid(Keyboard3D))	Piano::keyboard->Update();
	else assert(!"Wrong keyboard class");

	toRotate_ = false;
}

inline void MainWindow_OnKey(const HWND, const UINT vk, const BOOL, const int, const UINT)
{
	if (vk == VK_TAB) SetFocus(Controls::hDlgControls);
}

void MainWindow::OnMidiError(LPCTSTR msg)
{
	MessageBox(hWndMain, msg, TEXT("MIDI file error"), MB_OK | MB_ICONHAND);
	Edit_SetText(Controls::midiLog, msg);
	Button_Enable(Controls::playButton, false);
}
void MainWindow::OnMidiSuccess()
{
	SendMessage(Controls::progressLeft, PBM_SETPOS, 0, 0);
	SendMessage(Controls::progressRight, PBM_SETPOS, 0, 0);

	Piano::tracks.clear();
	Piano::indexes.assign(Piano::notes.size(), 0);
	Piano::leftTrack.reset();
	Piano::rightTrack.reset();
	Piano::fingersLeft.assign(Piano::notes.size(), vector<vector<string>>());
	Piano::fingersRight.assign(Piano::notes.size(), vector<vector<string>>());

	Button_Enable(Controls::checkAll, false);

	assert("There must appear the same tracks for both hands"
		&& ComboBox_GetCount(Controls::leftHand) == ComboBox_GetCount(Controls::rightHand)
		&& ComboBox_GetCount(Controls::leftHand) >= 1);
	if (ComboBox_GetCount(Controls::leftHand) == 1) MessageBox(hWndMain,
		TEXT("This MIDI-file does not contain any non-percussion notes.\nNothing to play on piano."),
		TEXT("Midi info"), MB_OK | MB_ICONASTERISK);
	else
	{
		const auto maxElement(max_element(Piano::milliSeconds.cbegin(), Piano::milliSeconds.cend(),
			[](const vector<pair<unsigned, unsigned>>& left,
				const vector<pair<unsigned, unsigned>>& right)
		{
			return right.empty() ? false : left.empty() ? true
				: left.back().second < right.back().second;
		}));
		if (maxElement->empty())
			OnMidiError(TEXT("This MIDI-file does not contain any time data.\nCannot play it :("));
		else
		{
			ScrollBar_SetRange(Controls::scrollBar, 0, static_cast<int>(maxElement->back().second), false);
			ScrollBar_SetPos(Controls::scrollBar, 0, true);
			Button_Enable(Controls::playButton, true);
			Button_Enable(Controls::checkAll, true);
		}
	}
}
bool MainWindow::OpenMidiFile(LPCTSTR fileName)
{
	Controls::Reset();
	try
	{
		MidiParser midi(fileName);

		Edit_SetText(Controls::midiLog, midi.GetLog());

		auto trackNames(midi.GetTrackNames());
		for (size_t i(0); i < trackNames.size(); ++i)
			if (!Piano::notes.at(i).empty())
			{
				trackNames.at(i).insert(0, lexical_cast<wstring>(i) + TEXT(' '));
				ListBox_AddString(Controls::trackList, trackNames.at(i).c_str());
				ListBox_SetItemData(Controls::trackList, ListBox_GetCount(Controls::trackList) - 1, i);
				if (!Piano::percussions.at(i))
				{
					ComboBox_AddString(Controls::leftHand, trackNames.at(i).c_str());
					ComboBox_AddString(Controls::rightHand, trackNames.at(i).c_str());
					ComboBox_SetItemData(Controls::leftHand, ComboBox_GetCount(Controls::leftHand) - 1, i);
					ComboBox_SetItemData(Controls::rightHand, ComboBox_GetCount(Controls::rightHand) - 1, i);
				}
				else
				{
					Piano::notes.at(i).clear();
					Piano::milliSeconds.at(i).clear();
				}
			}

		OnMidiSuccess();
		return true;
	}
	catch (const MidiError& e)
	{
		OnMidiError(e.RusWhat());
		return false;
	}
}
void MainWindow::OpenMediaFile(LPCTSTR fileName)
{
	Spectrogram::mediaFile = fileName;
	// Pointer to potentially throwing function passed to extern C function under -EHc.
	// Undefined behavior may occur if this function throws
#pragma warning(suppress:5039)
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_SPECTRUM), hWndMain, Spectrogram::Main);
	if (!Spectrogram::midiFile.empty())
	{
		const wstring midiFileW(Spectrogram::midiFile.cbegin(), Spectrogram::midiFile.cend());
		OpenMidiFile(midiFileW.c_str());
	}
}
void MainWindow::OnDropFiles(const HWND, const HDROP hDrop)
{
	TCHAR fileName[MAX_PATH] = TEXT("");
	DragQueryFile(hDrop, 0, fileName, sizeof fileName / sizeof *fileName);
	if (!OpenMidiFile(fileName)) OpenMediaFile(fileName);
	DragFinish(hDrop);
}
void MainWindow::OnCommand(const HWND hWnd, const int id, const HWND, const UINT)
{
	switch (id)
	{
	case IDM_OPEN:
	{
		OPENFILENAME fileName{ sizeof fileName, hWnd };
		fileName.lpstrFilter = TEXT("MIDI files (*.mid)\0")	TEXT("*.mid*\0")
							TEXT("Karaoke files (*.kar)\0")	TEXT("*.kar*\0")
								TEXT("All files\0")			TEXT("*.*\0");
		TCHAR buf[MAX_PATH] = TEXT("");
		fileName.lpstrFile = buf;
		fileName.nMaxFile = sizeof buf / sizeof *buf;
		fileName.Flags = OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&fileName)) OpenMidiFile(fileName.lpstrFile);

		toRotate_ = false;
	}
	break;
	case IDM_OPEN_MEDIA:
	{
		OPENFILENAME fileName{ sizeof fileName, hWnd };
		fileName.lpstrFilter = TEXT("All files\0") TEXT("*.*\0");
		TCHAR buf[MAX_PATH] = TEXT("");
		fileName.lpstrFile = buf;
		fileName.nMaxFile = sizeof buf / sizeof *buf;
		fileName.Flags = OFN_FILEMUSTEXIST;
		if (GetOpenFileName(&fileName)) OpenMediaFile(fileName.lpstrFile);
	}
	break;
	case IDM_2D:
		Piano::keyboard = make_shared<Keyboard2D>(hWnd, path,
			IsDlgButtonChecked(Controls::hDlgControls, IDC_NORM_VOL) == BST_CHECKED);
		CorrectAspectRatio();
		CheckMenuRadioItem(GetMenu(hWnd), IDM_2D, IDM_3D, static_cast<UINT>(id), MF_BYCOMMAND);
		EnableMenuItem(hContextSubMenu_, IDM_DEFAULT_3D_POS, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		InvalidateRect(hWnd, nullptr, false);
		break;
	case IDM_3D:
		if (typeid(*Piano::keyboard) == typeid(Keyboard3D)) Piano::keyboard->Restore3DPosition();
		else
		{
			Piano::keyboard = make_shared<Keyboard3D>(hWnd, path,
				IsDlgButtonChecked(Controls::hDlgControls, IDC_NORM_VOL) == BST_CHECKED);
			CorrectAspectRatio();
			CheckMenuRadioItem(GetMenu(hWnd), IDM_2D, IDM_3D, static_cast<UINT>(id), MF_BYCOMMAND);
			EnableMenuItem(hContextSubMenu_, IDM_DEFAULT_3D_POS, MF_BYCOMMAND | MF_ENABLED);
			Piano::keyboard->Update();
		}
		break;
	case IDM_USERGUIDE: MessageBox(hWnd, TEXT(R"(1. Drag-and-drop any MIDI- or Karaoke-file onto the application.

2. Or alternatively, most common audio formats are now supported (such as MP3, WAV, etc.).  However, no instrument information is extracted, and all transcribed notes get combined into one part of a MIDI-file.  The accuracy is obviously higher for solo piano pieces.  Accuracy for piano pieces is around 75%.

3. Select appropriate track for left hand, and another track for right hand (not applicable for MIDI generated from audio/video, because there will be just one track).  Finger numbers for left hand will be drawn with blue color, for right hand - with red.  Accuracy of finger numbers is around 75%.  If you are not interested in finger numbers, you can skip this step.

4. Select any additional tracks in "Remaining Tracks" list, if you want.  Finger numbers for those additional tracks will not be calculated or drawn.  Percussion-tracks will be disabled.

5. If you want to go forward or backwards chord-by-chord, you can use scroll-bar left or right button.  Or if you want just to play the song in real time, press "Play" button.  For smoother performance, try not to move mouse over the program while playing in 3D-mode.

6. By default, each note is being played with different volume.  If you want all notes to be played with the same maximal loudness, check "Normalize volume" box.

7. Use left mouse button to rotate, middle (or press mouse wheel) to move, scroll mouse wheel to zoom, double-click on mouse wheel to fit the piano inside the window.  Right click --> choose context menu to restore the default 3D-piano position.

8. Enjoy :))"), (
#ifdef UNICODE
	wstring
#else
	string
#endif
	(TEXT("\"")) + Piano::windowTitle + TEXT("\" - User Guide")).c_str(), MB_OK | MB_ICONASTERISK);
	break;
	case IDM_ABOUT:
		// Potentially throwing function passed to extern C function
		// Undefined behavior may occur if this function throws
#pragma warning(suppress:5039)
		DialogBox(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
	}
}

void MainWindow::OnPaint(const HWND hWnd)
{
	CanvasGdi_Keyboard canvas(hWnd);
	Piano::keyboard->Update(canvas);

	if (typeid(*Piano::keyboard) == typeid(Keyboard2D)) Piano::keyboard->ReleaseKeys();
	else assert("Wrong keyboard class" && typeid(*Piano::keyboard) == typeid(Keyboard3D));
}

void OnMouseWheel(HWND, int, int, int delta, UINT)
{
	Piano::keyboard->Zoom3D(delta / WHEEL_DELTA);
}
void OnMButtonDblClick(HWND, BOOL, int, int, UINT)
{
	Piano::keyboard->Fit3DToWindow();
}
void OnMButtonDown(HWND, BOOL, int x, int y, UINT)
{
	Piano::keyboard->Move3DStart(x, y);
}
void OnLButtonDown(HWND, BOOL, int x, int y, UINT)
{
	Piano::keyboard->Rotate3DStart(x, y);
}
void MainWindow::OnMouseMove(const HWND, const int x, const int y, const UINT keyFlags)
{
	Piano::keyboard->On3DMouseMove(x, y, (keyFlags & MK_MBUTTON) != 0,
		(keyFlags & MK_LBUTTON) && toRotate_);
	if (!toRotate_) toRotate_ = true;

	// Do not freeze 3D-Piano while moving the mouse
	Controls::OnMouseMove(Controls::hDlgControls, x, y, keyFlags);
}
void MainWindow::OnContextMenu(const HWND hWnd, const HWND, const int xPos, const int yPos)
{
	if (TrackPopupMenu(hContextSubMenu_, TPM_CENTERALIGN | TPM_VCENTERALIGN | TPM_RETURNCMD,
		xPos, yPos, 0, hWnd, nullptr) == IDM_DEFAULT_3D_POS) Piano::keyboard->Restore3DPosition();
}

LRESULT CALLBACK MainWindow::WndProc(const HWND hWnd, const UINT message,
	const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hWnd, WM_CREATE,				OnCreate);
		HANDLE_MSG(hWnd, WM_DESTROY,			OnDestroy);

		HANDLE_MSG(hWnd, WM_WINDOWPOSCHANGING,	OnWindowPosChanging);
		HANDLE_MSG(hWnd, WM_MOVE,				OnMove);
	case WM_SIZING:								OnMove(hWnd, 0, 0); return FALSE;
		HANDLE_MSG(hWnd, WM_SIZE,				OnSize);

		HANDLE_MSG(hWnd, WM_KEYDOWN,			MainWindow_OnKey);

		HANDLE_MSG(hWnd, WM_DROPFILES,			OnDropFiles);
		HANDLE_MSG(hWnd, WM_COMMAND,			OnCommand);

		HANDLE_MSG(hWnd, WM_PAINT,				OnPaint);


		HANDLE_MSG(hWnd, WM_MOUSEWHEEL,			OnMouseWheel);
		HANDLE_MSG(hWnd, WM_MBUTTONDBLCLK,		OnMButtonDblClick);

		HANDLE_MSG(hWnd, WM_MBUTTONDOWN,		OnMButtonDown);
		HANDLE_MSG(hWnd, WM_LBUTTONDOWN,		OnLButtonDown);
		HANDLE_MSG(hWnd, WM_MOUSEMOVE,			OnMouseMove);

		HANDLE_MSG(hWnd, WM_CONTEXTMENU,		OnContextMenu);

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}