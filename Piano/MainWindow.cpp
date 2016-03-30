#include "stdafx.h"
#include "MainWindow.h"
#include "Controls.h"
#include "About.h"
#include "Piano.h"
#include "CanvasGdi.h"
#include "ResourceLoader.h"
#pragma warning(push)
#pragma warning(disable:4711)
#	include "PianoKeyboard\Keyboard.h"
#pragma warning(pop)
#include "PianoSound\Sound_Facade.h"
#include "PianoSound\SoundError.h"

using namespace std;
using namespace boost;

HWND MainWindow::hWndMain = nullptr;
int MainWindow::dlgWidth_ = 0;

BOOL MainWindow::OnCreate(const HWND hWnd, const LPCREATESTRUCT)
{
	try
	{
		Piano::sound = make_shared<Sound_Facade>();
	}
	catch (const SoundError& e)
	{
		MessageBoxA(hWnd, e.what(), "Sound error", MB_ICONHAND | MB_OK);
	}

	CreateDialog(GetWindowInstance(hWnd), MAKEINTRESOURCE(IDD_CONTROLS), hWnd, Controls::Main);
	FORWARD_WM_COMMAND(hWnd, IDM_OPEN, nullptr, 0, SendMessage);
	ShowWindow(Controls::hDlgControls, SW_SHOWNORMAL);
	RECT rect{ 0 };
	GetWindowRect(Controls::hDlgControls, &rect);
	dlgWidth_ = rect.right - rect.left;

	return true;
}

BOOL MainWindow::OnWindowPosChanging(const HWND hWnd, const LPWINDOWPOS pos)
{
	RECT rect{ 0 };
	GetWindowRect(hWnd, &rect);
	const auto height(rect.bottom - rect.top);

	GetClientRect(hWnd, &rect);
	pos->cy = pos->cx / 8 + height + rect.top - rect.bottom;

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
inline void MainWindow::OnSize(const HWND hWnd, const UINT, const int cx, const int cy)
{
	Piano::keyboard->UpdateSize(hWnd, cx, cy);
	Piano::keyboard->ReleaseWhiteKeys();
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
		vector<set<int16_t>>());
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
				log.append((wformat{ TEXT("Time %02d:%02d:%02d Tempo %d BPM\r\n") } %
					(milliSeconds / 1'000 / 60) % (milliSeconds / 1'000 % 60) % (milliSeconds % 1'000 / 10)
					% static_cast<int>(60 / message.getTempoSecondsPerQuarterNote() + 0.5)).str());
			else if (message.isKeySignatureMetaEvent())
			{
				log.append(TEXT("Key signature: "));
				const auto nSharpsOrFlats(message.getKeySignatureNumberOfSharpsOrFlats());
				switch (nSharpsOrFlats)
				{
				case 0:	log.append(TEXT("Natural key signature, "));						break;
				case -7: case -6: case -5: case -4: case -3: case -2: case -1:
					log.append(lexical_cast<wstring>(-nSharpsOrFlats) + TEXT(" flats, "));	break;
				case 7: case 6: case 5: case 4: case 3: case 2: case 1:
					log.append(lexical_cast<wstring>(nSharpsOrFlats) + TEXT(" sharps, "));	break;
				default: assert("Wrong key signature");
				}
				log.append(message.isKeySignatureMajorKey()
					? nSharpsOrFlats ? TEXT("Major key\r\n") : TEXT("C-Major\r\n")
					: nSharpsOrFlats ? TEXT("Minor key\r\n") : TEXT("A-Minor\r\n"));
			}
			else if (message.isNoteOn())
			{
				if (milliSeconds - lastTime < Piano::timerTick
					&& !Piano::notes.at(static_cast<size_t>(i)).empty())
				{
					Piano::notes.at(static_cast<size_t>(i)).back().insert(
						static_cast<int16_t>(message.getNoteNumber()));
					Piano::milliSeconds.at(static_cast<size_t>(i)).back().second = milliSeconds;
				}
				else
				{
					Piano::notes.at(static_cast<size_t>(i)).push_back({
						static_cast<int16_t>(message.getNoteNumber()) });
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
	case IDM_USERGUIDE:
	{
		ResourceLoader resource(IDR_README, TEXT("Text"));
		MessageBoxA(hWnd, static_cast<const char*>(resource.Data()), "User Guide", MB_OK | MB_ICONASTERISK);
	}
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
	Piano::keyboard->Draw(canvas);
	Piano::keyboard->ReleaseWhiteKeys();
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
	case WM_SIZING:								OnMove(hWnd, 0, 0); return FALSE; break;
		HANDLE_MSG(hWnd, WM_SIZE,				OnSize);

		HANDLE_MSG(hWnd, WM_KEYDOWN,			MainWindow_OnKey);

		HANDLE_MSG(hWnd, WM_DROPFILES,			OnDropFiles);
		HANDLE_MSG(hWnd, WM_COMMAND,			OnCommand);

		HANDLE_MSG(hWnd, WM_PAINT,				OnPaint);

	default: return DefWindowProc(hWnd, message, wParam, lParam);
	}
}