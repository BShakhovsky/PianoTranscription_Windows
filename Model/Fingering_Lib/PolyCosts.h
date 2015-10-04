# pragma once
# include "ICostsCalculator.h"

namespace Model
{
	namespace Fingering
	{
		class PolyCosts : public ICostsCalculator
		{
		public:
			PolyCosts() = default;
			virtual ~PolyCosts() override final = default;
		private:
			virtual int CostOfSingle_impl(char) const override final
			{
				assert(!"COST OF SINGLE NOTE IS NO USE FOR POLYPHONIC CHORD");
				return NULL;
			}

			virtual float CostOfPair_impl(std::pair<int16_t, char> note1,
				std::pair<int16_t, char> note2) const override final;

			virtual int CostOfThree_impl(std::pair<int16_t, char>, std::pair<int16_t, char>,
				std::pair<int16_t, char>) const override final
			{
				assert(!"COST OF THREE NOTES IS FOR SOME REASON NOT APPLICABLE"
					&& !"FOR POLYPHONIC CHORD ACCORDING TO CURRENT ALGORITHM");
				return NULL;
			}
		};
	}
}