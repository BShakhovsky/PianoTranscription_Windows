# include "stdafx.h"
# include "DirectSound_buffer.h"
# include "DirectSound_locker.h"

using Interface::Sound::DirectSound_buffer;

DirectSound_buffer::DirectSound_buffer() :
	buffer_(),
	format_(),
	isNormalized(false),
	locker_(nullptr)
{
	ZeroMemory(static_cast<void*>(&format_), sizeof format_);
	format_.wFormatTag = WAVE_FORMAT_PCM;
	format_.nChannels = 1;
	format_.nSamplesPerSec = 22'050;

	format_.wBitsPerSample = 8 * sizeof QUALITY;
	format_.nBlockAlign = static_cast<WORD>(sizeof QUALITY * format_.nChannels);
	format_.nAvgBytesPerSec = format_.nSamplesPerSec * format_.nBlockAlign;
}

DirectSound_buffer::~DirectSound_buffer() {}

void DirectSound_buffer::Init(HWND hWnd, const unsigned totalMilliSeconds)
{
	assert("WINDOW HANDLER HWND WAS NOT OBTAINED" && hWnd);

	LPDIRECTSOUND8 sound;
	if (FAILED(DirectSoundCreate8(NULL, &sound, NULL))) WARNING("Failed to create Direct Sound object");
	if (FAILED(sound->SetCooperativeLevel(hWnd, DSSCL_NORMAL))) WARNING("Failed to set cooperative level for Direct Sound");

	DSBUFFERDESC bufferStruct;
	ZeroMemory(&bufferStruct, sizeof bufferStruct);
	bufferStruct.dwSize = sizeof bufferStruct;
	bufferStruct.dwBufferBytes = static_cast<unsigned>(static_cast<unsigned long long>(format_.nAvgBytesPerSec)
																			* totalMilliSeconds / 1'000'000);
	bufferStruct.lpwfxFormat = &format_;

	if (FAILED(sound->CreateSoundBuffer(&bufferStruct, &buffer_, NULL))) WARNING("Failed to create sound buffer");

	if (locker_.get()) { BORIS_ASSERT(__FUNCTION__ " MUST NOT BE CALLED TWICE"); }
	else locker_.reset(new DirectSound_locker(buffer_));
}

void DirectSound_buffer::AddNote(const int deltaNote, const unsigned start, const unsigned duration)
{
	const auto freq(55 * pow(2.f, deltaNote / 12.f));
	locker_->AddHarmonicWaves(freq,
		static_cast<unsigned>(static_cast<unsigned long long>(format_.nAvgBytesPerSec) * start / 1'000),
		static_cast<unsigned>(static_cast<unsigned long long>(format_.nAvgBytesPerSec) * duration / 1'000),
		format_.nSamplesPerSec);
	isNormalized = false;
}

void DirectSound_buffer::NormalizeAndPlay()
{
	if (!isNormalized)
	{
		locker_->NormalizeVolume();
		isNormalized = true;
	}
	if (FAILED(buffer_->Play(NULL, NULL, NULL))) WARNING("Failed to play sound buffer");
}

void DirectSound_buffer::Stop() const
{
	if (FAILED(buffer_->Stop())) WARNING("Failed to stop playing the buffer :)");
}

void DirectSound_buffer::Clear() const
{
	locker_->Clear();
	if (FAILED(buffer_->SetCurrentPosition(NULL))) WARNING("Failed to reset buffer position to the beginning");
}