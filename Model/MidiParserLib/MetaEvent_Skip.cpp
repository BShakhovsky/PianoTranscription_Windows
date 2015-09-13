# include "stdafx.h"
# include "MetaEvent_Skip.h"
# include "FileParser.h"
# include "MidiStruct.h"

using namespace std;
using boost::serialization::singleton;
using Model::MidiParser::MetaEvent_Skip;

MetaEvent_Skip& MetaEvent_Skip::GetInstance()
{
	return singleton<MetaEvent_Skip>::get_mutable_instance();
}

# define PRINT_AND_SKIP(BYTE, MESSG) {	if ((BYTE) != GetInputFile()->PeekByte())								\
											cout << "WARNING: Wrong " << (MESSG) << " chunk length" << endl;	\
										SkipEvent();	/* Skip anyway */										}
void MetaEvent_Skip::Read_impl()
{
	switch (GetChunk()->metaType)
	{
	case 0x20: PRINT_AND_SKIP(1, "MIDI channel prefix assignment");	break;
	case 0x21: PRINT_AND_SKIP(1, "MIDI port");						break;
	case 0x54: PRINT_AND_SKIP(5, "SMPTE offset");					break;
	case 0x58: PRINT_AND_SKIP(4, "time signature");					break;
	case 0x7F: SkipEvent();		/* Sequencer specific event */		break;
	default: assert(!"WRONG META TYPE");
	}
}