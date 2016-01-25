#include "stdafx.h"
#include "Keyboard.h"
#include "NoteNames.h"
#include "PianoFingering\BlackWhiteKeys.h"

using namespace std;

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

	SetBkMode(hdcMem_, TRANSPARENT);
}

int16_t CalcWhiteKeyIndex(int16_t note)
{
	auto noteIndex(NoteNames::GetNoteNumber(note));
	return static_cast<int16_t>((noteIndex < 5 ? noteIndex / 2 : (noteIndex + 1) / 2)
		+ (NoteNames::GetOctaveNumber(note) - 2) * 7 + 2);
}
int16_t CalcBlackKeyIndex(int16_t note)
{
	return static_cast<int16_t>(NoteNames::GetNoteNumber(note) / 2
		+ (NoteNames::GetOctaveNumber(note) - 2) * 7 + 3);
}

void Keyboard::GetKeyPosition(const int16_t note)
{
	if (BlackWhiteKeys::IsWhite(note))
		GetWhiteKeyPosition(CalcWhiteKeyIndex(note));
	else
		GetBlackKeyPosition(CalcBlackKeyIndex(note));
}

void Keyboard::ReleaseAllKeys()
{
	RECT rect{ 0 };
	for (int16_t i(0); i < nWhiteKeys_; ++i) DrawReleasedWhiteKey(i);
	DrawReleasedBlackKey(1);
	for (int16_t i(0); i < nOctaves_; ++i)
	{
		for (int16_t j(0); j < 2; ++j)	DrawReleasedBlackKey(3 + i * 7 + j);
		for (int16_t j(0); j < 3; ++j)	DrawReleasedBlackKey(6 + i * 7 + j);
	}
}

void Keyboard::PressKey(const int16_t note)
{
	GetKeyPosition(note);
	SetBkColor(hdcMem_, BlackWhiteKeys::IsWhite(note) ? RGB(0xFF, 0xFF, 0xFF) : 0);
	FillRect(hdcMem_, &rect_, hBrushPressed_);
	DrawEdge(hdcMem_, &rect_, EDGE_SUNKEN, BF_RECT);
}

void Keyboard::AssignFinger(const int16_t note, const string& fingers, const bool leftHand)
{
	SetTextColor(hdcMem_, leftHand ? RGB(0, 0xB0, 0xFF) : RGB(0xFF, 0, 0));
	GetKeyPosition(note);
	rect_.top = (rect_.top + rect_.bottom) / 2;

	wstring text(fingers.cbegin(), fingers.cend());
	const auto len(text.length());
	for (size_t i(0); i < len - 1; ++i) text.insert(i * 2 + 1, TEXT("\n"), 1);
	DrawText(hdcMem_, text.c_str(), static_cast<int>(text.length()), &rect_, DT_CENTER);
}