#include "stdafx.h"
#include "DirectSound_locker.h"

using namespace std;
using placeholders::_1;
using Interface::Sound::DirectSound_locker;

DirectSound_locker::DirectSound_locker(LPDIRECTSOUNDBUFFER buffer) :
	buffer_(buffer),
	data_(nullptr),
	size_(NULL),
	loudData_()
{
	if (FAILED(buffer_->Lock(NULL, NULL, reinterpret_cast<void**>(&data_), &size_, nullptr, NULL,
		DSBLOCK_ENTIREBUFFER)))
		WARNING("Failed to lock sound buffer");
	Clear();
}

DirectSound_locker::~DirectSound_locker()
{
	if (FAILED(buffer_->Unlock(static_cast<void*>(data_), size_, nullptr, NULL)))
		WARNING("Failed to unlock sound buffer");
}

void DirectSound_locker::AddHarmonicWaves(const float primFreq, const DWORD offset, const DWORD duration,
	const DWORD samplPerSec)
{
	const auto period(static_cast<size_t>(samplPerSec / primFreq + 0.5f));
	const auto unitAngle(2 * static_cast<float>(M_PI) / period),
		dampCoeff(pow(0.15f, 1.f / primFreq));

	vector<float> ringBuff;
//	if (XYLO_PHONE)
//	{
//		ringBuff.reserve(period);
//		for (size_t i(NULL); i < period; ++i) ringBuff.push_back(sin(unitAngle * i));
//	}
//	else if (TENSED_GUITAR)
//	{
//		ringBuff.reserve(period);
//		srand(static_cast<unsigned>(time(NULL)));
//		for (size_t i(NULL); i < period; ++i) ringBuff.push_back(rand() - RAND_MAX / 2);
//	}
//	else if (XYLO_PHONE + TENSED_GUITAR)
//	{
//		ringBuff.reserve(period);
//		srand(static_cast<unsigned>(time(NULL)));
//		for (size_t i(NULL); i < period; ++i) ringBuff.push_back(
//			sin(unitAngle * i) + (static_cast<float>(rand()) / RAND_MAX - 0.5f));
//	}
//	else if (ELECTRIC_PIANO)
//	{
		ringBuff.assign(period, NULL);
		for (size_t i(NULL); i < period; ++i)
		{
			const auto angle(unitAngle * i);
			static const float harmonics[] = { 1.0f,
				0.399064778f, 0.229404484f, 0.151836061f, 0.196754229f, 0.093742264f,
				0.060871957f, 0.138605419f, 0.010535002f, 0.071021868f, 0.029954614f,
				0.051299684f, 0.055948288f, 0.066208224f, 0.010067391f, 0.007536790f,
				0.008196947f, 0.012955577f, 0.007316738f, 0.006216476f, 0.005116215f,
				0.006243983f, 0.002860679f, 0.002558108f,	NULL,		0.001650392f };
			for (auto j(0); j < sizeof harmonics / sizeof *harmonics; ++j)
				ringBuff.at(i) += harmonics[j] * sin(angle * (j + 1));
		}
//	}
//	else assert(false);
	const auto
		start(offset / sizeof *data_),
		finish(min(offset + duration, size_) / sizeof *data_);
	for (auto i(start); i < finish; ++i)
	{
		const auto iBuff((i - start) % period);
		ringBuff.at(iBuff) += ringBuff.at((iBuff + 1) % period);
		loudData_.at(i) += ringBuff.at(iBuff) *= dampCoeff / 2;
	}
}

void DirectSound_locker::NormalizeVolume() const
{
	const auto minMax(minmax_element(loudData_.cbegin(), loudData_.cend()));
	const auto maxVal(max(-*minMax.first, *minMax.second));
	transform(loudData_.cbegin(), loudData_.cend(), data_, bind(multiplies<double>(), _1, VOLUME / maxVal));
}

void DirectSound_locker::Clear()
{
	loudData_.assign(size_ / sizeof *data_, NULL);
}