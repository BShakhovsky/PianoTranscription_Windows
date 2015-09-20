# include "stdafx.h"
# include "MidiParser.h"

# include "FileParser.h"
# include "MidiStruct.h"
# include "NoteNames.h"

# include "Event.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

MidiParser::MidiParser(const char* fileName) :
	IMidiParser(),
	inputFile_(make_unique<FileParser>(fileName))
{}


const ChunkType MidiParser::ReadChunkType() const
{
	ChunkType result;
	inputFile_->ReadData(result.type, sizeof ChunkType::type);
	return result;
}


const ChunkIntro MidiParser::ReadChunkIntro_impl() const
{
	ChunkIntro result;
	result.type = ReadChunkType();
	result.length = ReadWord(inputFile_);
	return result;
}

const HeaderData MidiParser::ReadHeaderData_impl() const
{
	HeaderData result;

	result.format = ReadDWord(inputFile_);
	result.tracks = ReadDWord(inputFile_);
	result.division = ReadDWord(inputFile_);

	return result;
}


void MidiParser::SkipTrackEvents_impl(const uint32_t length) const
{
	inputFile_->SetBytesRemained(static_cast<int>(length));
	inputFile_->SkipData(length);
	WARNING("Corrupted MIDI Track Header, " << length << " bytes skipped");
}

vector<TrackEvent> MidiParser::ReadTrackEvents_impl(const uint32_t length) const
{
	vector<TrackEvent> result;

	inputFile_->SetBytesRemained(static_cast<int>(length));
	while (inputFile_->GetBytesRemained() > 0)
	{
		result.emplace_back();
		result.back().deltaTime = inputFile_->ReadVarLenFormat();	// may throw std::length_error
		result.back().eventChunk = *Event::GetInstance(inputFile_)->Read();
	}

	return result;
}