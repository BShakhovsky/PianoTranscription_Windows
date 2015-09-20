# include "stdafx.h"
# include "Factory_Console.h"
# include "MidiLog_Console.h"
# include "Keyboard_Console.h"

using std::unique_ptr;
using namespace View;

unique_ptr<MidiLog> Factory_Console::GetMidiLog_impl() const
{
	return unique_ptr<MidiLog_Console>(new MidiLog_Console);
}

unique_ptr<Keyboard> Factory_Console::GetKeyboard_impl() const
{
	return unique_ptr<Keyboard_Console>(new Keyboard_Console);
}