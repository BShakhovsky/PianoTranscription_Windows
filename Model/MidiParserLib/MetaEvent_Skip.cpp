# include "stdafx.h"
# include "MetaEvent_Skip.h"
# include "IFileParser.h"
# include "MidiStruct.h"

using Model::MidiParser::MetaEvent_Skip;

# define PRINT_AND_SKIP(BYTE_VAL, MESSG)	{	if ((BYTE_VAL) != GetInputFile()->PeekByte())			\
													WARNING("Wrong " << (MESSG) << " chunk length");	\
												SkipEvent();	/* Skip anyway */						}
# pragma warning(push)
#	ifdef _DEBUG
#		pragma warning(disable:4702)	// unreachable code (but may become reachable by mistake ==> test it)
#	endif
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
# pragma warning(pop)