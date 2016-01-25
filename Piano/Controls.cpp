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

void Controls::OnSoundError(const SoundError& e)
{
	if (isPlaying_) FORWARD_WM_COMMAND(hDlgControls, IDB_PLAY, playButton, 0, SendMessage);
	MessageBox(hDlgControls, lexical_cast<String>(e.what()).c_str(),
		TEXT("Error"), MB_ICONERROR | MB_OK);
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
			OnSoundError(e);
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
void AssignFingers(const vector<vector<pair<int16_t, string>>>& fingers,
	const size_t trackNo, size_t* index, size_t* indexTotal)
{
	for (; *indexTotal < Piano::indexes.at(trackNo);
			*indexTotal += fingers.at((*index)++).size())
		for (const auto& note : fingers.at(*index))
			Piano::keyboard->AssignFinger(note.first, note.second);
	assert("Chord played is inconsistant with chord in fingering analysis"
		&& *indexTotal == Piano::indexes.at(trackNo));
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
		if (Piano::leftTrack) AssignFingers(fingersLeft_,
			*Piano::leftTrack, &Piano::hands.leftIndex, &Piano::hands.leftIndexTotal);
		if (Piano::rightTrack) AssignFingers(fingersRight_,
			*Piano::rightTrack, &Piano::hands.rightIndex, &Piano::hands.rightIndexTotal);
		InvalidateRect(hWnd, nullptr, false);
		try
		{
			Piano::sound->Play();
		}
		catch (const SoundError& e)
		{
			OnSoundError(e);
		}
		result = true;
	}

	if (isPlaying_ && all_of(Piano::tracks.cbegin(), Piano::tracks.cend(), [](size_t track)
		{
			return Piano::indexes.at(track) >= Piano::midi->GetNotes().at(track).size();
		}))
	{
		FORWARD_WM_COMMAND(hDlgControls, IDB_PLAY, playButton, 0, SendMessage);
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
	for (const auto& track : Piano::tracks)
	{
		Piano::indexes.at(track) = static_cast<size_t>(
			lower_bound(Piano::midi->GetMilliSeconds().at(track).cbegin(),
				Piano::midi->GetMilliSeconds().at(track).cend(), static_cast<unsigned>(pos))
			- Piano::midi->GetMilliSeconds().at(track).cbegin());
		if (Piano::leftTrack && *Piano::leftTrack == track)
		{
			for (; Piano::hands.leftIndexTotal < Piano::indexes.at(track);
				Piano::hands.leftIndexTotal += fingersLeft_.at(Piano::hands.leftIndex++).size());
			for (; Piano::hands.leftIndexTotal > Piano::indexes.at(track);
				Piano::hands.leftIndexTotal -= fingersLeft_.at(--Piano::hands.leftIndex).size());
			assert("Chord played is inconsistant with chord in fingering analysis"
				&& Piano::hands.leftIndexTotal == Piano::indexes.at(track));
		}
		if (Piano::rightTrack && *Piano::rightTrack == track)
		{
			for (; Piano::hands.rightIndexTotal < Piano::indexes.at(track);
			Piano::hands.rightIndexTotal += fingersRight_.at(Piano::hands.rightIndex++).size());
				for (; Piano::hands.rightIndexTotal > Piano::indexes.at(track);
			Piano::hands.rightIndexTotal -= fingersRight_.at(--Piano::hands.rightIndex).size());
				assert("Chord played is inconsistant with chord in fingering analysis"
					&& Piano::hands.rightIndexTotal == Piano::indexes.at(track));
		}
	}
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
	while (!isPlaying_ && !Piano::tracks.empty())
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
		if (Piano::indexes.at(track) < Piano::midi->GetMilliSeconds().at(track).size()
				&& OnTimer(GetParent(hDlgControls), Piano::midi->GetMilliSeconds()
					.at(track).at(Piano::indexes.at(track)) + timerTick_))
			break;
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
			SetTimer(GetParent(hDlg), 0, timerTick_, OnTimer);
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
				vector<vector<int16_t>> chords({ { Piano::midi->GetNotes().at(*track).front() } });
				auto lastTime(static_cast<int>(Piano::midi->GetMilliSeconds().at(*track).front()));
				for (auto note(Piano::midi->GetNotes().at(*track).cbegin() + 1);
				note != Piano::midi->GetNotes().at(*track).cend(); ++note)
				{
					const auto newTime(static_cast<int>(Piano::midi->GetMilliSeconds().at(*track).at(
						static_cast<size_t>(note - Piano::midi->GetNotes().at(*track).cbegin()))));
					if (newTime - lastTime < timerTick_)
						chords.back().push_back(*note);
					else
						chords.push_back({ *note });
					lastTime = newTime;
				}
				TrellisGraph graph(chords, hCtrl == leftHand);
				Cursor cursorWait;
				for (size_t i(1); i; i = graph.NextStep())
					SendMessage(progressBar, PBM_SETPOS, i * 95 / chords.size(), 0);
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

			if (Piano::leftTrack && find(Piano::tracks.cbegin(), Piano::tracks.cend(), *Piano::leftTrack)
				== Piano::tracks.cend()) Piano::tracks.push_back(*Piano::leftTrack);
			if (Piano::rightTrack && find(Piano::tracks.cbegin(), Piano::tracks.cend(), *Piano::rightTrack)
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