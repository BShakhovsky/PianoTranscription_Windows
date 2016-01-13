#include "stdafx.h"
#include "Controls.h"
#include "MainWindow.h"
#include "Piano.h"

#include "MidiParser_Facade.h"
#include "Keyboard.h"
#include "Sound_Facade.h"
#include "SoundError.h"

using boost::lexical_cast;

HWND Controls::hDlgControls	= nullptr;

HWND Controls::midiLog		= nullptr;
HWND Controls::leftHand		= nullptr;
HWND Controls::rightHand	= nullptr;
HWND Controls::trackList	= nullptr;

HWND Controls::scrollBar	= nullptr;
HWND Controls::playButton	= nullptr;

HWND Controls::time_		= nullptr;


bool Controls::isPlaying_ = false;
DWORD Controls::start_ = 0;


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
	scrollBar	= GetDlgItem(hDlg, IDC_SCROLLBAR);
	playButton	= GetDlgItem(hDlg, IDB_PLAY);

	midiLog		= GetDlgItem(hDlg, IDC_MIDI_LOG);
	time_		= GetDlgItem(hDlg, IDC_TIME);
	
	leftHand	= GetDlgItem(hDlg, IDC_LEFT_HAND);
	rightHand	= GetDlgItem(hDlg, IDC_RIGHT_HAND);

	trackList	= GetDlgItem(hDlg, IDC_TRACKS);

	Edit_SetText(midiLog, TEXT("MIDI info and errors if any"));

	Edit_SetText(time_, TEXT("Time 0:00:00"));

	ComboBox_AddString(leftHand, TEXT("None"));
	ComboBox_AddString(rightHand, TEXT("None"));
	ComboBox_SetCurSel(leftHand, 0);
	ComboBox_SetCurSel(rightHand, 0);

	return true;
}

void Controls::UpdateTime(const DWORD dwTime)
{
	const auto currTime(dwTime - start_);
	ScrollBar_SetPos(scrollBar, static_cast<int>(currTime), true);

	const auto seconds(currTime / 1'000), milliSec(currTime % 1'000);
	Edit_SetText(time_, (Format{ TEXT("Time %u:%02u:%02u") } %
		(seconds / 60) % (seconds % 60) % (milliSec / 10)).str().c_str());
}
int Controls::PlayTrack(const size_t trackNo, const DWORD dwTime)
{
	auto result(0);

	for (; Piano::indexes.at(trackNo) < Piano::midi->GetNotes().at(trackNo).size()
		&& static_cast<time_t>(dwTime) - static_cast<time_t>(start_
			+ Piano::midi->GetMilliSeconds().at(trackNo).at(Piano::indexes.at(trackNo))) >= 0;
	++Piano::indexes.at(trackNo))
	{
		Piano::keyboard->PressKey(Piano::midi->GetNotes().at(trackNo).at(Piano::indexes.at(trackNo)));
		try
		{
			Piano::sound->AddNote(Piano::midi->GetNotes().at(trackNo).at(Piano::indexes.at(trackNo)));
		}
		catch (const SoundError& e)
		{
			MessageBox(MainWindow::hWndMain, lexical_cast<String>(e.what()).c_str(),
				TEXT("Error"), MB_ICONERROR | MB_OK);
		}
		result = 1;
	}
	if (Piano::indexes.at(trackNo) < Piano::midi->GetNotes().at(trackNo).size() && (result &&
		static_cast<time_t>(Piano::midi->GetMilliSeconds().at(trackNo).at(Piano::indexes.at(trackNo)))
		- static_cast<time_t>(Piano::midi->GetMilliSeconds().at(trackNo).at(Piano::indexes.at(trackNo)
			- 1)) <= timerTick_
		|| static_cast<time_t>(dwTime + timerTick_) - static_cast<time_t>(start_
			+ Piano::midi->GetMilliSeconds().at(trackNo).at(Piano::indexes.at(trackNo))) >= 0))
		result = INT16_MIN;

	return result;
}
void CALLBACK Controls::OnTimer(const HWND hWnd, UINT, UINT_PTR, const DWORD dwTime)
{
	UpdateTime(dwTime);
	if (accumulate(Piano::tracks.cbegin(), Piano::tracks.cend(), 0,
		[dwTime](int val, size_t track)
		{
			return val + PlayTrack(track, dwTime);
		}
			) > 0)
	{
		InvalidateRect(hWnd, nullptr, false);
		try
		{
			Piano::sound->Play();
		}
		catch (const SoundError& e)
		{
			MessageBox(MainWindow::hWndMain, lexical_cast<String>(e.what()).c_str(),
				TEXT("Error"), MB_ICONERROR | MB_OK);
		}
	}
	if (isPlaying_ && all_of(Piano::tracks.cbegin(), Piano::tracks.cend(),
		[](size_t track)
		{
			return Piano::indexes.at(track) >= Piano::midi->GetNotes().at(track).size();
		}))
	{
		FORWARD_WM_COMMAND(hDlgControls, IDB_PLAY, playButton, 0, SendMessage);
		ScrollBar_SetPos(scrollBar, 0, true);
		fill(Piano::indexes.begin(), Piano::indexes.end(), 0);
	}
}


