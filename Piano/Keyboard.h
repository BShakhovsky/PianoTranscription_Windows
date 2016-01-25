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
		hBrushPressed_(CreateHatchBrush(HS_DIAGCROSS, RGB(0xA0, 0xA0, 0xA0))),
		rect_({ 0 })
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

	void ReleaseAllKeys();
	void PressKey(int16_t note);
	void AssignFinger(int16_t note, const std::string& fingers, bool leftHand = false);
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

	RECT rect_;
	void GetWhiteKeyPosition(int16_t keyIndex)
	{
		rect_.left = gap_ + keyIndex * keyWidth_;
		rect_.right = gap_ + (keyIndex + 1) * keyWidth_;

		rect_.top = height_ - gap_ - keyHeight_;
		rect_.bottom = height_ - gap_;
	}
	void GetBlackKeyPosition(int16_t keyIndex)
	{
		rect_.left = gap_ + keyIndex * keyWidth_ - keyWidth_ / 3;
		rect_.right = gap_ + keyIndex * keyWidth_ + keyWidth_ / 3;

		rect_.top = height_ - gap_ - keyHeight_ - keyHeight_ / 3;
		rect_.bottom = height_ - gap_ - keyHeight_ + keyHeight_ / 2;
	}
	void GetKeyPosition(int16_t note);

	void DrawReleasedWhiteKey(int16_t keyIndex)
	{
		GetWhiteKeyPosition(keyIndex);
		FillRect(hdcMem_, &rect_, GetStockBrush(WHITE_BRUSH));
		DrawEdge(hdcMem_, &rect_, EDGE_RAISED, BF_RECT);
	}
	void DrawReleasedBlackKey(int16_t keyIndex)
	{
		GetBlackKeyPosition(keyIndex);
		FillRect(hdcMem_, &rect_, GetStockBrush(BLACK_BRUSH));
		DrawEdge(hdcMem_, &rect_, EDGE_RAISED, BF_RECT);
	}
};