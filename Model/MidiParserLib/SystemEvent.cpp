# include "stdafx.h"
# include "SystemEvent.h"
# include "MidiStruct.h"
# include "IFileParser.h"

using Model::MidiParser::SystemEvent;

# define CHECK(BYTE_VAL, MESSG) if ((BYTE_VAL) != GetInputFile()->PeekByte()) WARNING(MESSG);

void SystemEvent::Read_impl()
{
	switch (GetChunk()->status)
	{
		// Exclusive events:
	case -16:/*F0*/ case -9: /*F7*/													break;
		// Common events:
	case -15:/*F1*/	CHECK(1, "Wrong MIDI time code");								break;
	case -14:/*F2*/	CHECK(2, "Wrong song position");								break;
	case -13:/*F3*/	CHECK(1, "Wrong song select");									break;
	case -10:/*F6*/																	break;
		// Real-time events:
	case -8: /*F8*/ case -6: /*FA*/ case -5: /*FB*/ case -4: /*FC*/ case -2:/*FE*/	break;
	default: assert(!"WRONG STATUS BYTE");
	}
	SkipEvent();
}