# include "stdafx.h"
# include "MidiChunksReader.h"
# include "MidiParser.h"

# include "FileParser.h"
# include "MidiStruct.h"
# include "MetaEvent.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
MidiChunksReader::MidiChunksReader(const char *fileName) :
	header_(make_shared<HeaderChunk>()),
	tracks_(),
	pImpl_(make_unique<MidiParser>(fileName))
{}
MidiChunksReader::~MidiChunksReader() {}
# pragma warning(pop)

void MidiChunksReader::ReadHeaderChunk()
{
	header_->intro = pImpl_->ReadChunkIntro();
	if (header_->intro.type != ChunkIntro::HEADER)
		throw runtime_error("CORRUPTED MIDI FILE HEADER");
	if (header_->intro.length > sizeof HeaderData)
		throw length_error("CORRUPTED MIDI FILE HEADER LENGTH");
	
	header_->data.format = ReadDWord(pImpl_->GetInputFile());
	tracks_.reserve(header_->data.tracks = ReadDWord(pImpl_->GetInputFile()));

	header_->data.division = ReadDWord(pImpl_->GetInputFile());
	assert(R"(DELTA TIMES ARE IN "SMPTE" COMPATIBLE UNITS)"
		&& R"(ONLY "PPQN" CASE IS CURRENTLY CONSIDERED IN THIS SOFTWARE)"
		&& header_->data.division > 0);

	switch (header_->data.format)
	{
	case 0: if (1 != header_->data.tracks) throw logic_error("CORRUPTED MIDI FILE TRACK NUMBERS");
		cout << "There is a single track" << endl;				break;
	case 1: cout << "There are " << header_->data.tracks
		<< " tracks to be played simultaneously" << endl;		break;
	case 2: cout << "There are " << header_->data.tracks
		<< " independant tracks" << endl;						break;
	default: throw runtime_error("CORRUPTED MIDI FILE FORMAT");	break;
	}
	cout << "PPQN = " << header_->data.division << '\n' << endl;
}

void MidiChunksReader::ReadTrackChunks()
{
	for (auto i(0); i < header_->data.tracks; ++i)
	{
		tracks_.emplace_back();
		tracks_.back().intro = pImpl_->ReadChunkIntro();
		if (tracks_.back().intro.type != ChunkIntro::TRACK)
		{
			pImpl_->GetInputFile()->SkipData(tracks_.back().intro.length);
			cout << "\nWARNING: Corrupted MIDI Track Header, " << tracks_.back().intro.length << "bytes skipped" << endl;
		}
		else
		{
			pImpl_->GetInputFile()->SetBytesRemained(static_cast<int>(tracks_.back().intro.length));
			while (pImpl_->GetInputFile()->GetBytesRemained() > 0)
			{
				tracks_.back().trackEvent.emplace_back();
				tracks_.back().trackEvent.back().deltaTime = pImpl_->GetInputFile()->ReadVarLenFormat();
					// may throw std::length_error
				tracks_.back().trackEvent.back().eventChunk = *pImpl_->ReadEvent();
			}
		}
	}
	cout << "\nAll tracks have been read successfully" << endl;
}