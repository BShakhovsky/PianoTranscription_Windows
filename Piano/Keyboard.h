#pragma once

class Keyboard
{
public:
	Keyboard();
	~Keyboard();

	void UpdateSize(HWND, int width, int height) const;

	void PressKey(int16_t note) const;
	void ReleaseWhiteKeys() const;
	void AssignFinger(int16_t note, const char* fingers, bool leftHand = false) const;

	void Draw(HDC) const;
private:
	class Keyboard_pimpl* pimpl_;

	Keyboard(const Keyboard&) = delete;
	Keyboard(const Keyboard&&) = delete;
	Keyboard& operator=(const Keyboard&) = delete;
	Keyboard&& operator=(const Keyboard&&) = delete;
};