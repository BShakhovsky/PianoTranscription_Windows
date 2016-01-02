#include "stdafx.h"
#include "Keyboard.h"

inline void DrawBlackKey(HDC hdc, RECT* rect, unsigned edge)
{
	FillRect(hdc, rect, GetStockBrush(BLACK_BRUSH));
	DrawEdge(hdc, rect, edge, BF_RECT);
}

void Keyboard::ReleaseAllKeys(const HDC hdc) const
{
	const auto wKeyWidth((width_ - 2 * gap_) / nWhiteKeys_), wKeyHeight(wKeyWidth * 5);
	for (auto i(0); i < nWhiteKeys_; ++i)
	{
		RECT rect{ gap_ + i * wKeyWidth, height_ - gap_ - wKeyHeight,
			gap_ + (i + 1) * wKeyWidth, height_ - gap_ };
		DrawEdge(hdc, &rect, EDGE_RAISED, BF_RECT);
	}
	RECT rect{ gap_ + 2 * wKeyWidth / 3, height_ - gap_ - wKeyHeight - wKeyHeight / 3,
		gap_ + 4 * wKeyWidth / 3, height_ - gap_ - wKeyHeight + wKeyHeight / 2 };
	DrawBlackKey(hdc, &rect, EDGE_RAISED);
	for (auto i(0); i < nOctaves_; ++i)
	{
		for (auto j(0); j < 2; ++j)
		{
			rect = { gap_ + (3 + i * 7 + j) * wKeyWidth - wKeyWidth / 3,
				height_ - gap_ - wKeyHeight - wKeyHeight / 3,
				gap_ + (3 + i * 7 + j) * wKeyWidth + wKeyWidth / 3,
				height_ - gap_ - wKeyHeight + wKeyHeight / 2 };
			DrawBlackKey(hdc, &rect, EDGE_RAISED);
		}
		for (auto j(0); j < 3; ++j)
		{
			rect = { gap_ + (3 + 3 + i * 7 + j) * wKeyWidth - wKeyWidth / 3,
				height_ - gap_ - wKeyHeight - wKeyHeight / 3,
				gap_ + (3 + 3 + i * 7 + j) * wKeyWidth + wKeyWidth / 3,
				height_ - gap_ - wKeyHeight + wKeyHeight / 2 };
			DrawBlackKey(hdc, &rect, EDGE_RAISED);
		}
	}
}