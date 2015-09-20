# include "stdafx.h"
# include "MidiChunksReader.h"
# include "MidiParser.h"
# include "FileParser.h"
# include "MidiStruct.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
	MidiChunksReader::MidiChunksReader(const char *fileName) :				// for use in production
		pImpl_(make_unique<MidiParser>(fileName))
	{}
#	ifdef FRIEND_TEST
		MidiChunksReader::MidiChunksReader(unique_ptr<IMidiParser> mock) :	// for unit tests
			pImpl_(mock.release())
		{}
#	endif
MidiChunksReader::~MidiChunksReader() {}
# pragma warning(pop)

const HeaderChunk MidiChunksReader::ReadHeaderChunk() const
{
	HeaderChunk result;
	result.intro = pImpl_->ReadChunkIntro();	CheckHeaderIntro(result.intro);
	result.data = pImpl_->ReadHeaderData();		PrintHeaderData(result.data);
	return result;
}

void MidiChunksReader::CheckHeaderIntro(const ChunkIntro intro) const
{
	if (intro.type != ChunkIntro::HEADER)
		throw runtime_error("CORRUPTED MIDI FILE HEADER");
	if (intro.length != (sizeof HeaderData::format + sizeof HeaderData::tracks + sizeof HeaderData::division))
		throw length_error("CORRUPTED MIDI FILE HEADER LENGTH");
}

void MidiChunksReader::PrintHeaderData(const HeaderData data) const
{
	switch (data.format)
	{
	case 0: if (1 != data.tracks) throw logic_error("CORRUPTED MIDI FILE TRACK NUMBERS");
			cout << "There is a single track";												break;
	case 1: cout << "There are " << data.tracks << " tracks to be played simultaneously";	break;
	case 2: cout << "There are " << data.tracks << " independant tracks";					break;
	default: throw runtime_error("CORRUPTED MIDI FILE FORMAT");
	}

	assert(R"(DELTA TIMES ARE IN "SMPTE" COMPATIBLE UNITS)"
		&& R"(ONLY "PPQN" CASE IS CURRENTLY CONSIDERED IN THIS SOFTWARE)" && data.division > 0);
	cout << ", PPQN = " << data.division;
}


const TrackChunk MidiChunksReader::ReadTrackChunk() const
{
	TrackChunk result;
							result.intro =		pImpl_->ReadChunkIntro();
	if (ChunkIntro::TRACK != result.intro.type)	pImpl_->SkipTrackEvents(result.intro.length);
	else					result.trackEvent =	pImpl_->ReadTrackEvents(result.intro.length);

	return result;
}