#include "stdafx.h"
#include "DirectHarmonic.h"

using namespace std;
using Interface::Sound::DirectHarmonic;

void DirectHarmonic::Execute()
{
	LPDIRECTSOUND8 sound;
	if (FAILED(DirectSoundCreate8(NULL, &sound, NULL))) cout << "NOT OK" << endl;
	if (FAILED(sound->SetCooperativeLevel(GetConsoleWindow(), DSSCL_NORMAL))) cout << "NOT OK" << endl;

	WAVEFORMATEX format;
	ZeroMemory(static_cast<void*>(&format), sizeof format);
	format.wFormatTag = WAVE_FORMAT_PCM;
	format.nChannels = 1;
	format.nSamplesPerSec = 22'050;
# define QUALITY INT16
# define VOLUME INT16_MAX
	format.wBitsPerSample = 8 * sizeof QUALITY;
	format.nBlockAlign = static_cast<WORD>((format.wBitsPerSample / 8) * format.nChannels);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

	DSBUFFERDESC bufferStruct;
	ZeroMemory(&bufferStruct, sizeof bufferStruct);
	bufferStruct.dwSize = sizeof bufferStruct;
	bufferStruct.dwBufferBytes = format.nAvgBytesPerSec * 5;	// 5 seconds
	bufferStruct.lpwfxFormat = &format;

	LPDIRECTSOUNDBUFFER buffer;
	if (FAILED(sound->CreateSoundBuffer(&bufferStruct, &buffer, NULL))) cout << "NOT OK" << endl;

	QUALITY* data;
	DWORD size;
	if (FAILED(buffer->Lock(NULL, NULL, reinterpret_cast<void**>(&data), &size, nullptr, NULL, DSBLOCK_ENTIREBUFFER)))
		cout << "NOT OK" << endl;
	else
	{
		constexpr auto
			noteMin(21),	// Lya of second octave, 55 Hz
			noteMax(120);	// Do of last octave, 55 * 2 ^ (8 + 3/12) =~ 14'080 * 1.2 =~ 16'744 Hz < 22'050 Hz
		constexpr auto note1(40), note2(44), note3(47), note4(52);

		constexpr auto
			delta1(note1 > noteMin ? note1 < noteMax ? note1 - noteMin : noteMax : noteMin),
			delta2(note2 > noteMin ? note2 < noteMax ? note2 - noteMin : noteMax : noteMin),
			delta3(note3 > noteMin ? note3 < noteMax ? note3 - noteMin : noteMax : noteMin),
			delta4(note4 > noteMin ? note4 < noteMax ? note4 - noteMin : noteMax : noteMin);

		const auto
			freq1(55 * powf(2, delta1 / 12.f)),
			freq2(55 * powf(2, delta2 / 12.f)),
			freq3(55 * powf(2, delta3 / 12.f)),
			freq4(55 * powf(2, delta4 / 12.f));

		for (DWORD i(NULL); i < size / sizeof *data; ++i)
		{
			auto result(sin(2 * M_PI * i * freq1 / format.nSamplesPerSec));
			result += sin(2 * M_PI * i * freq2 / format.nSamplesPerSec);
			result += sin(2 * M_PI * i * freq3 / format.nSamplesPerSec);
			result += sin(2 * M_PI * i * freq4 / format.nSamplesPerSec);

			data[i] = static_cast<QUALITY>(VOLUME * result / 4);
		}
	}
	if (FAILED(buffer->Unlock(static_cast<void*>(data), size, nullptr, NULL))) cout << "NOT OK" << endl;

	if (FAILED(buffer->SetCurrentPosition(NULL))) cout << "NOT OK" << endl;
	else if (FAILED(buffer->Play(NULL, NULL, NULL))) cout << "NOT OK" << endl;
}