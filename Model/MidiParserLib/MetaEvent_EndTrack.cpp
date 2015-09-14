# include "stdafx.h"
# include "MetaEvent_EndTrack.h"
# include "FileParser.h"

# include "..\..\Executable\gTests\stdafx.h"

using std::cout;
using boost::serialization::singleton;
using Model::MidiParser::MetaEvent_EndTrack;

MetaEvent_EndTrack& MetaEvent_EndTrack::GetInstance()
{
	return singleton<MetaEvent_EndTrack>::get_mutable_instance();
}

# ifndef FAIL
#	define FAIL() cout
# endif
void MetaEvent_EndTrack::Read_impl()
{
	assert("SOMETHING IS WRONG IN PROGRAM ARITHMETICS" && 1 == GetInputFile()->GetBytesRemained());
	if (GetInputFile()->ReadByte()) FAIL() << "WARNING: Corrupted MIDI's end of track";
}