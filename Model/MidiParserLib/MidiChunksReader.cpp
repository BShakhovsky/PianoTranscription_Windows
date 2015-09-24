# include "stdafx.h"
# include "MidiChunksReader.h"
# include "MidiParser.h"
# include "IFileParser.h"
# include "MidiStruct.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
	MidiChunksReader::MidiChunksReader(const char* fileName) :			// for use in production
		pImpl_(make_unique<MidiParser>(fileName))
	{}
	MidiChunksReader::MidiChunksReader(unique_ptr<IMidiParser> mock) :	// for unit tests
		pImpl_(mock.release())
	{}
	MidiChunksReader::~MidiChunksReader() {}
# pragma warning(pop)

void CheckHeaderIntro(const ChunkIntro intro)
{
	if (intro.type != ChunkIntro::HEADER)
		throw runtime_error("CORRUPTED MIDI FILE HEADER");
	if (intro.length != (sizeof HeaderData::format + sizeof HeaderData::tracks + sizeof HeaderData::division))
		throw length_error("CORRUPTED MIDI FILE HEADER LENGTH");
}

uint32_t MidiChunksReader::SMPTE_TicksPerSec(const uint32_t division, const bool toPrint)
{
	const auto
		ticks(division & 0x0'00'FF),
		frames((division & 0x0'FF'00 >> Bytes::byteSize) * (-1)),
		result(ticks * frames);

	if (toPrint)
	{
		switch (frames)
		{
		case 24: case 25: case 29: case 30: break;
		default: WARNING("Wrong frames per second SMPTE time");
		}
		cout << "Time-code-based time: ticks per frame = " << ticks << ", frames per second SMPTE time = " << frames
			<< "\n\tTherefore, ticks per second = " << ticks << " * " << frames << " = " << result;
	}
	return result;
}

void PrintHeaderData(const HeaderData data)
{
	switch (data.format)
	{
	case 0: if (1 != data.tracks) throw logic_error("CORRUPTED MIDI FILE TRACK NUMBERS");
			cout << "There is a single track" << endl;												break;
	case 1: cout << "There are " << data.tracks << " tracks to be played simultaneously" << endl;	break;
	case 2: cout << "There are " << data.tracks << " independant tracks" << endl;					break;
	default: throw runtime_error("CORRUPTED MIDI FILE FORMAT");
	}

	if (!data.division) throw logic_error("TIME DIVISION IS ZERO, NOT REALLY SURE WHAT IT MEANS");
	else if (data.division & 0x80'00) MidiChunksReader::SMPTE_TicksPerSec(data.division, true);
	else cout << "Metrical time: Pulses per Quarter Note = " << data.division << " (pulses are MIDI imaginary time units)";
}

const HeaderChunk MidiChunksReader::ReadHeaderChunk() const
{
	HeaderChunk result;
	result.intro = pImpl_->ReadChunkIntro();	CheckHeaderIntro(result.intro);
	result.data = pImpl_->ReadHeaderData();		PrintHeaderData(result.data);
	return result;
}


const TrackChunk MidiChunksReader::ReadTrackChunk() const
{
	TrackChunk result;
							result.intro =		pImpl_->ReadChunkIntro();
	if (ChunkIntro::TRACK != result.intro.type)	pImpl_->SkipTrackEvents(result.intro.length);
	else					result.trackEvent =	pImpl_->ReadTrackEvents(result.intro.length);

	return result;
}