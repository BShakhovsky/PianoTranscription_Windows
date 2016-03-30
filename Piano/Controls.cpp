#include "stdafx.h"
#include "Controls.h"
#include "MainWindow.h"
#include "Piano.h"
#include "Cursor.h"
#include "PianoFingering\TrellisGraph_Facade.h"
#pragma warning(push)
#pragma warning(disable:4711)
#	include "PianoKeyboard\IKeyboard.h"
#pragma warning(pop)

using namespace std;
using namespace boost;

HWND Controls::hDlgControls		= nullptr;

HWND Controls::midiLog			= nullptr;
HWND Controls::leftHand			= nullptr;
HWND Controls::rightHand		= nullptr;
HWND Controls::progressLeft		= nullptr;
HWND Controls::progressRight	= nullptr;
HWND Controls::trackList		= nullptr;

HWND Controls::scrollBar		= nullptr;
HWND Controls::playButton		= nullptr;

HWND Controls::time_			= nullptr;
bool Controls::isPlaying_		= false;
DWORD Controls::start_			= 0;

void Controls::Reset()
{
	if (isPlaying_) FORWARD_WM_COMMAND(hDlgControls, IDB_PLAY, playButton, 0, SendMessage);
	ComboBox_ResetContent(leftHand);
	ComboBox_ResetContent(rightHand);
	ListBox_ResetContent(trackList);
	OnInitDialog(hDlgControls, nullptr, 0);
}
BOOL Controls::OnInitDialog(const HWND hDlg, HWND, LPARAM)
{
	hDlgControls	= hDlg;

	scrollBar		= GetDlgItem(hDlg, IDC_SCROLLBAR);

	playButton		= GetDlgItem(hDlg, IDB_PLAY);

	midiLog			= GetDlgItem(hDlg, IDC_MIDI_LOG);
	time_			= GetDlgItem(hDlg, IDC_TIME);

	leftHand		= GetDlgItem(hDlg, IDC_LEFT_HAND);
	rightHand		= GetDlgItem(hDlg, IDC_RIGHT_HAND);
	progressLeft	= GetDlgItem(hDlg, IDC_PROGRESS_LEFT);
	progressRight	= GetDlgItem(hDlg, IDC_PROGRESS_RIGHT);
	
	trackList		= GetDlgItem(hDlg, IDC_TRACKS);

	Edit_SetText(midiLog, TEXT("MIDI info and errors if any"));

	Edit_SetText(time_, TEXT("Time 0:00:00"));

	ComboBox_AddString(leftHand, TEXT("None"));
	ComboBox_AddString(rightHand, TEXT("None"));
	ComboBox_SetItemData(leftHand, 0, -1);
	ComboBox_SetItemData(rightHand, 0, -1);
	ComboBox_SetCurSel(leftHand, 0);
	ComboBox_SetCurSel(rightHand, 0);

	return true;
}

