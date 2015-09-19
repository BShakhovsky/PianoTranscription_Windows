# include "stdafx.h"
# include "MidiEvent.h"
# include "MidiStruct.h"
# include "FileParser.h"

using namespace std;
using Model::MidiParser::MidiEvent;

char MidiEvent::runStatus_ = '\0';

EVENT_IMPL(Midi)
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
	GetChunk()->velocity = GetInputFile()->ReadByte();

	PrintMidiEvent();
}

void MidiEvent::PrintMidiEvent() const
{
	switch (GetChunk()->status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
	{
	case 0x090: if (GetChunk()->velocity) {	cout << ' ' << GetChunk()->note;	// else velocity = 0 ==> "note-off" event
				break; }
	case 0x080: break;	// Note off (key is released)

	case 0x0A0: break;	// Key after-touch : Note number, Velocity
	case 0x0B0:	break;	// Control change : Controller number, New value
	case 0x0C0: break;	// New program (patch) number
	case 0x0D0: break;	// Channel after-touch number

	case 0x0E0: break;	// Pitch wheel change(2'000H is normal or no change) :	\
						//	Bottom(least significant) 7 bits of value,			\
						//	Top (most significant) 7 bits of value
	default: assert(!"MIDI RUNNING STATUS");
	}
}