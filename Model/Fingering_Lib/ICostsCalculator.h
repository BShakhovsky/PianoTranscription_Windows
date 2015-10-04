# pragma once

namespace Model
{
	namespace Fingering
	{
		class ICostsCalculator abstract : private boost::noncopyable
		{
		public:
			int CostOfSingle(char finger1) const
			{
				return CostOfSingle_impl(finger1);
			}
			float CostOfPair(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2) const
			{
				return CostOfPair_impl(note1, note2);
			}
			int CostOfThree(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2,
				std::pair<int16_t, char> note3) const
			{
				return CostOfThree_impl(note1, note2, note3);
			}

			virtual ~ICostsCalculator() = 0 {}
		protected:
			ICostsCalculator() = default;
		private:
			virtual int CostOfSingle_impl(char) const = 0
			{
				PURE_VIRTUAL;
			}
			virtual float CostOfPair_impl(std::pair<int16_t, char>, std::pair<int16_t, char>) const = 0
			{
				PURE_VIRTUAL;
			}
			virtual int CostOfThree_impl(std::pair<int16_t, char>, std::pair<int16_t, char>,
				std::pair<int16_t, char>) const = 0
			{
				PURE_VIRTUAL;
			}
		};
	}
}