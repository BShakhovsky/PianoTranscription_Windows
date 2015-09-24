# include "stdafx.h"
# include "Keyboard_Console.h"
# include "Keyboard_Console_handle.h"
# include "KeyboardStruct.h"

using namespace std;
using namespace Interface::View;

Keyboard_Console::Keyboard_Console() : Keyboard(),
	consoleHandle_(make_unique<Keyboard_Console_handle>())
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

Keyboard_Console::~Keyboard_Console() {}

void Keyboard_Console::Update_impl() const
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