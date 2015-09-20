# include "stdafx.h"
# include "MetaEvent_EndTrack.h"
# include "FileParser.h"

using Model::MidiParser::MetaEvent_EndTrack;

void MetaEvent_EndTrack::Read_impl()
{
	assert("SOMETHING IS WRONG IN PROGRAM ARITHMETICS" && 1 == GetInputFile()->GetBytesRemained());
	if (GetInputFile()->ReadByte()) ADD_FAILURE() << "Corrupted MIDI's end of track";
}