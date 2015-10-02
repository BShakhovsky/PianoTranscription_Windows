#pragma once

namespace Interface
{
	namespace Sound
	{
		class DirectHarmonics : private boost::noncopyable
		{
			const int
				noteMin_ = 21,	// Lya of second octave, 55 Hz
				noteMax_ = 120;	// Do of last octave, 55 * 2 ^ (8 + 3/12) =~ 14'080 * 1.2 =~ 16'744 Hz < 22'050 Hz

			DirectHarmonics() = delete;
		public:
			explicit DirectHarmonics(HWND, unsigned totalMilliSeconds = 5'000);
			~DirectHarmonics();

			void AddNote(int note, unsigned start = NULL, unsigned duration = 1'500) const;	// time in milliseconds
			void Play() const;
			void Stop() const;
			void Clear() const;
		private:
			const std::unique_ptr<class DirectSound_buffer> buffer_;
		};
	}
}