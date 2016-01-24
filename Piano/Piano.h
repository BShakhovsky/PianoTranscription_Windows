#pragma once

#include "resource.h"

#ifdef UNICODE
	typedef std::wstring String;
	typedef boost::wformat Format;
#else
	typedef std::string String;
	typedef boost::format Format;
#endif

class Piano abstract
{
public:
	static std::unique_ptr<class MidiParser_Facade> midi;
	static std::unique_ptr<class Keyboard> keyboard;
	static std::unique_ptr<class Sound_Facade> sound;

	static std::vector<size_t> indexes, tracks;
	static std::unique_ptr<size_t> leftTrack, rightTrack;
	static struct Hands {
		size_t leftIndex, rightIndex, leftIndexTotal, rightIndexTotal;
	} hands;

	static int Main(HINSTANCE, int);
private:
	static constexpr LPCTSTR szWindowClass_ = TEXT("MainWindow");
	static ATOM MyRegisterClass(HINSTANCE);
	static BOOL InitInstance(HINSTANCE, int);
};