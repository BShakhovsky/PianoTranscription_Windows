# pragma once

namespace Interface
{
	namespace View
	{
		class NoteNames abstract : private boost::noncopyable
		{
		public:
			static constexpr unsigned NUM_NOTES = 12;
			enum class NOTE_NAMES { C, C1, D, D1, E, F, F1, G, G1, A, A1, B };

			static unsigned GetOctaveNumber(int16_t noteNumber)
			{
				assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);
				return noteNumber / NUM_NOTES;
			}
			static NOTE_NAMES GetNoteName(int16_t noteNumber)
			{
				assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);
				return static_cast<NOTE_NAMES>(noteNumber % NUM_NOTES);
			}
		private:
			NoteNames() = delete;
			~NoteNames() = delete;
		};
		
		typedef NoteNames::NOTE_NAMES NOTE_NAMES;
	}
}