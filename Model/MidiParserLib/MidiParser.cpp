# include "stdafx.h"
# include "MidiParser.h"

# include "FileParser.h"
# include "MidiStruct.h"
# include "NoteNames.h"

# include "Event.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

MidiParser::MidiParser(const char *fileName) :
	fileParser_(make_shared<FileParser>(fileName))
{}

shared_ptr<EventChunk> MidiParser::ReadEvent() const
{
	return Event::GetInstance(fileParser_).Read();
}

const ChunkIntro MidiParser::ReadChunkIntro() const
{
	ChunkIntro result;
	result.type = ReadChunkType();
	result.length = ReadWord(GetInputFile());
	return result;
}

const ChunkType MidiParser::ReadChunkType() const
{
	ChunkType result;
	GetInputFile()->ReadData(result.type, sizeof ChunkType::type);
	return result;
}