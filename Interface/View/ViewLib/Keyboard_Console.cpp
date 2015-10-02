# include "stdafx.h"
# include "Keyboard_Console.h"
# include "Keyboard_Console_handle.h"
# include "KeyboardStruct.h"

using namespace std;
using namespace Interface::View;

Keyboard_Console::Keyboard_Console() : Keyboard(),
	consoleHandle_(make_unique<Keyboard_Console_handle>())
{
	system("CLS");
	/*************************************************
	CONSOLE_SCREEN_BUFFER_INFO iConScrBuf;
	GetConsoleScreenBufferInfo(hStdOut_, &iConScrBuf);
	height_ = iConScrBuf.dwMaximumWindowSize.X - 2;
	width_ = iConScrBuf.dwMaximumWindowSize.Y - 3;
	*************************************************/
}

Keyboard_Console::~Keyboard_Console() {}

void Keyboard_Console::PressKey_impl(const int16_t note) const
{
	KeyboardStruct::PressKey(note);
}

void Keyboard_Console::ReleaseKey_impl(const int16_t note) const
{
	KeyboardStruct::ReleaseKey(note);
}

void Keyboard_Console::ReleaseAllKeys_impl() const
{
	for (int16_t i(NULL); i < 9 * 12; ++i) KeyboardStruct::ReleaseKey(i);
}

void Keyboard_Console::Update_impl() const
{
	KeyboardStruct::PrintKeyboard(consoleHandle_->Get());
}