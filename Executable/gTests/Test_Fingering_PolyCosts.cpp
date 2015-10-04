# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\PolyCosts.h"
# include "Fingering_CostCommon.h"
# include "Fingering_CostTable.h"

using std::make_pair;
using Model::Fingering::PolyCosts;
using namespace gTest;

class PolyCosts_F : public CostCommon
{
public:
	PolyCosts_F() = default;
	virtual ~PolyCosts_F() override = default;

	//	virtual void SetUp() override final {}
	//	virtual void TearDown() override final {}
};

void Test_PolyphonicSum(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2)
{
	static const int polyphonic[][21] =
	{
		{ 20, 14, 8, 6, 4, 2, 0, 0, 0, 0, 0, 2, 4, 6, 12, 18, 34, 50, 66, 82, 98 },
		{ 38, 22, 16, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 2, 4, 6, 12, 18, 34, 50, 66 },
		{ 56, 40, 24, 18, 12, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 2, 4, 6, 12, 18, 34 },
		{ 88, 72, 56, 40, 24, 18, 12, 10, 8, 6, 4, 2, 0, 0, 0, 0, 2, 4, 6, 12, 18 },
		{ 108, 90, 72, 54, 36, 18, 0, 0, 4, 12, 20, 38, 56, 74, 92, 110, 128, 146, 164, 182, 200 },
		{ 116, 98, 80, 62, 44, 26, 8, 4, 0, 0, 4, 12, 20, 38, 56, 74, 92, 110, 128, 146, 164 },
		{ 138, 120, 102, 84, 66, 48, 30, 12, 8, 4, 0, 0, 4, 8, 16, 24, 42, 60, 78, 96, 114 },
		{ 108, 90, 72, 54, 36, 18, 0, 0, 8, 16, 34, 52, 70, 88, 106, 124, 142, 160, 178, 196, 214 },
		{ 116, 98, 80, 62, 44, 26, 8, 4, 0, 0, 4, 12, 20, 38, 56, 74, 92, 110, 128, 146, 164 },
		{ 108, 90, 72, 54, 36, 18, 0, 0, 4, 12, 20, 38, 56, 74, 92, 110, 128, 146, 164, 182, 200 }
	};
# pragma warning(push)
# ifdef NDEBUG
#	pragma warning(disable:4738)	// storing 32-bit float in memory
# endif
	ASSERT_EQ(polyphonic[tableRow][tableCol],
		PolyCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger2)));
	ASSERT_EQ(NULL, PolyCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger1)));
	ASSERT_EQ(NULL, PolyCosts().CostOfPair(make_pair(note1, finger2), make_pair(note2, finger2)));
# pragma warning(pop)
}

TEST_F(PolyCosts_F, CostOfPair)
{
	CostTable::CheckAllTableCells(&Test_PolyphonicSum, randNote1);
}