# include "stdafx.h"
/*
To finish:

MidiStruct::EventChunk --> possibly replace 6 fields with less
Replace std::shared_ptr<EventChunk> with EventChunk by value
Replace std::unique_ptr<TrackEvent> with TrackEvent by value

std::ifstream::good, fail, bad, rdstate, clear
Delta time is calculated for known events only

Structural: Composite, Decorator, Flyweight
Behaviourial: Command, Interpreter, Mediator, Memento, State, Strategy
*/
# include "..\MidiTracksCompiler.h"
# include "MidiChunksReader.h"
# include "MidiStruct.h"
# include "..\..\View\Keyboard.h"

using namespace std;
using boost::format;
using namespace Model::MidiParser;
using namespace MidiStruct;
using View::Keyboard;

MidiTracksCompiler::MidiTracksCompiler(const char *fileName) :
	header_(make_unique<HeaderChunk>()),
	tracks_(),

	ptrMidi_(new MidiChunksReader(fileName)),
	totalMicroSeconds_(NULL),
	tempoSettings_(),
	isPaused_(true),
	itCurrentTrack_(),
	itCurrentEvent_(),
	itCurrentTempo_(tempoSettings_.cbegin())
{
	switch (project_)
	{
	case 'W':
	{
		FreeConsole();
		AllocConsole();
		FILE* stream(nullptr);
		freopen_s(&stream, "CON", "w", stdout);
	}
	case 'C': break;
	default: assert(!"WRONG PROJECT TYPE");
	}

	*header_ = ptrMidi_->ReadHeaderChunk();	// may throw std::runtime_error, std::length_error and std::logic_error

	tracks_.reserve(header_->data.tracks);

	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);

	for (auto i(0); i < header_->data.tracks; ++i)
	{
		tracks_.emplace_back(ptrMidi_->ReadTrackChunk());	// may throw std::runtime_error
		cout << "\n\nEnd of track " << i + 1 << " of " << header_->data.tracks << endl;
		system("Pause");
	}
	cout << "\n\nAll tracks have been read successfully" << endl;

	for (itCurrentTrack_ = tracks_.cbegin();
	itCurrentTrack_ != tracks_.cend(); ++itCurrentTrack_)
		if (!itCurrentTrack_->trackEvent.empty())
		{
			itCurrentEvent_ = itCurrentTrack_->trackEvent.cbegin();
			break;
		}
	if (itCurrentTrack_->trackEvent.empty()) throw logic_error("MIDI FILE DOES NOT CONTAIN ANY TRACKS");
}

MidiTracksCompiler::~MidiTracksCompiler()
{
	delete ptrMidi_;

	cout << "YOU MAY CLOSE THIS CONSOLE WINDOW AT ANY TIME" << endl;
	switch (project_)
	{
	case 'W': FreeConsole();
	case 'C': break;
	default: assert(!"WRONG PROJECT TYPE");
	}
}

bool MidiTracksCompiler::LoadNextTrack()
{
	if (!isPaused_)
	{
		if (!tempoSettings_.empty()) totalMicroSeconds_ += itCurrentEvent_->deltaTime
			* itCurrentTempo_->second / header_->data.division;
		
		if (-1 == itCurrentEvent_->eventChunk.status &&	// 0xFF
			0x51 == itCurrentEvent_->eventChunk.metaType)
		{
			PrintTime();
			tempoSettings_.emplace_back(make_pair(totalMicroSeconds_, itCurrentEvent_->eventChunk.metaData));
			itCurrentTempo_ = tempoSettings_.cend() - 1;
		}
		else if (0x0'90 == (itCurrentEvent_->eventChunk.status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
			&& itCurrentEvent_->eventChunk.velocity)						// if velocity = 0 ==> "note-off" event
		{
			PrintTime(); cout << endl;
			if (itCurrentTempo_ != tempoSettings_.cend() - 1
				&& totalMicroSeconds_ >= (itCurrentTempo_ + 1)->first) ++itCurrentTempo_;
#ifdef _DEBUG
			isPaused_ = true;
#endif
		}
		if (++itCurrentEvent_ == itCurrentTrack_->trackEvent.cend())
		{
			cout << "\nEnd of track " << itCurrentTrack_ - tracks_.cbegin() + 1 << " of " <<
				header_->data.tracks << "\n\n==================================================\n" << endl;
			while (++itCurrentTrack_ != tracks_.cend())
				if (!itCurrentTrack_->trackEvent.empty())
				{
					itCurrentEvent_ = itCurrentTrack_->trackEvent.cbegin();
					itCurrentTempo_ = tempoSettings_.cbegin();
					totalMicroSeconds_ = NULL;
					break;
				}
			isPaused_ = true;
			if (itCurrentTrack_ == tracks_.cend())
			{
				cout << "\nEnd of MIDI file\n\n==================================================\n" << endl;
				return false;	// return without pause
			}
		}
	}
	return true;
}

void MidiTracksCompiler::PrintTime() const
{
	const auto totalSeconds(totalMicroSeconds_ / TrackEvent::microSec);
	cout << "Time = " << (totalSeconds / TrackEvent::minute) << ':'
		<< setfill('0') << setw(2) << (totalSeconds % TrackEvent::minute);
}

void MidiTracksCompiler::ShowKeyboard() const
{
	Keyboard::Init();
	Keyboard::Update();
}