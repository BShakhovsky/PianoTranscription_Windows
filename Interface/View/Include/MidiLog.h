# pragma once

namespace Interface
{
	namespace View
	{
		class MidiLog abstract : private boost::noncopyable
		{
		public:
			MidiLog* Maximize()
			{
				ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
				return this;
			}

			virtual ~MidiLog() = 0 {}
		protected:
			MidiLog() = default;
		};
	}
}