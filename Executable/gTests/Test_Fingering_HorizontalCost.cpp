# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\HorizontalCost.h"

using namespace std;
using namespace testing;
using Model::Fingering::HorizontalCost;

class HorizontalCost_F : public Test
{
public:
	vector<pair<int16_t, char>> emptyChord;

	HorizontalCost_F() :
		emptyChord()
	{}
	HorizontalCost_F(const HorizontalCost_F&) = default;
	HorizontalCost_F& operator = (const HorizontalCost_F&) = default;
	virtual ~HorizontalCost_F() override = default;

	virtual void SetUp() override final
	{
		FLAGS_gtest_break_on_failure = true;
	}
	virtual void TearDown() override final {}
};

// not finished (just partially tested)

TEST_F(HorizontalCost_F, CostOfSingle)
{
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(emptyChord, emptyChord, emptyChord),
		"THERE MUST BE AT LEAST ONE CHORD TO CALCULATE MOVING COST");
# ifdef NDEBUG
	ASSERT_EQ(NULL, HorizontalCost::Calculate(emptyChord, emptyChord, emptyChord));
# endif
	auto singleChord(emptyChord);
	singleChord.assign(5, make_pair(0i16, '\0'));
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(singleChord, emptyChord, emptyChord), "WRONG FINGER NUMBER");
	singleChord.front().second = '\1';
	singleChord.at(1).second = '\2';
	singleChord.at(2).second = singleChord.at(3).second = singleChord.back().second = '\3';
	ASSERT_EQ(NULL, HorizontalCost::Calculate(singleChord, emptyChord, emptyChord));
	singleChord.at(3).second = '\4';
	ASSERT_EQ(1, HorizontalCost::Calculate(singleChord, emptyChord, emptyChord));
	singleChord.back().second = '\5';
	ASSERT_EQ(2, HorizontalCost::Calculate(singleChord, emptyChord, emptyChord));

	singleChord.front().first = singleChord.at(1).first = singleChord.at(2).first
		= singleChord.at(3).first = singleChord.back().first = 1i16;
	ASSERT_EQ(2, HorizontalCost::Calculate(singleChord, emptyChord, emptyChord));
}

TEST_F(HorizontalCost_F, CostOfPair)
{
	auto chord1(emptyChord), chord2(emptyChord);
	chord2.assign(4, make_pair(1i16, '\0'));
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(chord1, chord2, emptyChord), "WRONG FINGER NUMBER");
	chord2.front().second = '\2';
	chord2.at(1).second = '\3';
	chord2.at(2).second = '\4';
	chord2.back().second = '\5';
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(chord1, chord2, emptyChord),
		"THERE SHOULD BE AT LEAST ONE NOTE IN BOTH CHORDS TO CALCULATE MOVING COST OF PAIR");

	chord1.reserve(2);
	chord1.emplace_back(make_pair(0i16, '\1'));
	ASSERT_EQ(5.25, HorizontalCost::Calculate(chord1, chord2, emptyChord))
		<< "single cost = 2, cost of pair = 2 + 4 + 6 + 1(pinky on black) = 13 / 4 = 3.25";
	chord1.emplace_back(make_pair(0i16, '\3'));
	ASSERT_EQ(8, HorizontalCost::Calculate(chord1, chord2, emptyChord))
		<< "single = again 2, additional pairs = 28 + 0 + 1(three-four) + 1(four on black)"
		<< " + 1(pinky on black again) + 4 = 35, total = 48 / 8 = 6";
}

TEST_F(HorizontalCost_F, CostOfTriple)
{
	auto chord1(emptyChord), chord2(emptyChord), chord3(emptyChord);
	chord3.assign(4, make_pair(1i16, '\0'));
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(chord1, chord2, chord3), "WRONG FINGER NUMBER");
	chord3.front().second = '\2';
	chord3.at(1).second = '\3';
	chord3.at(2).second = '\4';
	chord3.back().second = '\5';
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(chord1, chord2, chord3),
		"THERE SHOULD BE AT LEAST ONE NOTE IN BOTH CHORDS TO CALCULATE MOVING COST OF PAIR");

	chord2.emplace_back(make_pair(0i16, '\1'));
	ASSERT_DEBUG_DEATH(HorizontalCost::Calculate(chord1, chord2, chord3),
		"THERE SHOULD BE AT LEAST ONE NOTE IN ALL THREE CHORDS TO CALCULATE MOVING COST OF TRIPLE");

	chord1.reserve(2);
	chord1.emplace_back(make_pair(0i16, '\1'));
	ASSERT_EQ(5.25, HorizontalCost::Calculate(chord1, chord2, chord3)) << "pair cost = 5.25, triple cost = 0 / 4";
	chord1.emplace_back(make_pair(0i16, '\2'));
	ASSERT_EQ(5.5, HorizontalCost::Calculate(chord1, chord2, chord3)) << "1st pair cost is not calculated"
		<< ", additional triple cost = 0(same finger, but notes order is not strict)"
		<< " + 0 + 0 + 1+1(position count and size) = 2, total triple cost = 2 / 2 / 4 = 0.25";
}