# pragma once

class NoteNames abstract
{
public:
	static constexpr int NUM_NOTES = 12;
	enum class NOTE_NAMES { C, C1, D, D1, E, F, F1, G, G1, A, A1, B };

	static int GetOctaveNumber(int16_t noteNumber)
	{
		assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);
		return noteNumber / NUM_NOTES;
	}
	static int GetNoteNumber(int16_t noteNumber)
	{
		assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);
		return noteNumber % NUM_NOTES;
	}
	static NOTE_NAMES GetNoteName(int16_t noteNumber)
	{
		return static_cast<NOTE_NAMES>(GetNoteNumber(noteNumber));
	}
};
typedef NoteNames::NOTE_NAMES NOTE_NAMES;