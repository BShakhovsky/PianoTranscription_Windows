#pragma once
#include "resource.h"

class Piano abstract
{
public:
	static constexpr auto windowTitle = TEXT("Piano Fingers 3D");

	static std::vector<std::vector<std::map<int16_t, float>>> notes;
	static std::vector<std::vector<std::pair<unsigned, unsigned>>> milliSeconds;

	static std::shared_ptr<class IKeyboard> keyboard;

	static std::vector<size_t> indexes, tracks;
	static std::shared_ptr<size_t> leftTrack, rightTrack;
	static std::vector<std::vector<std::vector<std::string>>> fingersLeft, fingersRight;
	static std::vector<bool> percussions;

	static constexpr UINT timerTick = USER_TIMER_MINIMUM;

	static int Main(int);
private:
	static constexpr LPCTSTR szWindowClass_ = TEXT("MainWindow");
	static ATOM MyRegisterClass();
	static BOOL InitInstance(int);
};