#pragma once

class Keyboard : private boost::noncopyable
{
public:
	Keyboard();
	~Keyboard();

	void UpdateSize(HWND, int width, int height);
	int GetWidth() const
	{
		return width_;
	}
	int GetHeight() const
	{
		return height_;
	}

	void ReleaseAllKeys() const;
	void PressKey(int16_t note) const;
	void Draw(HDC) const;
private:
	const int nOctaves_ = 7, nWhiteKeys_ = nOctaves_ * 7 + 2 + 1, gap_ = 0;
	int width_, height_, keyWidth_, keyHeight_;
	HDC hdcMem_;
	HBITMAP hBitmap_;
	HBRUSH hBrushPressed_;

	void DrawPressedWhiteKey(int noteIndex) const;
	void DrawPressedBlackKey(int noteIndex) const;
	void DrawReleasedWhiteKey(int noteIndex) const;
	void DrawReleasedBlackKey(int noteIndex) const;
};