# include "stdafx.h"
# include "MidiEvent.h"
# include "MidiStruct.h"
# include "FileParser.h"
# include "NoteNames.h"

using std::runtime_error;
using namespace boost;
using serialization::singleton;
using Model::MidiParser::MidiEvent;

MidiEvent& MidiEvent::GetInstance()
{
	return singleton<MidiEvent>::get_mutable_instance();
}

void MidiEvent::Read_impl()
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

	PrintMsg();
}

# define FORMAT_MSG(MSG1, VALUE) GetChunk()->metaText += (format{ MSG1##" = %1%" } % GetChunk()->VALUE).str();
# define PRINT_MSG1(MSG1) FORMAT_MSG(MSG1, note);
# define PRINT_MSG2(MSG1, MSG2) PRINT_MSG1(MSG1); FORMAT_MSG(MSG2, velocity);
# define PRINT_NOTE(MESSAGE) GetChunk()->metaText += (format{ MESSAGE##"Note %1% (%2% %3%)" }			\
														% GetChunk()->note								\
														% NoteNames::GetOctaveNumber(GetChunk()->note)	\
														% NoteNames::GetNoteName(GetChunk()->note)).str();
void MidiEvent::PrintMsg() const
{
	GetChunk()->metaText.clear();
	switch (GetChunk()->status & 0x0'F0)	// 0xF0 is negative ==> 0x0F0 is positive
	{
	case 0x090: if (GetChunk()->velocity) {	// else velocity = 0 == > "note-off" event
				PRINT_NOTE("");														break; }
	case 0x080: PRINT_NOTE("Note off (key is released) : ");						break;

	case 0x0A0: PRINT_MSG2("Key after-touch : Note number", ", Velocity");			break;
	case 0x0B0: PRINT_MSG2("Control change : Controller number", ", New value");	break;
	case 0x0C0: PRINT_MSG1("New program (patch) number");							break;
	case 0x0D0: PRINT_MSG1("Channel after-touch number");							break;

	case 0x0E0: PRINT_MSG2("Pitch wheel change(2'000H is normal or no change) : \
							\n\t\tBottom(least significant) 7 bits of value",
							"\n\t\tTop (most significant) 7 bits of value");		break;
	default: assert(!"MIDI RUNNING STATUS");
	}
}