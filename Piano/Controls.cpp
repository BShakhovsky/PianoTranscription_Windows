#include "stdafx.h"
#include "Controls.h"
#include "MainWindow.h"
#include "Piano.h"
#include "Cursor.h"

#include "MidiParser\MidiParser_Facade.h"
#include "PianoFingering\TrellisGraph_Facade.h"
#include "Keyboard.h"
#include "PianoSound\Sound_Facade.h"
#include "PianoSound\SoundError.h"

using namespace std;
using boost::lexical_cast;

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

vector<vector<pair<int16_t, string>>> Controls::fingersLeft_ = vector<vector<pair<int16_t, string>>>();
vector<vector<pair<int16_t, string>>> Controls::fingersRight_ = vector<vector<pair<int16_t, string>>>();

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
void Controls::OnDestroy(const HWND)
{
	fingersLeft_.clear();
	fingersRight_.clear();
}

void Controls::OnSoundError(const string& err)
{
	Piano::sound->Mute();
	StopPlaying();
	MessageBox(hDlgControls, (lexical_cast<String>(err.c_str())
			+ TEXT("\nSound will be mute until you restart the program")).c_str(),
		TEXT("Error"), MB_ICONERROR | MB_OK);
}
void Controls::StopPlaying()
{
	if (isPlaying_) FORWARD_WM_COMMAND(hDlgControls, IDB_PLAY, playButton, 0, SendMessage);
}
void Controls::UpdateTime(const DWORD dwTime)
{
	if (dwTime < start_) start_ = dwTime;
	const auto currTime(dwTime - start_);
	ScrollBar_SetPos(scrollBar, static_cast<int>(currTime), true);

	const auto seconds(currTime / 1'000), milliSec(currTime % 1'000);
	Edit_SetText(time_, (Format{ TEXT("Time %u:%02u:%02u") } %
		(seconds / 60) % (seconds % 60) % (milliSec / 10)).str().c_str());
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
			try
			{
				Piano::sound->AddNote(note);
			}
			catch (const SoundError& e)
			{
				OnSoundError(string(e.what()) + "\nFailed to add note");
			}
		}
		result = 1;
	}
	if (Piano::indexes.at(trackNo) < Piano::notes.at(trackNo).size() && (result &&
		static_cast<time_t>(Piano::milliSeconds.at(trackNo).at(Piano::indexes.at(trackNo)).first)
		- static_cast<time_t>(Piano::milliSeconds.at(trackNo).at(Piano::indexes.at(trackNo)
			- 1).second) <= Piano::timerTick
		|| static_cast<time_t>(dwTime + Piano::timerTick) - static_cast<time_t>(start_
			+ Piano::milliSeconds.at(trackNo).at(Piano::indexes.at(trackNo)).first) >= 0))
		result = INT16_MIN;

	return result;
}
bool Controls::OnTimer(const HWND hWnd, const DWORD dwTime)
{
	auto result(false);

	UpdateTime(dwTime);
	if (accumulate(Piano::tracks.cbegin(), Piano::tracks.cend(), 0,
		[dwTime](int val, size_t track)
		{
			return val + PlayTrack(track, dwTime);
		}
		) > 0)
	{
		if (Piano::leftTrack)
		{
			const auto leftIndex(Piano::indexes.at(*Piano::leftTrack));
			if (leftIndex) for (const auto& note : fingersLeft_.at(leftIndex - 1))
				Piano::keyboard->AssignFinger(note.first, note.second, true);
		}
		if (Piano::rightTrack)
		{
			const auto rightIndex(Piano::indexes.at(*Piano::rightTrack));
			if (rightIndex) for (const auto& note : fingersRight_.at(rightIndex - 1))
				Piano::keyboard->AssignFinger(note.first, note.second);
		}

		InvalidateRect(hWnd, nullptr, false);
		try
		{
			Piano::sound->Play();
		}
		catch (const SoundError& e)
		{
			OnSoundError(string(e.what()) + "\nFailed to play chords");
		}
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
}
void Controls::NextChord()
{
	while (!Piano::tracks.empty())
	{
		const auto track(*min_element(Piano::tracks.cbegin(), Piano::tracks.cend(),
			[](size_t left, size_t right)
			{
				return Piano::indexes.at(left) >= Piano::milliSeconds.at(left).size()
					? false
					: Piano::indexes.at(right) >= Piano::milliSeconds.at(right).size()
					? true
					: Piano::milliSeconds.at(left).at(Piano::indexes.at(left)).first
						< Piano::milliSeconds.at(right).at(Piano::indexes.at(right)).first;
			}));
		if (Piano::indexes.at(track) >= Piano::milliSeconds.at(track).size()
				|| OnTimer(GetParent(hDlgControls), start_ + Piano::milliSeconds
					.at(track).at(Piano::indexes.at(track)).second + Piano::timerTick))
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
void Controls::OnHScroll(const HWND hDlg, const HWND hCtl, const UINT code, const int)
	// "int pos" parameter is 16 bit, therefore, 32 bit GetScrollInfo() is used instead
{
	switch (code)
	{
	case SB_LEFT:		UpdateScrollBar(INT_MIN);		break;
	case SB_RIGHT:		UpdateScrollBar(INT_MAX);		break;

	case SB_LINELEFT:	if (!isPlaying_) PrevChord();	break;
	case SB_LINERIGHT:	if (!isPlaying_) NextChord();	break;

	case SB_PAGELEFT:	UpdateScrollBar(-10'000);		break;
	case SB_PAGERIGHT:	UpdateScrollBar(10'000);		break;

	case SB_THUMBTRACK: case SB_THUMBPOSITION:
	{
		Piano::sound->PressSustain(false);
		SCROLLINFO scrollInfo{ sizeof scrollInfo, SIF_POS | SIF_TRACKPOS };
		GetScrollInfo(hCtl, SB_CTL, &scrollInfo);
		UpdateScrollBar(scrollInfo.nTrackPos - scrollInfo.nPos);
		if (scrollInfo.nTrackPos) NextChord();
		else UpdateTime(start_);
	}													break;
	case SB_ENDSCROLL: Piano::sound->PressSustain(IsDlgButtonChecked(
		hDlg, IDC_PEDAL) == BST_CHECKED);				break;

	default: assert(!"Unhandled scroll bar message");
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
			KillTimer(GetParent(hDlg), 0);
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
			start_ = GetTickCount() - ScrollBar_GetPos(scrollBar);
			SetTimer(GetParent(hDlg), 0, Piano::timerTick, OnTimer);
			Button_SetText(hCtrl, TEXT("Pause"));
			ComboBox_Enable(leftHand, false);
			ComboBox_Enable(rightHand, false);
			isPlaying_ = true;
		}
	}
	break;

	case IDC_LEFT_HAND: case IDC_RIGHT_HAND:
		if (notifyCode == CBN_SELCHANGE)
		{
			const auto trackNo(ComboBox_GetItemData(hCtrl, ComboBox_GetCurSel(hCtrl)));
			if (hCtrl == leftHand)	Piano::leftTrack = trackNo > -1
				? make_unique<size_t>(static_cast<size_t>(trackNo)) : nullptr;
			else					Piano::rightTrack = trackNo > -1
				? make_unique<size_t>(static_cast<size_t>(trackNo)) : nullptr;

			const auto progressBar(hCtrl == leftHand ? progressLeft : progressRight);
			const auto track(hCtrl == leftHand ? Piano::leftTrack.get() : Piano::rightTrack.get());
			if (track)
			{
				TrellisGraph graph(Piano::notes.at(*track), hCtrl == leftHand);
				Cursor cursorWait;
				for (size_t i(1); i; i = graph.NextStep())
					SendMessage(progressBar, PBM_SETPOS, i * 95 / Piano::notes.at(*track).size(), 0);
				graph.Finish();
				if (hCtrl == leftHand) fingersLeft_ = graph.GetResult();
				else fingersRight_ = graph.GetResult();
				SendMessage(progressBar, PBM_SETPOS, 100, 0);
			}
			else SendMessage(progressBar, PBM_SETPOS, 0, 0);
		}
//		break;
	case IDC_TRACKS:
		if (notifyCode == LBN_SELCHANGE)
		{
			vector<int> items(static_cast<size_t>(ListBox_GetSelCount(trackList)), 0);
			ListBox_GetSelItems(trackList, items.size(), items.data());
			Piano::tracks.clear();
			Piano::tracks.reserve(items.size());
			for (const auto& item : items)
				Piano::tracks.push_back(static_cast<size_t>(ListBox_GetItemData(trackList, item)));

			if (Piano::leftTrack.get()
				&& find(Piano::tracks.cbegin(), Piano::tracks.cend(), *Piano::leftTrack)
					== Piano::tracks.cend()) Piano::tracks.push_back(*Piano::leftTrack);
			if (Piano::rightTrack.get()
				&& find(Piano::tracks.cbegin(), Piano::tracks.cend(), *Piano::rightTrack)
					== Piano::tracks.cend()) Piano::tracks.push_back(*Piano::rightTrack);

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
		HANDLE_MSG(hDlg, WM_DESTROY,	OnDestroy);

		HANDLE_MSG(hDlg, WM_HSCROLL,	OnHScroll);
		HANDLE_MSG(hDlg, WM_COMMAND,	OnCommand);
	default: return false;
	}
}