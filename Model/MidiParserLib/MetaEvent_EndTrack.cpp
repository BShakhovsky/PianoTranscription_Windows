# include "stdafx.h"
# include "MetaEvent_EndTrack.h"
# include "FileParser.h"

META_IMPL(EndTrack)
{
	assert("SOMETHING IS WRONG IN PROGRAM ARITHMETICS" && 1 == GetInputFile()->GetBytesRemained());
	if (GetInputFile()->ReadByte()) ADD_FAILURE() << "Corrupted MIDI's end of track";
}