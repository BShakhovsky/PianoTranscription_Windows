#include "stdafx.h"
#include "Keyboard.h"
#include "NoteNames.h"
#include "PianoFingering\BlackWhiteKeys.h"

void Keyboard::UpdateSize(const HWND hWnd, const int width, const int height)
{
	const auto hdc(GetDC(hWnd));
	hdcMem_ = CreateCompatibleDC(hdc);

	width_ = width;
	height_ = height;
	keyWidth_ = (width_ - 2 * gap_) / nWhiteKeys_;
	keyHeight_ = keyWidth_ * 5;
	
	DeleteBitmap(hBitmap_);
	hBitmap_ = CreateCompatibleBitmap(hdc, width_, height_);
	ReleaseDC(hWnd, hdc);
	SelectBitmap(hdcMem_, hBitmap_);

	RECT rect{ 0 };
	GetClientRect(hWnd, &rect);
	FillRect(hdcMem_, &rect, GetStockBrush(WHITE_BRUSH));
}


void Keyboard::DrawPressedWhiteKey(const int noteIndex) const
{
	RECT rect{ gap_ + noteIndex * keyWidth_, height_ - gap_ - keyHeight_,
		gap_ + (noteIndex + 1) * keyWidth_, height_ - gap_ };
	SetBkColor(hdcMem_, RGB(0xFF, 0xFF, 0xFF));
	FillRect(hdcMem_, &rect, hBrushPressed_);
	DrawEdge(hdcMem_, &rect, EDGE_SUNKEN, BF_RECT);
}

void Keyboard::DrawPressedBlackKey(const int noteIndex) const
{
	RECT rect{ gap_ + noteIndex * keyWidth_ - keyWidth_ / 3 + 1,
		height_ - gap_ - keyHeight_ - keyHeight_ / 3 + 1,
		gap_ + noteIndex * keyWidth_ + keyWidth_ / 3 - 1,
		height_ - gap_ - keyHeight_ + keyHeight_ / 2 - 1};
	SetBkColor(hdcMem_, 0);
	FillRect(hdcMem_, &rect, hBrushPressed_);
	DrawEdge(hdcMem_, &rect, EDGE_SUNKEN, BF_RECT);
}


void Keyboard::ReleaseAllKeys() const
{
	for (auto i(0); i < nWhiteKeys_; ++i) DrawReleasedWhiteKey(i);
	DrawReleasedBlackKey(1);
	for (auto i(0); i < nOctaves_; ++i)
	{
		for (auto j(0); j < 2; ++j)	DrawReleasedBlackKey(3 + i * 7 + j);
		for (auto j(0); j < 3; ++j)	DrawReleasedBlackKey(6 + i * 7 + j);
	}
}

void Keyboard::PressKey(const int16_t note) const
{
	auto noteNo(NoteNames::GetNoteNumber(note));
	noteNo = BlackWhiteKeys::IsWhite(note) ? noteNo < 5 ? noteNo / 2 : (noteNo + 1) / 2 : noteNo / 2;
	noteNo += (NoteNames::GetOctaveNumber(note) - 2) * 7 + 2;
	if (BlackWhiteKeys::IsWhite(note))	DrawPressedWhiteKey(noteNo);
	else								DrawPressedBlackKey(noteNo + 1);
}