# pragma once

namespace Model
{
	namespace Fingering
	{
		class VerticalCost abstract : private boost::noncopyable
		{
			VerticalCost() = delete;
			~VerticalCost() = delete;
		public:
			static float Calculate(std::vector<std::pair<int16_t, char>> notes_fingers);
		};
	}
}