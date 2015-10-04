# pragma once

namespace gTest
{
	class CostCommon : public testing::Test
	{
	public:
		int16_t whiteNote, blackNote, randNote1, randNote2, distance;
		char randFinger1, randFinger2, nonThumb;
		const char UNUSED[3] = { '\0', '\0', '\0' };	// three padding bytes

		CostCommon();
		virtual ~CostCommon() override = default;

		virtual void SetUp() override final
		{
			testing::FLAGS_gtest_break_on_failure = true;
		}
		virtual void TearDown() override final {}
	};
}