# include "stdafx.h"
# include "DirectHarmonics.h"
# include "DirectSound_buffer.h"

using namespace Interface::Sound;

DirectHarmonics::DirectHarmonics(HWND hWnd, const unsigned totalMilliSeconds) :
	buffer_(new DirectSound_buffer())
{
	buffer_->Init(hWnd, totalMilliSeconds);
}

DirectHarmonics::~DirectHarmonics()
{
	delete buffer_;
}

void DirectHarmonics::AddNote(const int note, const unsigned start, const unsigned duration) const
{
	const auto delta(note > noteMin_ ? note < noteMax_ ? note - noteMin_ : noteMax_ - noteMin_ : NULL);
	buffer_->AddNote(delta, start, duration);
}

void DirectHarmonics::Play() const
{
	buffer_->NormalizeAndPlay();
}

void DirectHarmonics::Stop() const
{
	buffer_->Stop();
}

void DirectHarmonics::Clear() const
{
	Stop();
	buffer_->Clear();
}