inline void Controls::StopPlaying()
{
	if (isPlaying_) FORWARD_WM_COMMAND(hDlgControls, IDB_PLAY, playButton, 0, SendMessage);
}
void Controls::UpdateTime(const DWORD dwTime)
{
	if (dwTime < start_) start_ = dwTime;
	const auto currTime(dwTime - start_);
	ScrollBar_SetPos(scrollBar, static_cast<int>(currTime), true);

	const auto seconds(currTime / 1'000), milliSec(currTime % 1'000);
	Edit_SetText(time_, (wformat{ TEXT("Time %u:%02u:%02u") } %
		(seconds / 60) % (seconds % 60) % (milliSec / 10)).str().c_str());
}
void AssignFinger(const vector<vector<vector<string>>>& fingers, size_t trackNo, bool leftHand = false)
{
	for (size_t i(0); i < fingers.at(trackNo).at(Piano::indexes.at(trackNo)).size(); ++i)
	{
		auto note(Piano::notes.at(trackNo).at(Piano::indexes.at(trackNo)).cbegin());
		advance(note, i);
		Piano::keyboard->AssignFinger(*note, fingers.at(trackNo)
			.at(Piano::indexes.at(trackNo)).at(i).c_str(), leftHand);
	}
}
int Controls::PlayTrack(const size_t trackNo, const DWORD dwTime)
{
	auto result(0);

	for (; Piano::indexes.at(trackNo) < Piano::notes.at(trackNo).size()
		&& static_cast<time_t>(dwTime) - static_cast<time_t>(start_
			+ Piano::milliSeconds.at(trackNo).at(Piano::indexes.at(trackNo)).first) >= 0;
		++Piano::indexes.at(trackNo))
	{
		for (const auto& note : Piano::notes.at(trackNo).at(Piano::indexes.at(trackNo)))
		{
			Piano::keyboard->PressKey(note);
			if (Piano::leftTrack && *Piano::leftTrack == trackNo)
				AssignFinger(Piano::fingersLeft, trackNo, true);
			if (Piano::rightTrack && *Piano::rightTrack == trackNo)
				AssignFinger(Piano::fingersRight, trackNo);
		}
		result = 1;
	}
	if (Piano::indexes.at(trackNo) < Piano::notes.at(trackNo).size() && (result &&
		static_cast<time_t>(dwTime) - static_cast<time_t>(start_
			+ Piano::milliSeconds.at(trackNo).at(Piano::indexes.at(trackNo)).first) >= 0))
		result = INT16_MIN;

	return result;
}
bool Controls::OnTimer(const HWND hWnd, const DWORD dwTime)
{
	auto result(false);

	UpdateTime(dwTime);
	if (accumulate(Piano::tracks.cbegin(), Piano::tracks.cend(), 0, [dwTime](int val, size_t track)
		{
			return val + PlayTrack(track, dwTime);
		}
		) > 0)	// std::any_of() and std::all_of() return as soon as result is found,
				// but we need to play all tracks anyway
	{
		InvalidateRect(hWnd, nullptr, false);
		result = true;
	}

	if (isPlaying_ && all_of(Piano::tracks.cbegin(), Piano::tracks.cend(), [](size_t track)
		{
			return Piano::indexes.at(track) >= Piano::notes.at(track).size();
		}))
	{
		StopPlaying();
		ScrollBar_SetPos(scrollBar, 0, true);
		fill(Piano::indexes.begin(), Piano::indexes.end(), 0);
	}

	return result;
}
void CALLBACK Controls::OnTimer(const HWND hWnd, UINT, UINT_PTR, const DWORD dwTime)
{
	OnTimer(hWnd, dwTime);
}


