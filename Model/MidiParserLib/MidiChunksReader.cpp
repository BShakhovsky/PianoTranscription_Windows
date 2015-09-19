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
MidiChunksReader::MidiChunksReader(const char *fileName) :
	pImpl_(make_unique<MidiParser>(fileName))
{}
MidiChunksReader::~MidiChunksReader() {}
# pragma warning(pop)

const HeaderChunk MidiChunksReader::ReadHeaderChunk()
{
	HeaderChunk result;

	result.intro = pImpl_->ReadChunkIntro();
	if (result.intro.type != ChunkIntro::HEADER) throw runtime_error("CORRUPTED MIDI FILE HEADER");
	if (result.intro.length > sizeof HeaderData) throw length_error("CORRUPTED MIDI FILE HEADER LENGTH");

	result.data = pImpl_->ReadHeaderData();
	assert(R"(DELTA TIMES ARE IN "SMPTE" COMPATIBLE UNITS)"
		&& R"(ONLY "PPQN" CASE IS CURRENTLY CONSIDERED IN THIS SOFTWARE)" && result.data.division > 0);

	switch (result.data.format)
	{
	case 0: if (1 != result.data.tracks) throw logic_error("CORRUPTED MIDI FILE TRACK NUMBERS");
			cout << "There is a single track";														break;
	case 1: cout << "There are " << result.data.tracks << " tracks to be played simultaneously";	break;
	case 2: cout << "There are " << result.data.tracks << " independant tracks";					break;
	default: throw runtime_error("CORRUPTED MIDI FILE FORMAT");
	}
	cout << ", PPQN = " << result.data.division;

	return result;
}

const TrackChunk MidiChunksReader::ReadTrackChunk()
{
	TrackChunk result;
							result.intro =		pImpl_->ReadChunkIntro();
	if (ChunkIntro::TRACK != result.intro.type)	pImpl_->SkipTrackEvents(result.intro.length);
	else					result.trackEvent =	pImpl_->ReadTrackEvents(result.intro.length);

	return result;
}