# pragma once
# include "..\Include\MidiLog.h"

namespace View
{
	class MidiLog_Console : public MidiLog
	{
	public:
		virtual ~MidiLog_Console() override final = default;
	private:
		MidiLog_Console() = default;
		friend class Factory_Console;
	};
}