void RewindTracks(int pos)
{
	for (const auto& track : Piano::tracks)
		Piano::indexes.at(track) = static_cast<size_t>(
			lower_bound(Piano::midi->GetMilliSeconds().at(track).cbegin(),
				Piano::midi->GetMilliSeconds().at(track).cend(), static_cast<unsigned>(pos))
			- Piano::midi->GetMilliSeconds().at(track).cbegin());
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
	if (!isPlaying_ && !Piano::tracks.empty())
	{
		const auto track(*min_element(Piano::tracks.cbegin(), Piano::tracks.cend(),
			[](size_t left, size_t right)
			{
				return Piano::indexes.at(left) >= Piano::midi->GetMilliSeconds().at(left).size()
					? false
					: Piano::indexes.at(right) >= Piano::midi->GetMilliSeconds().at(right).size()
					? true
					: Piano::midi->GetMilliSeconds().at(left).at(Piano::indexes.at(left))
						< Piano::midi->GetMilliSeconds().at(right).at(Piano::indexes.at(right));
			}));
		if (Piano::indexes.at(track) < Piano::midi->GetMilliSeconds().at(track).size())
			OnTimer(GetParent(hDlgControls), 0, 0,
				Piano::midi->GetMilliSeconds().at(track).at(Piano::indexes.at(track)) + timerTick_);
	}
}
void Controls::PrevChord()
{
	if (!isPlaying_ && !Piano::tracks.empty())
	{
		auto track(*max_element(Piano::tracks.cbegin(), Piano::tracks.cend(),
			[](size_t left, size_t right)
			{
				return !Piano::indexes.at(right) ? false : !Piano::indexes.at(left) ? true
					: Piano::midi->GetMilliSeconds().at(left).at(Piano::indexes.at(left) - 1)
						< Piano::midi->GetMilliSeconds().at(right).at(Piano::indexes.at(right) - 1);
			}));
		for (auto finish(false); !finish;)
		{
			if (Piano::indexes.at(track)) --Piano::indexes.at(track);
			auto prevTime(Piano::midi->GetMilliSeconds().at(track).at(Piano::indexes.at(track)));
			for (; Piano::indexes.at(track) && prevTime
					- Piano::midi->GetMilliSeconds().at(track).at(Piano::indexes.at(track) - 1)
					< timerTick_;
				prevTime = Piano::midi->GetMilliSeconds().at(track).at(--Piano::indexes.at(track)))
				;
			finish = true;
			for (const auto& anotherTrack : Piano::tracks)
				if (Piano::indexes.at(anotherTrack) &&
					Piano::midi->GetMilliSeconds().at(anotherTrack).at(Piano::indexes.at(anotherTrack) - 1)
						+ timerTick_ > prevTime)
				{
					track = anotherTrack;
					finish = false;
					break;
				}
		}
		auto prevIndexes(Piano::indexes);
		NextChord();
		Piano::indexes.swap(prevIndexes);
	}
}
void Controls::OnHScroll(const HWND, const HWND hCtl, const UINT code, int)
{
	switch (code)
	{
	case SB_LEFT:		UpdateScrollBar(INT_MIN);	break;
	case SB_RIGHT:		UpdateScrollBar(INT_MAX);	break;

	case SB_LINELEFT:	PrevChord();				break;
	case SB_LINERIGHT:	NextChord();				break;

	case SB_PAGELEFT:	UpdateScrollBar(-10'000);	break;
	case SB_PAGERIGHT:	UpdateScrollBar(10'000);	break;

	case SB_THUMBTRACK: case SB_THUMBPOSITION:
	{
		SCROLLINFO scrollInfo{ sizeof scrollInfo, SIF_POS | SIF_TRACKPOS };
		GetScrollInfo(hCtl, SB_CTL, &scrollInfo);
		UpdateScrollBar(scrollInfo.nTrackPos - scrollInfo.nPos);
		if (!isPlaying_ && scrollInfo.nTrackPos) NextChord();
	}												break;
	case SB_ENDSCROLL:								break;

	default: assert(!"Unhandled scroll bar message");
	}
}
void Controls::OnCommand(const HWND hDlg, const int id, const HWND hCtrl, const UINT notifyCode)
{
	using std::vector;

	switch (id)
	{
	case IDB_PLAY:
	{
		if (isPlaying_)
		{
			KillTimer(GetParent(hDlg), 0);
			start_ = 0;
			Button_SetText(hCtrl, TEXT("Play"));
			ComboBox_Enable(leftHand, true);
			ComboBox_Enable(rightHand, true);
			isPlaying_ = false;
		}
		else
		{
			start_ = GetTickCount() - ScrollBar_GetPos(scrollBar);
			SetTimer(GetParent(hDlg), 0, timerTick_, OnTimer);
			Button_SetText(hCtrl, TEXT("Pause"));
			ComboBox_Enable(leftHand, false);
			ComboBox_Enable(rightHand, false);
			isPlaying_ = true;
		}
	}
	break;
	case IDC_TRACKS:
		if (notifyCode == LBN_SELCHANGE)
		{
			vector<int> items(static_cast<size_t>(ListBox_GetSelCount(hCtrl)), 0);
			ListBox_GetSelItems(hCtrl, items.size(), items.data());
			Piano::tracks.clear();
			Piano::tracks.reserve(items.size());
			for (const auto& item : items)
				Piano::tracks.push_back(static_cast<size_t>(ListBox_GetItemData(hCtrl, item)));
			RewindTracks(ScrollBar_GetPos(scrollBar));
		}
		break;
	case IDC_PEDAL:
		Piano::sound->PressSustain(IsDlgButtonChecked(hDlg, IDC_PEDAL) == BST_CHECKED);
		break;
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