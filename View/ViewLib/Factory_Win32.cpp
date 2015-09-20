# include "stdafx.h"
# include "Factory_Win32.h"
# include "MidiLog_Win32.h"
# include "Keyboard_Win32.h"

using namespace std;
using namespace View;

unique_ptr<MidiLog> Factory_Win32::GetMidiLog_impl() const
{
	return unique_ptr<MidiLog_Win32>(new MidiLog_Win32);
}

unique_ptr<Keyboard> Factory_Win32::GetKeyboard_impl() const
{
	return unique_ptr<Keyboard_Win32>(new Keyboard_Win32);
}