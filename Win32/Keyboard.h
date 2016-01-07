#pragma once

class Keyboard : private boost::noncopyable
{
public:
	Keyboard()
		: width_(0),
		height_(0),
		keyWidth_(0),
		keyHeight_(0),
		hdcMem_(nullptr),
		hBitmap_(nullptr),
		hBrushPressed_(CreateHatchBrush(HS_DIAGCROSS, RGB(0xA0, 0xA0, 0xA0)))
	{}
	~Keyboard()
	{
		if (!hdcMem_)	DeleteDC(hdcMem_);
		if (!hBitmap_)	DeleteBitmap(hBitmap_);
		DeleteBrush(hBrushPressed_);
	}

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
	void Draw(HDC hdc) const
	{
		BitBlt(hdc, 0, 0, width_, height_, hdcMem_, 0, 0, SRCCOPY);
	}
private:
	const int nOctaves_ = 7, nWhiteKeys_ = nOctaves_ * 7 + 2 + 1, gap_ = 0;
	int width_, height_, keyWidth_, keyHeight_;
	HDC hdcMem_;
	HBITMAP hBitmap_;
	HBRUSH hBrushPressed_;

	void DrawPressedWhiteKey(int noteIndex) const;
	void DrawPressedBlackKey(int noteIndex) const;

	void DrawReleasedWhiteKey(int noteIndex) const
	{
		RECT rect{ gap_ + noteIndex * keyWidth_, height_ - gap_ - keyHeight_,
			gap_ + (noteIndex + 1) * keyWidth_, height_ - gap_ };
		FillRect(hdcMem_, &rect, GetStockBrush(WHITE_BRUSH));
		DrawEdge(hdcMem_, &rect, EDGE_RAISED, BF_RECT);
	}
	void DrawReleasedBlackKey(int noteIndex) const
	{
		RECT rect{ gap_ + noteIndex * keyWidth_ - keyWidth_ / 3,
			height_ - gap_ - keyHeight_ - keyHeight_ / 3,
			gap_ + noteIndex * keyWidth_ + keyWidth_ / 3,
			height_ - gap_ - keyHeight_ + keyHeight_ / 2 };
		FillRect(hdcMem_, &rect, GetStockBrush(BLACK_BRUSH));
		DrawEdge(hdcMem_, &rect, EDGE_RAISED, BF_RECT);
	}
};