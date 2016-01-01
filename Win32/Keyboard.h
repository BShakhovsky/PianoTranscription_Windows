#pragma once

class Keyboard : private boost::noncopyable
{
public:
	void SetWidthHeight(int width, int height)
	{
		width_ = width;
		height_ = height;
	}
	int GetWidth() const
	{
		return width_;
	}
	int GetHeight() const
	{
		return height_;
	}

	void ReleaseAllKeys(HDC) const;
private:
	const int nOctaves_ = 7, nWhiteKeys_ = nOctaves_ * 7 + 2 + 1, gap_ = 20;
	int width_, height_;
};