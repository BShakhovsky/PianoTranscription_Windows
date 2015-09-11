# include "stdafx.h"
# include "..\KeyboardConsole.h"
# include "KeyboardConsole_handle.h"
# include "KeyboardStruct.h"

using namespace std;
using namespace View;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
KeyboardConsole::KeyboardConsole()
	: Keyboard(),
	consoleHandle_(make_unique<KeyboardConsole_handle>())
{}
KeyboardConsole::~KeyboardConsole() {}
# pragma warning(pop)

void KeyboardConsole::Init_impl() const
{
	system("cls");
	system("COLOR 4");
	/*************************************************
	CONSOLE_SCREEN_BUFFER_INFO iConScrBuf;
	GetConsoleScreenBufferInfo(hStdOut_, &iConScrBuf);
	height_ = iConScrBuf.dwMaximumWindowSize.X - 2;
	width_ = iConScrBuf.dwMaximumWindowSize.Y - 3;
	*************************************************/
}

void KeyboardConsole::Update_impl() const
{
	SetConsoleCursorPosition(consoleHandle_->Get(), { NULL, NULL });
	for (auto i(NULL); i < 2; ++i)
	{
		for (auto row(NULL); row < KeyboardStruct::KB_HEIGHT; ++row)
		{
			for (unsigned octave(1); octave < 6; ++octave)
				for (auto column(NULL); column < KeyboardStruct::OCTAVE_WIDTH; ++column)
					cout << KeyboardStruct::keyboard[row][column];
			cout << '\n';	// std::endl would clean the buffer which is inefficient in cycles
		}
		cout << '\n';		// --//--
	}
	cout << endl;
}