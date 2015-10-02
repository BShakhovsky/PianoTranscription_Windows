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
			static int Calculate(std::vector<std::pair<int16_t, char>> chord1,
				std::vector<std::pair<int16_t, char>> chord2,
				const std::vector<std::pair<int16_t, char>>* chord3 = nullptr);
		private:
			static int CalcSingles(const std::vector<std::pair<int16_t, char>>* chord);
			static int CalcPairs(
				const std::vector<std::pair<int16_t, char>>* chord1,
				const std::vector<std::pair<int16_t, char>>* chord2);
			static int CalcTriples(
				const std::vector<std::pair<int16_t, char>>* chord1,
				const std::vector<std::pair<int16_t, char>>* chord2,
				const std::vector<std::pair<int16_t, char>>* chord3);
		};
	}
}