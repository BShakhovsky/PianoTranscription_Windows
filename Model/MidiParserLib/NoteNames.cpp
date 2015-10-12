#include "stdafx.h"
#include "..\MidiParser_Include\NoteNames.h"

using Model::MidiParser::NoteNames;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
bool NoteNames::IsBlack(const int16_t noteNumber)
{
	assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);

	switch (GetNoteName(noteNumber))
	{
	case NOTE_NAMES::C:
	case NOTE_NAMES::D:
	case NOTE_NAMES::E:
	case NOTE_NAMES::F:
	case NOTE_NAMES::G:
	case NOTE_NAMES::A:
	case NOTE_NAMES::B: return false; break;
	case NOTE_NAMES::C1:
	case NOTE_NAMES::D1:
	case NOTE_NAMES::F1:
	case NOTE_NAMES::G1:
	case NOTE_NAMES::A1: return true; break;
	default: assert(!"WRONG NOTE NAME");
						return false;
	}
}
# pragma warning(pop)