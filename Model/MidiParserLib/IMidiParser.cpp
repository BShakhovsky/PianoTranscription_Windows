# include "stdafx.h"
# include "IMidiParser.h"
# include "MidiStruct.h"

using std::vector;
using namespace Model::MidiParser;
using namespace MidiStruct;

const ChunkIntro IMidiParser::ReadChunkIntro() const
{
	return ReadChunkIntro_impl();
}

const HeaderData IMidiParser::ReadHeaderData() const
{
	return ReadHeaderData_impl();
}

vector<TrackEvent> IMidiParser::ReadTrackEvents(const uint32_t length) const
{
	return ReadTrackEvents_impl(length);
}


const ChunkIntro IMidiParser::ReadChunkIntro_impl() const
{
	PURE_VIRTUAL;
	return ChunkIntro();
}

const HeaderData IMidiParser::ReadHeaderData_impl() const
{
	PURE_VIRTUAL;
	return HeaderData();
}

vector<TrackEvent> IMidiParser::ReadTrackEvents_impl(const uint32_t) const
{
	PURE_VIRTUAL;
	return vector<TrackEvent>();
}