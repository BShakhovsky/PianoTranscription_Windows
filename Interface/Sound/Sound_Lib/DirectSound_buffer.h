# pragma once

namespace Interface
{
	namespace Sound
	{
		class DirectSound_buffer : private boost::noncopyable
		{
			LPDIRECTSOUNDBUFFER buffer_;
			WAVEFORMATEX format_;
			bool isNormalized;
		public:
			DirectSound_buffer();
			~DirectSound_buffer();

			void Init(HWND hWnd, unsigned totalMilliSeconds);
			void AddNote(int deltaNote, unsigned start, unsigned duration);	// time in milliseconds
			void NormalizeAndPlay();
			void Stop() const;
			void Clear() const;
		private:
			std::unique_ptr<class DirectSound_locker> locker_;
		};
	}
}