# pragma once

namespace gTest
{
	class CostCommon abstract : public testing::Test
	{
	protected:
		CostCommon();
	public:
		CostCommon(const CostCommon&) = default;
		CostCommon& operator = (const CostCommon&) = default;

		int16_t whiteNote, blackNote, randNote1, randNote2, distance;
		char randFinger1, randFinger2, nonThumb;
		const char UNUSED[3] = { '\0', '\0', '\0' };	// three padding bytes

		virtual ~CostCommon() override = 0 {}

		virtual void SetUp() override = 0
		{
			testing::FLAGS_gtest_break_on_failure = true;
		}
		virtual void TearDown() override = 0 {}
	};
}