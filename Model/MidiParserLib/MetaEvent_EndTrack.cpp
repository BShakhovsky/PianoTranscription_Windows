# include "stdafx.h"
# include "MetaEvent_EndTrack.h"
# include "FileParser.h"

using boost::serialization::singleton;
using Model::MidiParser::MetaEvent_EndTrack;

MetaEvent_EndTrack& MetaEvent_EndTrack::GetInstance()
{
	return singleton<MetaEvent_EndTrack>::get_mutable_instance();
}

void MetaEvent_EndTrack::Read_impl()
{
	assert("SOMETHING IS WRONG IN PROGRAM ARITHMETICS" && 1 == GetInputFile()->GetBytesRemained());
	if (GetInputFile()->ReadByte()) FAIL() << "Corrupted MIDI's end of track";
}