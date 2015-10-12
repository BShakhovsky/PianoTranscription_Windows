# pragma once

namespace gTest
{
	class TrellisGraph_Common : public testing::Test
	{
	public:
		static constexpr int16_t
			DO = 0, DO_S = 1, RE_B = 1, RE = 2, RE_S = 3, MI_B = 3, MI = 4, FA = 5, FA_S = 6,
			SO_B = 6, SO = 7, SO_S = 8, LA_B = 8, LA = 9, LA_S = 10, SI_B = 10, SI = 11,

			DO_1 = DO + 12, DO_S_1 = DO_S + 12, RE_B_1 = RE_B + 12, RE_1 = RE + 12, RE_S_1 = RE_S + 12,
			MI_B_1 = MI_B + 12, MI_1 = MI + 12, FA_1 = FA + 12, FA_S_1 = FA_S + 12,
			SO_B_1 = SO_B + 12, SO_1 = SO + 12, SO_S_1 = SO_S + 12, LA_B_1 = LA_B + 12,
			LA_1 = LA + 12, LA_S_1 = LA_S + 12, SI_B_1 = SI_B + 12, SI_1 = SI + 12,

			DO_2 = DO_1 + 12, DO_S_2 = DO_S_1 + 12, RE_B_2 = RE_B_1 + 12, RE_2 = RE_1 + 12, RE_S_2 = RE_S_1 + 12,
			MI_B_2 = MI_B_1 + 12, MI_2 = MI_1 + 12, FA_2 = FA_1 + 12, FA_S_2 = FA_S_1 + 12,
			SO_B_2 = SO_B_1 + 12, SO_2 = SO_1 + 12, SO_S_2 = SO_S_1 + 12, LA_B_2 = LA_B_1 + 12,
			LA_2 = LA_1 + 12, LA_S_2 = LA_S_1 + 12, SI_B_2 = SI_B_1 + 12, SI_2 = SI_1 + 12;

		TrellisGraph_Common() : Test() {}
		TrellisGraph_Common(const TrellisGraph_Common&) = default;
		TrellisGraph_Common& operator = (const TrellisGraph_Common&) = default;

		virtual ~TrellisGraph_Common() override = 0 {}

		virtual void SetUp() override = 0
		{
			testing::FLAGS_gtest_break_on_failure = true;
		}
		virtual void TearDown() override = 0 {}
	};
}