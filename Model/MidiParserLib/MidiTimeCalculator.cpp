# include "stdafx.h"
# include "MidiTimeCalculator.h"
# include "MidiTracksCollector.h"
# include "MidiChunksReader.h"
# include "MidiStruct.h"
# include "NoteNames.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

MidiTimeCalculator::MidiTimeCalculator() :
	tempoDivision_(NULL),
	tracks_(),

	totalMicroSeconds_(NULL),
	tempoSettings_(),

	itCurrentTrack_(),
	itCurrentEvent_(),
	itCurrentTempo_(tempoSettings_.cbegin())
{}

MidiTimeCalculator::~MidiTimeCalculator() {}

uint32_t RealMicrosec(uint32_t deltaTime, uint32_t tempoSetting, uint16_t division)
{
	assert("TIME DIVISION IS ZERO, NOT REALLY SURE WHAT IT MEANS" && division);

	if (division & 0x80'00)
		return deltaTime * static_cast<uint32_t>(TrackEvent::microSec)
			/ MidiChunksReader::SMPTE_TicksPerSec(division);
	else
		return deltaTime * tempoSetting / division;
}

void MidiTimeCalculator::LoadMidiData(const char* fileName)
{
	const auto midiData_(make_unique<MidiTracksCollector>(fileName));
	midiData_->ReadMidiFile();

	tempoDivision_ = midiData_->GetHeaderData()->division;
	tracks_ = midiData_->GetTracks();
	if (tracks_.empty()) throw logic_error("MIDI FILE DOES NOT CONTAIN ANY TRACKS");

	itCurrentTrack_ = tracks_.cbegin();
	itCurrentEvent_ = itCurrentTrack_->cbegin();
}

void MidiTimeCalculator::CollectTempos()
{
	do
	{
		if (tempoSettings_.empty()) assert("DELTA TIME STARTED BEFORE TEMPO IS SET" && !itCurrentEvent_->deltaTime);
		else totalMicroSeconds_ += RealMicrosec(itCurrentEvent_->deltaTime, itCurrentTempo_->second, tempoDivision_);

		if (-1 == itCurrentEvent_->eventChunk.status && 0x51 == itCurrentEvent_->eventChunk.metaType)	// -1 = 0xFF
		{
			PrintTime();
			cout << '\t' << (TrackEvent::microSec * TrackEvent::minute / itCurrentEvent_->eventChunk.metaData)
				<< " BPM" << endl;
			tempoSettings_.emplace_back(make_pair(totalMicroSeconds_, itCurrentEvent_->eventChunk.metaData));
			itCurrentTempo_ = tempoSettings_.cend() - 1;
		}
	} while (!EndOfTracks());
}

void MidiTimeCalculator::CalcDeltaTimes()
{
	assert("CollectTempos SHOULD BE CALLED BEFORE CalcDeltaTimes" && !tempoSettings_.empty());
	do
	{
		if (tempoSettings_.empty()) assert("DELTA TIME STARTED BEFORE TEMPO IS SET" && !itCurrentEvent_->deltaTime);
		else totalMicroSeconds_ += RealMicrosec(itCurrentEvent_->deltaTime, itCurrentTempo_->second, tempoDivision_);

		if (0x0'90 == (itCurrentEvent_->eventChunk.status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
			&& itCurrentEvent_->eventChunk.velocity)				// if velocity = 0 ==> "note-off" event
		{
			PrintTime();
			cout << '\t' << "Note "
				<< NoteNames::GetOctaveNumber(itCurrentEvent_->eventChunk.note)
				<< NoteNames::GetNoteName(itCurrentEvent_->eventChunk.note) << endl;
			if (itCurrentTempo_ != tempoSettings_.cend() - 1 && totalMicroSeconds_ >= (itCurrentTempo_ + 1)->first)
				++itCurrentTempo_;
		}
	} while (!EndOfTracks());
}

bool MidiTimeCalculator::EndOfTracks()
{
	auto result(false);
	if (++itCurrentEvent_ == itCurrentTrack_->cend())
	{
		cout << "\nEnd of track " << itCurrentTrack_ - tracks_.cbegin() + 1 << " of " << tracks_.size() << '\n' << endl;
		if (++itCurrentTrack_ == tracks_.cend())
		{
			cout << "End of all tracks" << endl;
			result = true;
			itCurrentTrack_ = tracks_.cbegin();
		}
		itCurrentEvent_ = itCurrentTrack_->cbegin();
		itCurrentTempo_ = tempoSettings_.cbegin();
		totalMicroSeconds_ = NULL;
		system("Pause");
	}
	return result;
}

void MidiTimeCalculator::PrintTime() const
{
	const auto
		totalSeconds(totalMicroSeconds_ / TrackEvent::microSec),
		milliSeconds(totalMicroSeconds_ % TrackEvent::microSec * 10 / TrackEvent::microSec),

		minutes(totalSeconds / TrackEvent::minute),
		seconds(totalSeconds % TrackEvent::minute);
	cout << "Time = " << minutes << ':' << setfill('0') << setw(2) << seconds << ':' << milliSeconds;
}