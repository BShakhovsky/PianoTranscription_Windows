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
	
	static void StopPlaying();
	static void UpdateTime(DWORD);
	static int PlayTrack(size_t, DWORD);
	static bool OnTimer(HWND, DWORD);
	static void CALLBACK OnTimer(HWND, UINT, UINT_PTR, DWORD);

	static void RewindTracks(int);
	static void UpdateScrollBar(int);
	static void NextChord();
	static void PrevChord();
	static void OnHScroll(HWND, HWND, UINT, int);
	static void OnCommand(HWND, int, HWND, UINT);

	static HWND time_;
	static bool isPlaying_;
	static DWORD start_;
};