#include "stdafx.h"
#include "MainWindow.h"
#include "Controls.h"
#include "About.h"
#include "Piano.h"
#include "CanvasGdi.h"
#include "PianoKeyboard\IKeyboard.h"

using namespace std;
using namespace boost;

HINSTANCE MainWindow::hInstance = nullptr;
HWND MainWindow::hWndMain = nullptr;
int MainWindow::dlgWidth_ = 0, MainWindow::width_ = 0, MainWindow::height_ = 0;
const float MainWindow::cameraX_ = 26.0f, MainWindow::cameraY_ = 18.0f, MainWindow::cameraZ_ = 19.0f;
wstring MainWindow::path_ = TEXT("");

BOOL MainWindow::OnCreate(const HWND hWnd, const LPCREATESTRUCT)
{
	TCHAR buffer[MAX_PATH];
	GetCurrentDirectory(ARRAYSIZE(buffer), buffer);
	path_ = buffer;

	Piano::keyboard = make_shared<Keyboard3D>(hWnd, cameraX_, cameraY_, cameraZ_, path_.c_str());
	CheckMenuRadioItem(GetMenu(hWnd), IDM_2D, IDM_3D, IDM_3D, MF_BYCOMMAND);

	CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_CONTROLS), hWnd, Controls::Main);
	FORWARD_WM_COMMAND(hWnd, IDM_OPEN, nullptr, 0, SendMessage);
	ShowWindow(Controls::hDlgControls, SW_SHOWNORMAL);
	RECT rect{ 0 };
	GetWindowRect(Controls::hDlgControls, &rect);
	dlgWidth_ = rect.right - rect.left;

	return true;
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
void MainWindow::OnSize(const HWND hWnd, const UINT, const int cx, const int cy)
{
	width_ = cx;
	height_ = cy;
	Piano::keyboard->UpdateSize(hWnd, static_cast<UINT>(width_), static_cast<UINT>(height_));
	if (typeid(*Piano::keyboard) == typeid(Keyboard2D))			Piano::keyboard->ReleaseKeys();
	else if (typeid(*Piano::keyboard) == typeid(Keyboard3D))	Piano::keyboard->Update();
	else assert(!"Wrong keyboard class");
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
void MainWindow::OpenMidiFile(LPCTSTR fileName)
{
	Controls::Reset();

	FileInputStream inputFile(
		File::getCurrentWorkingDirectory().getChildFile(String(fileName)).getFullPathName());
	if (inputFile.failedToOpen())
	{
		OnMidiError((wstring(TEXT("Could not open file:\r\n")) + fileName).c_str());
		return;
	}
	MidiFile midi;
	if (!midi.readFrom(inputFile))
	{
		OnMidiError((wstring(TEXT("Not a MIDI-file:\r\n")) + fileName).c_str());
		return;
	}

	midi.convertTimestampTicksToSeconds();
	Piano::notes.assign(static_cast<size_t>(midi.getNumTracks()),
		vector<map<int16_t, float>>());
	Piano::milliSeconds.assign(static_cast<size_t>(midi.getNumTracks()),
		vector<pair<unsigned, unsigned>>());
	vector<wstring> trackNames(static_cast<size_t>(midi.getNumTracks()));
	wstring log;
	for (auto i(0); i < midi.getNumTracks(); ++i)
	{
		unsigned lastTime(0);
		const auto track(midi.getTrack(i));
		for (auto j(0); j < track->getNumEvents(); ++j)
		{
			const auto message(track->getEventPointer(j)->message);
			const auto milliSeconds(static_cast<unsigned>(message.getTimeStamp() * 1'000));

			if (message.isTextMetaEvent())
				if (message.isTrackNameEvent()) trackNames.at(static_cast<size_t>(i))
					= message.getTextFromTextMetaEvent().toWideCharPointer();
				else log.append(wstring(TEXT("\t"))
					+ message.getTextFromTextMetaEvent().toWideCharPointer() + TEXT("\r\n"));
			else if (message.isTempoMetaEvent())
				log.append((wformat{ TEXT("Time %02d:%02d:%02d Tempo %d BPM\r\n") }
					% (milliSeconds / 1'000 / 60) % (milliSeconds / 1'000 % 60) % (milliSeconds % 1'000 / 10)
					% static_cast<int>(60 / message.getTempoSecondsPerQuarterNote() + 0.5)).str());
			else if (message.isKeySignatureMetaEvent())
			{
				log.append(TEXT("Key signature: "));

				const auto nSharpsOrFlats(message.getKeySignatureNumberOfSharpsOrFlats());
				log.append(nSharpsOrFlats ? lexical_cast<wstring>(abs(nSharpsOrFlats))
					+ (nSharpsOrFlats > 0 ? TEXT(" sharps (") : TEXT(" flats ("))
					: TEXT("probably, natural "));

				if (nSharpsOrFlats >= 1)
				{
					log.append(TEXT("F"));
					if (nSharpsOrFlats >= 2)
					{
						log.append(TEXT(", C"));
						if (nSharpsOrFlats >= 3)
						{
							log.append(TEXT(", G"));
							if (nSharpsOrFlats >= 4)
							{
								log.append(TEXT(", D"));
								if (nSharpsOrFlats >= 5)
								{
									log.append(TEXT(", A"));
									if (nSharpsOrFlats >= 6)
									{
										log.append(TEXT(", E"));
										if (nSharpsOrFlats >= 7)
										{
											assert("Wrong key signature" && nSharpsOrFlats == 7);
											log.append(TEXT(", H"));
										}
									}
								}
							}
						}
					}
				}
				else if (nSharpsOrFlats <= -1)
				{
					log.append(TEXT("H"));
					if (nSharpsOrFlats <= -2)
					{
						log.append(TEXT(", E"));
						if (nSharpsOrFlats <= -3)
						{
							log.append(TEXT(", A"));
							if (nSharpsOrFlats <= -4)
							{
								log.append(TEXT(", D"));
								if (nSharpsOrFlats <= -5)
								{
									log.append(TEXT(", G"));
									if (nSharpsOrFlats <= -6)
									{
										log.append(TEXT(", C"));
										if (nSharpsOrFlats <= -7)
										{
											assert("Wrong key signature" && nSharpsOrFlats == -7);
											log.append(TEXT(", F"));
										}
									}
								}
							}
						}
					}
				}

				switch (nSharpsOrFlats)
				{
				case -7: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> Cb-Major tone\r\n") : TEXT(") -> Ab-Minor tone\r\n"));	break;
				case -6: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> Gb-Major tone\r\n") : TEXT(") -> Eb-Minor tone\r\n"));	break;
				case -5: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> Db-Major tone\r\n") : TEXT(") -> Bb-Minor tone\r\n"));	break;
				case -4: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> Ab-Major tone\r\n") : TEXT(") -> F-Minor tone\r\n"));	break;
				case -3: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> Eb-Major tone\r\n") : TEXT(") -> C-Minor tone\r\n"));	break;
				case -2: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> Bb-Major tone\r\n") : TEXT(") -> G-Minor tone\r\n"));	break;
				case -1: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> F-Major tone\r\n") : TEXT(") -> D-Minor tone\r\n"));	break;
				
				case 0: log.append(message.isKeySignatureMajorKey()
					? TEXT("C-Major tone\r\n") : TEXT("A-Minor tone\r\n"));				break;
				
				case 1: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> G-Major tone\r\n") : TEXT(") -> E-Minor tone\r\n"));	break;
				case 2: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> D-Major tone\r\n") : TEXT(") -> H-Minor tone\r\n"));	break;
				case 3: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> A#-Major tone\r\n") : TEXT(") -> F#-Minor tone\r\n"));	break;
				case 4: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> E-Major tone\r\n") : TEXT(") -> C#-Minor tone\r\n"));	break;
				case 5: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> H-Major tone\r\n") : TEXT(") -> G#-Minor tone\r\n"));	break;
				case 6: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> F#-Major tone\r\n") : TEXT(") -> D#-Minor tone\r\n"));	break;
				case 7: log.append(message.isKeySignatureMajorKey()
					? TEXT(") -> C#-Major tone\r\n") : TEXT(") -> A#-Minor tone\r\n"));	break;

				default: assert(!"Wrong key signature");
				}
			}
			else if (message.isNoteOn())
			{
				if (milliSeconds - lastTime < Piano::timerTick
					&& !Piano::notes.at(static_cast<size_t>(i)).empty())
				{
					Piano::notes.at(static_cast<size_t>(i)).back().insert(make_pair(
						static_cast<int16_t>(message.getNoteNumber()), message.getFloatVelocity()));
					Piano::milliSeconds.at(static_cast<size_t>(i)).back().second = milliSeconds;
				}
				else
				{
					Piano::notes.at(static_cast<size_t>(i)).push_back({ make_pair(
						static_cast<int16_t>(message.getNoteNumber()), message.getFloatVelocity()) });
					Piano::milliSeconds.at(static_cast<size_t>(i)).emplace_back(
						make_pair(milliSeconds, milliSeconds));
				}
				lastTime = milliSeconds;
			}
		}
	}

	Edit_SetText(Controls::midiLog, log.c_str());

	for (size_t i(0); i < trackNames.size(); ++i)
		if (!Piano::notes.at(i).empty())
		{
			trackNames.at(i).insert(0, lexical_cast<wstring>(i) + TEXT(' '));
			ComboBox_AddString(Controls::leftHand, trackNames.at(i).c_str());
			ComboBox_AddString(Controls::rightHand, trackNames.at(i).c_str());
			ListBox_AddString(Controls::trackList, trackNames.at(i).c_str());

			ComboBox_SetItemData(Controls::leftHand, ComboBox_GetCount(Controls::leftHand) - 1, i);
			ComboBox_SetItemData(Controls::rightHand, ComboBox_GetCount(Controls::rightHand) - 1, i);
			ListBox_SetItemData(Controls::trackList, ListBox_GetCount(Controls::trackList) - 1, i);
		}
	SendMessage(Controls::progressLeft, PBM_SETPOS, 0, 0);
	SendMessage(Controls::progressRight, PBM_SETPOS, 0, 0);

	Piano::tracks.clear();
	Piano::indexes.assign(Piano::notes.size(), 0);
	Piano::leftTrack.reset();
	Piano::rightTrack.reset();
	Piano::fingersLeft.assign(Piano::notes.size(), vector<vector<string>>());
	Piano::fingersRight.assign(Piano::notes.size(), vector<vector<string>>());
	const auto maxElement(max_element(Piano::milliSeconds.cbegin(), Piano::milliSeconds.cend(),
		[](const vector<pair<unsigned, unsigned>>& left,
			const vector<pair<unsigned, unsigned>>& right)
		{
			return right.empty() ? false : left.empty() ? true
				: left.back().second < right.back().second;
		}));
	if (maxElement->empty())
	{
		OnMidiError(TEXT("Midi file does not contain any time data"));
		return;
	}
	ScrollBar_SetRange(Controls::scrollBar, 0, static_cast<int>(maxElement->back().second), false);
	ScrollBar_SetPos(Controls::scrollBar, 0, true);
	Button_Enable(Controls::playButton, true);
}
void MainWindow::OnDropFiles(const HWND, const HDROP hDrop)
{
	TCHAR fileName[MAX_PATH] = TEXT("");
	DragQueryFile(hDrop, 0, fileName, sizeof fileName / sizeof *fileName);
	OpenMidiFile(fileName);
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
	}
	break;
	case IDM_2D:
		Piano::keyboard = make_shared<Keyboard2D>(hWnd, path_.c_str(),
			IsDlgButtonChecked(Controls::hDlgControls, IDC_NORM_VOL) == BST_CHECKED);
		CorrectAspectRatio();
		CheckMenuRadioItem(GetMenu(hWnd), IDM_2D, IDM_3D, static_cast<UINT>(id), MF_BYCOMMAND);
		InvalidateRect(hWnd, nullptr, false);
		break;
	case IDM_3D:
		Piano::keyboard = make_shared<Keyboard3D>(hWnd, cameraX_, cameraY_, cameraZ_, path_.c_str(),
			IsDlgButtonChecked(Controls::hDlgControls, IDC_NORM_VOL) == BST_CHECKED);
		CorrectAspectRatio();
		CheckMenuRadioItem(GetMenu(hWnd), IDM_2D, IDM_3D, static_cast<UINT>(id), MF_BYCOMMAND);
		Piano::keyboard->Update();
		break;
	case IDM_USERGUIDE: MessageBox(hWnd, TEXT(R"(How to use:

1. It is not possible to play on this piano using mouse nor keyboard.  It only plays *.mid (MIDI) or *.kar (Karaoke) files (you can find plenty of them over the internet). Drag-and-drop any MIDI- or Karaoke-file onto the applcation.

2. Select appropriate track for left hand, and another track for right hand.  Finger numbers for left hand will be drawn with blue color, for right hand - with red.  If you are not interested in finger numbers, you can skip this step.
(!) This option is EXPERIMENTAL, and finger numbers are not correct in 25% of cases!  Keep this in mind and do not blindly beleive them.

3. Select any additional tracks in "Remaining Tracks" list, if you want.  Finger numbers for those additional tracks will not be calculated or drawn.
Do not choose percussion-tracks (like "Drums", "Rythms", "Hit", "Blow", "Strike", "Clash", etc.) if you want a clearer sound.

4. If you want to go forward or backwards chord-by-chord, you can use scroll-bar left or right button.

5. Or if you want just to play the song in real time, press "Play" button.

6. By default each note is being played with different volume (note-volumes are also imported from MIDI-file).  If you want all notes to be played with the same maximal loudness, check "Normalize volume" box.

7. Enjoy :))"), (
#ifdef UNICODE
	wstring
#else
	string
#endif
	(TEXT("\"")) + Piano::windowTitle + TEXT("\" - User Guide")).c_str(), MB_OK | MB_ICONASTERISK);
	break;
	case IDM_ABOUT:
		DialogBox(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
		break;
	case IDM_EXIT:
		DestroyWindow(hWnd);
	}
}

void MainWindow::OnPaint(const HWND hWnd)
{
	CanvasGdi canvas(hWnd);
	Piano::keyboard->Update(canvas);

	if (typeid(*Piano::keyboard) == typeid(Keyboard2D)) Piano::keyboard->ReleaseKeys();
	else assert("Wrong keyboard class" && typeid(*Piano::keyboard) == typeid(Keyboard3D));
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

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}