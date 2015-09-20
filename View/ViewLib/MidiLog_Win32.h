# pragma once
# include "..\Include\MidiLog.h"

namespace View
{
	class MidiLog_Win32 : public MidiLog
	{
	public:
		virtual ~MidiLog_Win32() override final
		{
			std::cout << "YOU MAY CLOSE THIS CONSOLE WINDOW AT ANY TIME" << std::endl;
			FreeConsole();
		}
	private:
		MidiLog_Win32() : MidiLog()
		{
			FreeConsole();
			AllocConsole();
			FILE* stream(nullptr);
			freopen_s(&stream, "CON", "w", stdout);
		}
		friend class Factory_Win32;
	};
}