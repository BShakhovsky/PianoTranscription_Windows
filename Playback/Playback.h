# pragma once

class Playback abstract
{
public:
	Playback() = delete;
	~Playback() = delete;

	Playback(const Playback&) = delete;
	Playback(const Playback&&) = delete;
	Playback& operator=(const Playback&) = delete;
	Playback&& operator=(const Playback&&) = delete;

	static void Execute(HWND, const char* fileName);
};