void Controls::RewindTracks(const int pos)
{
	for (const auto& track : Piano::tracks) Piano::indexes.at(track) = static_cast<size_t>(
		lower_bound(Piano::milliSeconds.at(track).cbegin(), Piano::milliSeconds.at(track).cend(),
			make_pair(static_cast<unsigned>(pos), static_cast<unsigned>(pos)),
			[](const pair<unsigned, unsigned>& lhs, const pair<unsigned, unsigned>& rhs)
			{
				return lhs.first < rhs.first;
			})
		- Piano::milliSeconds.at(track).cbegin());
}
void Controls::UpdateScrollBar(int pos)
{
	pos += ScrollBar_GetPos(scrollBar);

	if (pos < 0) pos = 0;
	auto maxPos(0);
	ScrollBar_GetRange(scrollBar, nullptr, &maxPos);
	if (pos > maxPos) pos = maxPos;

	RewindTracks(pos);
	if (isPlaying_) start_ += ScrollBar_GetPos(scrollBar) - pos;
	else UpdateTime(static_cast<DWORD>(pos));
}
void Controls::NextChord()
{
	if (Piano::tracks.empty())
		MessageBox(hDlgControls, TEXT("No tracks are chosen, nothing to play yet"),
			TEXT("Choose tracks"), MB_ICONASTERISK);
	else for (;;)
	{
		const auto track(*min_element(Piano::tracks.cbegin(), Piano::tracks.cend(),
			[](size_t left, size_t right)
			{
				return Piano::indexes.at(left) >= Piano::milliSeconds.at(left).size()	? false
					: Piano::indexes.at(right) >= Piano::milliSeconds.at(right).size()	? true
					: Piano::milliSeconds.at(left).at(Piano::indexes.at(left)).first
						< Piano::milliSeconds.at(right).at(Piano::indexes.at(right)).first;
			}));
		if (Piano::indexes.at(track) >= Piano::milliSeconds.at(track).size()
				|| OnTimer(GetParent(hDlgControls), start_ + Piano::milliSeconds
					.at(track).at(Piano::indexes.at(track)).second))
			break;
	}
}
void Controls::PrevChord()
{
	if (!Piano::tracks.empty())
	{
		auto track(*max_element(Piano::tracks.cbegin(), Piano::tracks.cend(),
			[](size_t left, size_t right)
			{
				return !Piano::indexes.at(right) ? false : !Piano::indexes.at(left) ? true
					: Piano::milliSeconds.at(left).at(Piano::indexes.at(left) - 1).second
						< Piano::milliSeconds.at(right).at(Piano::indexes.at(right) - 1).second;
			}));
		for (auto finish(false); !finish;)
		{
			if (Piano::indexes.at(track)) --Piano::indexes.at(track);
			auto prevTime(Piano::milliSeconds.at(track).at(Piano::indexes.at(track)).first);
			for (; Piano::indexes.at(track) && prevTime
						- Piano::milliSeconds.at(track).at(Piano::indexes.at(track) - 1).second
					< Piano::timerTick;
				prevTime = Piano::milliSeconds.at(track).at(--Piano::indexes.at(track)).second)
				;
			finish = true;
			for (const auto& anotherTrack : Piano::tracks)
				if (Piano::indexes.at(anotherTrack) &&
					Piano::milliSeconds.at(anotherTrack).at(Piano::indexes.at(anotherTrack) - 1).second
						+ Piano::timerTick > prevTime)
				{
					track = anotherTrack;
					finish = false;
					break;
				}
		}
		auto prevIndexes(Piano::indexes);
		NextChord();
		Piano::indexes = prevIndexes;
	}
}
void Controls::OnHScroll(const HWND, const HWND hCtl, const UINT code, const int)
	// "int pos" parameter is 16 bit, therefore, 32 bit GetScrollInfo() is used instead
{
	switch (code)
	{
	case SB_LEFT:		UpdateScrollBar(INT_MIN);	if (!isPlaying_) NextChord();	break;
	case SB_RIGHT:		UpdateScrollBar(INT_MAX);	if (!isPlaying_) PrevChord();	break;

	case SB_LINELEFT:								if (!isPlaying_) PrevChord();	break;
	case SB_LINERIGHT:								if (!isPlaying_) NextChord();	break;

	case SB_PAGELEFT:	UpdateScrollBar(-10'000);	if (!isPlaying_) PrevChord();	break;
	case SB_PAGERIGHT:	UpdateScrollBar(10'000);	if (!isPlaying_) NextChord();	break;

	case SB_THUMBTRACK: case SB_THUMBPOSITION:
	{
		SCROLLINFO scrollInfo{ sizeof scrollInfo, SIF_POS | SIF_TRACKPOS };
		GetScrollInfo(hCtl, SB_CTL, &scrollInfo);
		UpdateScrollBar(scrollInfo.nTrackPos - scrollInfo.nPos);
		if (scrollInfo.nTrackPos) NextChord();
	}
	}
}

void Controls::OnCommand(const HWND hDlg, const int id, const HWND hCtrl, const UINT notifyCode)
{
	switch (id)
	{
	case IDB_PLAY:
	{
		if (isPlaying_)
		{
			KillTimer(MainWindow::hWndMain, 0);
			start_ = 0;
			Button_SetText(hCtrl, TEXT("Play"));
			ComboBox_Enable(leftHand, true);
			ComboBox_Enable(rightHand, true);
			isPlaying_ = false;
		}
		else
		{
			if (Piano::tracks.empty())
				MessageBox(hDlg, TEXT("No tracks are chosen, nothing to play yet"),
					TEXT("Choose tracks"), MB_ICONASTERISK);
			else
			{
				start_ = GetTickCount() - ScrollBar_GetPos(scrollBar);
				SetTimer(MainWindow::hWndMain, 0, Piano::timerTick, OnTimer);
				Button_SetText(hCtrl, TEXT("Pause"));
				ComboBox_Enable(leftHand, false);
				ComboBox_Enable(rightHand, false);
				isPlaying_ = true;
			}
		}
	}
	break;

	case IDC_LEFT_HAND: case IDC_RIGHT_HAND:
		if (notifyCode == CBN_SELCHANGE)
		{
			const auto progressBar(hCtrl == leftHand ? progressLeft : progressRight);
			const auto listIndex(ComboBox_GetCurSel(hCtrl));
			const auto trackNo(ComboBox_GetItemData(hCtrl, listIndex));
			if (trackNo > -1)
			{
				const auto track(static_cast<size_t>(trackNo));
				if (hCtrl == leftHand ? Piano::fingersLeft.at(track).empty()
					: Piano::fingersRight.at(track).empty())
				try
				{
					TrellisGraph graph(Piano::notes.at(track), hCtrl == leftHand);
					Cursor cursorWait;
					auto toFinish(true);
					auto timeStart(GetTickCount());
					for (size_t i(1); i; i = graph.NextStep())
					{
						SendMessage(progressBar, PBM_SETPOS,
							i * 95 / Piano::notes.at(track).size(), 0);
						if (static_cast<int>(GetTickCount()) - static_cast<int>(timeStart) > 10'000)
							if (MessageBox(hDlg,
									TEXT("It seems that fingering calculation might take a while.\n")
									TEXT("Press OK if you want to continue waiting."),
									TEXT("Lots of fingering combinations"),
								MB_ICONQUESTION | MB_OKCANCEL | MB_DEFBUTTON2) == IDCANCEL)
							{
								ComboBox_SetCurSel(hCtrl, 0);
								SendMessage(progressBar, PBM_SETPOS, 0, 0);
								toFinish = false;
								break;
							}
							else timeStart = USER_TIMER_MAXIMUM;
					}
					if (toFinish)
					{
						graph.Finish();
						if (hCtrl == leftHand)
						{
							Piano::fingersLeft.at(track) = graph.GetResult();
							Piano::leftTrack = make_unique<size_t>(track);
						}
						else
						{
							Piano::fingersRight.at(track) = graph.GetResult();
							Piano::rightTrack = make_unique<size_t>(track);
						}
						SendMessage(progressBar, PBM_SETPOS, 100, 0);
					}
				}
				catch (const bad_alloc& e)
				{
					const wstring trackName(static_cast<size_t>(
						ComboBox_GetLBTextLen(hCtrl, listIndex)), '\0');
					ComboBox_GetLBText(hCtrl, listIndex, trackName.data());
					if (MessageBoxA(hCtrl, ("Cannot finish fingering calculation: insufficient memory.\n"
								"Probably, track \"" + string(trackName.cbegin(), trackName.cend()) +
								"\" consists of too many notes.\n"
								"The program may behave inadequately"
								" and bullshit may be played until you restart it."
								" Do you want to close the program now?"
							).c_str(), e.what(), MB_ICONHAND | MB_YESNO
						) == IDYES) FORWARD_WM_DESTROY(MainWindow::hWndMain, SendMessage);

					ComboBox_SetCurSel(hCtrl, 0);
					SendMessage(progressBar, PBM_SETPOS, 0, 0);
				}
				else
				{
					if (hCtrl == leftHand)	Piano::leftTrack = make_unique<size_t>(track);
					else					Piano::rightTrack = make_unique<size_t>(track);
					SendMessage(progressBar, PBM_SETPOS, 100, 0);
				}
			}
			else
			{
				if (hCtrl == leftHand)	Piano::leftTrack = nullptr;
				else					Piano::rightTrack = nullptr;
				SendMessage(progressBar, PBM_SETPOS, 0, 0);
			}
		}
//		no break;
	case IDC_TRACKS:
		if (notifyCode == LBN_SELCHANGE)
		{
			vector<int> items(static_cast<size_t>(ListBox_GetSelCount(trackList)), 0);
			ListBox_GetSelItems(trackList, items.size(), items.data());
			Piano::tracks.clear();
			Piano::tracks.reserve(items.size());
			for (const auto& item : items)
				Piano::tracks.push_back(static_cast<size_t>(ListBox_GetItemData(trackList, item)));

			if (Piano::leftTrack
				&& find(Piano::tracks.cbegin(), Piano::tracks.cend(), *Piano::leftTrack)
					== Piano::tracks.cend()) Piano::tracks.push_back(*Piano::leftTrack);
			if (Piano::rightTrack
				&& find(Piano::tracks.cbegin(), Piano::tracks.cend(), *Piano::rightTrack)
					== Piano::tracks.cend()) Piano::tracks.push_back(*Piano::rightTrack);

			RewindTracks(ScrollBar_GetPos(scrollBar));
		}
	}
}

INT_PTR CALLBACK Controls::Main(const HWND hDlg, const UINT message,
	const WPARAM wParam, const LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hDlg, WM_INITDIALOG,	OnInitDialog);
		HANDLE_MSG(hDlg, WM_HSCROLL,	OnHScroll);
		HANDLE_MSG(hDlg, WM_COMMAND,	OnCommand);
	default: return false;
	}
}