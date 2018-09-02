#pragma once

class Spectrogram abstract
{
public:
	static LPCTSTR mediaFile;
	static std::string midiFile;

	static INT_PTR CALLBACK Main(HWND, UINT, WPARAM, LPARAM);
private:
	static BOOL OnInitDialog(HWND, HWND, LPARAM);
#pragma warning(suppress:4711) // Automatic inline expansion
	static void OnDestroyDialog(HWND);
	static void OnSize(HWND, UINT, int, int);
	static void OnPaint(HWND);
	static void OnCommand(HWND, int, HWND, UINT);

	static std::unique_ptr<class PianoToMidi_Win> media;
};