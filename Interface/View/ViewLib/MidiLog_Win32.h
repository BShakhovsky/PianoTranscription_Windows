# pragma once
# include "..\Include\MidiLog.h"

namespace Interface
{
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
				if (!freopen_s(&stream, "CON", "w", stdout)) throw std::runtime_error("CANNOT OPEN CONSOLE WINDOW");
			}
			friend class Factory_Win32;
		};
	}
}