#pragma once

class Keyboard_pimpl : private boost::noncopyable
{
public:
	Keyboard_pimpl();
	~Keyboard_pimpl();

	void UpdateSize(HWND, int width, int height);

	void PressKey(int16_t note);
	void ReleaseWhiteKeys();
	void AssignFinger(int16_t note, const char* fingers, bool leftHand = false);

	void Draw(HDC);
private:
	const std::unique_ptr<struct KeyboardData> data_;

	void GetWhiteKeyPosition(int16_t keyIndex);
	void GetBlackKeyPosition(int16_t keyIndex);

	void PressWhiteKey(int16_t note);
	void PressBlackKey(int16_t note);

	void DrawReleasedWhiteKey(int16_t keyIndex);
	void DrawReleasedBlackKey(int16_t keyIndex);
	void ReleaseBlackKeys();

	void DrawFinger(const std::wstring& fingers, bool leftHand);
};