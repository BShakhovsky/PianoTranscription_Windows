#include "stdafx.h"
#include "Keyboard.h"
#include "NoteNames.h"
#include "BlackWhiteKeys.h"

Keyboard::Keyboard()
	: width_(0),
	height_(0),
	keyWidth_(0),
	keyHeight_(0),
	hdcMem_(nullptr),
	hBitmap_(nullptr),
	hBrushPressed_(CreateHatchBrush(HS_DIAGCROSS, RGB(0xFF, 0x80, 0x80)))
{}
Keyboard::~Keyboard()
{
	if (!hdcMem_)	DeleteDC(hdcMem_);
	if (!hBitmap_)	DeleteBitmap(hBitmap_);
					DeleteBrush(hBrushPressed_);
}

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

void Keyboard::DrawReleasedWhiteKey(int noteIndex) const
{
	RECT rect{ gap_ + noteIndex * keyWidth_, height_ - gap_ - keyHeight_,
		gap_ + (noteIndex + 1) * keyWidth_, height_ - gap_ };
	FillRect(hdcMem_, &rect, GetStockBrush(WHITE_BRUSH));
	DrawEdge(hdcMem_, &rect, EDGE_RAISED, BF_RECT);
}
void Keyboard::DrawPressedWhiteKey(int noteIndex) const
{
	RECT rect{ gap_ + noteIndex * keyWidth_, height_ - gap_ - keyHeight_,
		gap_ + (noteIndex + 1) * keyWidth_, height_ - gap_ };
	SetBkColor(hdcMem_, RGB(0xFF, 0xFF, 0xFF));
	FillRect(hdcMem_, &rect, hBrushPressed_);
	DrawEdge(hdcMem_, &rect, EDGE_SUNKEN, BF_RECT);
}
void Keyboard::DrawReleasedBlackKey(int noteIndex) const
{
	RECT rect{ gap_ + noteIndex * keyWidth_ - keyWidth_ / 3,
		height_ - gap_ - keyHeight_ - keyHeight_ / 3,
		gap_ + noteIndex * keyWidth_ + keyWidth_ / 3,
		height_ - gap_ - keyHeight_ + keyHeight_ / 2 };
	FillRect(hdcMem_, &rect, GetStockBrush(BLACK_BRUSH));
	DrawEdge(hdcMem_, &rect, EDGE_RAISED, BF_RECT);
}
void Keyboard::DrawPressedBlackKey(int noteIndex) const
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

void Keyboard::PressKey(int16_t note) const
{
	auto noteNo(NoteNames::GetNoteNumber(note));
	noteNo = BlackWhiteKeys::IsWhite(note) ? noteNo < 5 ? noteNo / 2 : (noteNo + 1) / 2 : noteNo / 2;
	noteNo += (NoteNames::GetOctaveNumber(note) - 2) * 7 + 2;
	if (BlackWhiteKeys::IsWhite(note))	DrawPressedWhiteKey(noteNo);
	else								DrawPressedBlackKey(noteNo + 1);
}

void Keyboard::Draw(const HDC hdc) const
{
	BitBlt(hdc, 0, 0, width_, height_, hdcMem_, 0, 0, SRCCOPY);
}