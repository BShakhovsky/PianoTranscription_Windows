# pragma once

namespace Model
{
	namespace MidiParser
	{
		class NoteNames abstract : private boost::noncopyable
		{
			static constexpr unsigned NUM_NOTES = 12;
			static const char noteNames[NUM_NOTES][3];
		public:
			static unsigned GetOctaveNumber(int noteNumber)
			{
				assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);
				return noteNumber / NUM_NOTES;
			}
			static const char* GetNoteName(int noteNumber)
			{
				assert("NOTE MUST BE A POSITIVE NUMBER" && noteNumber >= 0);
				return noteNames[noteNumber % NUM_NOTES];
			}
		private:
			NoteNames() = delete;
			~NoteNames() = delete;
		};
	}
}