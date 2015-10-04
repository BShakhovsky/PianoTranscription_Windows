# pragma once

namespace Model
{
	namespace Fingering
	{
		class HorizontalCost abstract : private boost::noncopyable
		{
			HorizontalCost() = delete;
			~HorizontalCost() = delete;
		public:
			static float Calculate(std::vector<std::pair<int16_t, char>> chord1,
				std::vector<std::pair<int16_t, char>> chord2, std::vector<std::pair<int16_t, char>> chord3);
		private:
			static int CalcSingles(std::vector<std::pair<int16_t, char>> chord);
			static float CalcPairs(
				std::vector<std::pair<int16_t, char>> chord1,
				std::vector<std::pair<int16_t, char>> chord2);
			static float CalcTriples(
				std::vector<std::pair<int16_t, char>> chord1,
				std::vector<std::pair<int16_t, char>> chord2,
				std::vector<std::pair<int16_t, char>> chord3);
		};
	}
}