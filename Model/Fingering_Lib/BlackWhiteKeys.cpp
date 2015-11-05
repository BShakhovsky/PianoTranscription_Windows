# include "stdafx.h"
# include "BlackWhiteKeys.h"

using Model::Fingering::BlackWhiteKeys;

bool BlackWhiteKeys::IsBlack(const int16_t noteNumber)
{
	assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);

	enum class NOTE_NAMES { C, C1, D, D1, E, F, F1, G, G1, A, A1, B };

	switch (static_cast<NOTE_NAMES>(noteNumber % 12))
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