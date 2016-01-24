#pragma once

class Controls abstract
{
public:
	static HWND hDlgControls, midiLog,
		leftHand, rightHand, progressLeft, progressRight,
		trackList, scrollBar, playButton;

	static void Reset();
	static INT_PTR CALLBACK Main(HWND, UINT, WPARAM, LPARAM);
private:
	static BOOL OnInitDialog(HWND, HWND, LPARAM);

	static void OnSoundError(const class SoundError&);
	static void UpdateTime(DWORD);
	static int PlayTrack(size_t, DWORD);
	static void CALLBACK OnTimer(HWND, UINT, UINT_PTR, DWORD);

	static void UpdateScrollBar(int);
	static void NextChord();
	static void PrevChord();
	static void OnHScroll(HWND, HWND, UINT, int);
	static void OnCommand(HWND, int, HWND, UINT);

	static constexpr UINT timerTick_ = USER_TIMER_MINIMUM;
	static HWND time_;
	static bool isPlaying_;
	static DWORD start_;
};