#include "stdafx.h"
#include "MainWindow.h"
#include "Controls.h"
#include "About.h"

#include "Piano.h"
#include "Canvas.h"
#include "StdErrBuffer.h"

#include "MidiParser\MidiParser_Facade.h"
#include "MidiParser\MidiError.h"
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

BOOL MainWindow::OnCreate(const HWND hWnd, LPCREATESTRUCT)
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

BOOL MainWindow::OnWindowPosChanging(HWND hWnd, LPWINDOWPOS pos)
{
	RECT rect{ 0 };
	GetWindowRect(hWnd, &rect);
	const auto height(rect.bottom - rect.top);

	GetClientRect(hWnd, &rect);
	pos->cy = pos->cx / 8 + height + rect.top - rect.bottom;

	return false;
}
void MainWindow::OnMove(const HWND hWnd, int, int)
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
inline void MainWindow::OnSize(const HWND hWnd, UINT, const int cx, const int cy)
{
	Piano::keyboard->UpdateSize(hWnd, cx, cy);
	Piano::keyboard->ReleaseWhiteKeys();
}

inline void MainWindow_OnKey(HWND, UINT vk, BOOL, int, UINT)
{
	if (vk == VK_TAB) SetFocus(Controls::hDlgControls);
}

void MainWindow::OpenMidiFile(const HWND hWnd, const LPCTSTR fileName)
{
	Controls::Reset();
	StdErrBuffer errBuf;
	try
	{
		const MidiParser_Facade midi(fileName);
		const auto numTracks(midi.GetNotes().size());
		Piano::notes.assign(numTracks, vector<set<int16_t>>());
		Piano::milliSeconds.assign(numTracks, vector<pair<unsigned, unsigned>>());
		for (size_t i(0); i < numTracks; ++i)
		{
			const auto numNotes(midi.GetNotes().at(i).size());
			Piano::notes.at(i).reserve(numNotes);
			Piano::milliSeconds.at(i).reserve(numNotes);
			if (!midi.GetNotes().at(i).empty())
			{
				vector<set<int16_t>> chords({ { midi.GetNotes().at(i).front() } });
				vector<pair<unsigned, unsigned>> timeIntervals({ make_pair(
					midi.GetMilliSeconds().at(i).front(), midi.GetMilliSeconds().at(i).front()) });
				auto lastTime(midi.GetMilliSeconds().at(i).front());
				for (auto note(midi.GetNotes().at(i).cbegin() + 1); note != midi.GetNotes().at(i).cend(); ++note)
				{
					const auto newTime(midi.GetMilliSeconds().at(i).at(
						static_cast<size_t>(note - midi.GetNotes().at(i).cbegin())));
					if (newTime - lastTime < Piano::timerTick)
					{
						chords.back().insert(*note);
						timeIntervals.back().second = newTime;
					}
					else
					{
						chords.push_back({ *note });
						timeIntervals.emplace_back(make_pair(newTime, newTime));
					}
					lastTime = newTime;
				}
				Piano::notes.at(i) = chords;
				Piano::milliSeconds.at(i) = timeIntervals;
			}
			Piano::notes.at(i).shrink_to_fit();
			Piano::milliSeconds.at(i).shrink_to_fit();
		}

		Edit_SetText(Controls::midiLog, lexical_cast<String>(regex_replace(
			midi.GetLog() + "\nERRORS:\n" + (errBuf.Get().empty() ? "None" : errBuf.Get()),
				regex{ "\n" }, "\r\n").c_str()).c_str());

		for (size_t i(0); i < midi.GetTrackNames().size(); ++i)
			if (!Piano::notes.at(i).empty())
			{
				const auto aName(midi.GetTrackNames().at(i));
				String wName((Format{ TEXT("%d ") } % i).str()
					+ String(aName.cbegin(), aName.cend()));
				ComboBox_AddString(Controls::leftHand, wName.c_str());
				ComboBox_AddString(Controls::rightHand, wName.c_str());
				ListBox_AddString(Controls::trackList, wName.c_str());

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
		if (maxElement->empty()) throw MidiError("Midi file does not contain any time data");
		ScrollBar_SetRange(Controls::scrollBar, 0, static_cast<int>(maxElement->back().second), false);
		ScrollBar_SetPos(Controls::scrollBar, 0, true);
		Button_Enable(Controls::playButton, true);
	}
	catch (const MidiError& e)
	{
		MessageBox(hWnd, lexical_cast<String>(e.what()).c_str(),
			TEXT("Midi file error"), MB_ICONHAND | MB_OK);
		Edit_SetText(Controls::midiLog,
			lexical_cast<String>(regex_replace("Error opening MIDI file:\n"
				+ string(e.what()) + '\n' + errBuf.Get(), regex{ "\n" }, "\r\n").c_str()).c_str());
		Button_Enable(Controls::playButton, false);
	}
}
void MainWindow::OnDropFiles(HWND hWnd, HDROP hDrop)
{
	TCHAR fileName[MAX_PATH] = TEXT("");
	DragQueryFile(hDrop, 0, fileName, sizeof fileName / sizeof *fileName);
	OpenMidiFile(hWnd, fileName);
	DragFinish(hDrop);
}
void MainWindow::OnCommand(HWND hWnd, int id, HWND, UINT)
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
		if (GetOpenFileName(&fileName)) OpenMidiFile(hWnd, fileName.lpstrFile);
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
	Canvas canvas(hWnd);
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