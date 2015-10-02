# pragma once

# define QUALITY	INT16
# define VOLUME		(INT16_MAX - 1)

namespace Interface
{
	namespace Sound
	{
		class DirectSound_locker : private boost::noncopyable
		{
			LPDIRECTSOUNDBUFFER buffer_;
			QUALITY* data_;
			DWORD size_;

			std::vector<float> loudData_;
		public:
			explicit DirectSound_locker(LPDIRECTSOUNDBUFFER);
			~DirectSound_locker();

			void AddHarmonicWaves(float primaryFrequency, DWORD offset, DWORD duration, DWORD samplesPerSecond);
			void NormalizeVolume() const;
			void Clear();
		private:
			DirectSound_locker() = delete;
		};
	}
}