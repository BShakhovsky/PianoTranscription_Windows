#include "stdafx.h"
#include "Keyboard.h"
#include "Keyboard_pimpl.h"

Keyboard::Keyboard()
	: pimpl_(new Keyboard_pimpl)
{}

#pragma warning(push)
#pragma warning(disable:4711)	// automatic inline expansion
Keyboard::~Keyboard()
{
	delete pimpl_;
}
#pragma warning(pop)

void Keyboard::UpdateSize(const HWND hWnd, const int width, const int height) const
{
	pimpl_->UpdateSize(hWnd, width, height);
}

void Keyboard::PressKey(const int16_t note) const
{
	pimpl_->PressKey(note);
}
void Keyboard::ReleaseWhiteKeys() const
{
	pimpl_->ReleaseWhiteKeys();
}

void Keyboard::AssignFinger(const int16_t note, const char* fingers, const bool leftHand) const
{
	pimpl_->AssignFinger(note, fingers, leftHand);
}

void Keyboard::Draw(const HDC hdc) const
{
	pimpl_->Draw(hdc);
}