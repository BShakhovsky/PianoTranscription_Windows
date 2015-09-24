# include "stdafx.h"
# include "MidiEvent.h"
# include "MidiStruct.h"
# include "IFileParser.h"

using namespace std;
using Model::MidiParser::MidiEvent;

char MidiEvent::runStatus_ = '\0';

void MidiEvent::CheckRunStatus() const
{
	if (GetChunk()->status < 0)				// most significant byte is set ==> it is status byte, Ok
	{
		runStatus_ = GetChunk()->status;	// save running status
		GetChunk()->note = GetInputFile()->ReadByte();
	}
	else if (runStatus_ >= 0) throw runtime_error("MIDI RUNNING STATUS IS NOT CORRECT");
	else
	{
		GetChunk()->note = GetChunk()->status;
		GetChunk()->status = runStatus_;	// release running status if correct
	}
}

void MidiEvent::Read_impl()
{
	CheckRunStatus();

	switch (GetChunk()->status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
	{
	case 0x080:	// Note off (key is released)
	case 0x090:	// Note on (but if velocity == 0, it is again note off = 0x080)

	case 0x0A0:	// Key polyphonic after-touch : Note number, Velocity
	case 0x0B0:	// Control change : Controller number, New value
		
		GetChunk()->velocity = GetInputFile()->ReadByte(); break;

	case 0x0C0: break;	// New program (patch) number, one byte of data
	case 0x0D0: break;	// Channel after-touch number, one byte of data

	case 0x0E0:	// Pitch wheel change(2'000H is normal or no change) :	\
				//	Bottom(least significant) 7 bits of value,			\
				//	Top (most significant) 7 bits of value

		GetChunk()->velocity = GetInputFile()->ReadByte(); break;

	default: assert(!"MIDI RUNNING STATUS");
	}
}