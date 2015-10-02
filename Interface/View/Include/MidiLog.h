# pragma once

namespace Interface
{
	namespace View
	{
		class MidiLog abstract : private boost::noncopyable
		{
		public:
			void Maximize();

			virtual ~MidiLog() = 0 {}
		protected:
			MidiLog() = default;
		};
	}
}