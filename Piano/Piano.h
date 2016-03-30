#pragma once
#include "resource.h"

class Piano abstract
{
public:
	static std::vector<std::vector<std::set<int16_t>>> notes;
	static std::vector<std::vector<std::pair<unsigned, unsigned>>> milliSeconds;

	static std::shared_ptr<class Keyboard> keyboard;
	static std::shared_ptr<class Sound_Facade> sound;

	static std::vector<size_t> indexes, tracks;
	static std::shared_ptr<size_t> leftTrack, rightTrack;
	static std::vector<std::vector<std::vector<std::string>>> fingersLeft, fingersRight;

	static constexpr UINT timerTick = USER_TIMER_MINIMUM;

	static int Main(HINSTANCE, int);
private:
	static constexpr LPCTSTR szWindowClass_ = TEXT("MainWindow");
	static ATOM MyRegisterClass(HINSTANCE);
	static BOOL InitInstance(HINSTANCE, int);
};