# include "stdafx.h"
# include "..\Include\MidiLog.h"

# pragma comment(lib, "user32.lib")

using Interface::View::MidiLog;

void MidiLog::Maximize()
{
	ShowWindow(GetConsoleWindow(), SW_MAXIMIZE);
}