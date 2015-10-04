# pragma once
# include "ICostsCalculator.h"

namespace Model
{
	namespace Fingering
	{
		class MonoCosts : public ICostsCalculator
		{
		public:
			MonoCosts() = default;
			virtual ~MonoCosts() override final = default;
		private:
			virtual int CostOfSingle_impl(char finger) const override final;
			virtual float CostOfPair_impl(
				std::pair<int16_t, char> note1,
				std::pair<int16_t, char> note2) const override final;
			virtual int CostOfThree_impl(
				std::pair<int16_t, char> note1,
				std::pair<int16_t, char> note2,
				std::pair<int16_t, char> note3) const override final;
		};
	}
}