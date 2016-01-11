#pragma once

#include "resource.h"

class Piano abstract
{
public:
	static std::unique_ptr<class MidiParser_Facade> midi;
	static std::unique_ptr<class Keyboard> keyboard;
	static std::unique_ptr<class Sound_Facade> sound;

	static std::vector<size_t> indexes, tracks;

	static int Main(HINSTANCE, int);
private:
	static constexpr LPCTSTR szWindowClass_ = TEXT("MainWindow");
	static ATOM MyRegisterClass(HINSTANCE);
	static BOOL InitInstance(HINSTANCE, int);
};