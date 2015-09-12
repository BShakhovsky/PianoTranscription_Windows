# pragma once

namespace Model
{
	namespace MidiParser
	{
		class FileCounter
		{
			int bytesRemained_;
		public:
			FileCounter() :
				bytesRemained_(NULL)
			{}
			~FileCounter() = default;

			int Get() const
			{
				return bytesRemained_;
			}
			void Set(int value)
			{
				assert("LOGICAL ERROR: BYTES TO READ FROM MIDI FILE SHOULD BE A POSITIVE NUMBER" && value >= 0);
				bytesRemained_ = value;
			}
			int Reduce(int value, bool toCheck = true)
			{
				bytesRemained_ -= value;
				if (toCheck) assert("LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE"
										&& bytesRemained_ >= 0);
				return bytesRemained_;
			}
		};
	}
}