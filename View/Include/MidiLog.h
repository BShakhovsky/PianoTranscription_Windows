# pragma once

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

		virtual ~MidiLog() = 0
		{
			PURE_VIRTUAL;
		}
	protected:
		MidiLog() = default;
	};
}