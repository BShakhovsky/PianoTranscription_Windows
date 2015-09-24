#pragma once

namespace Interface
{
	namespace Sound
	{
		class DirectHarmonic abstract : private boost::noncopyable
		{
		public:
			DirectHarmonic() = delete;
			~DirectHarmonic() = delete;

			static void Execute();
		};
	}
}