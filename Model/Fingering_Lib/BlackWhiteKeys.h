# pragma once

namespace Model
{
	namespace Fingering
	{
		class BlackWhiteKeys abstract : private boost::noncopyable
		{
			BlackWhiteKeys() = delete;
			~BlackWhiteKeys() = delete;
		public:
			static bool IsWhite(int16_t noteNumber)
			{
				return !IsBlack(noteNumber);
			}
			static bool IsBlack(int16_t noteNumber);
		};
	}
}