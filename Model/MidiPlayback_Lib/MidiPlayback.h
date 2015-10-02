# pragma once

namespace Model
{
	namespace MidiPlayback
	{
		class MidiPlayback abstract : private boost::noncopyable
		{
		public:
			MidiPlayback() = delete;
			~MidiPlayback() = delete;

			static void Execute(const char* fileName);
		};
	}
}