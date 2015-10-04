# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\VerticalCost.h"

using namespace std;
using testing::FLAGS_gtest_break_on_failure;
using Model::Fingering::VerticalCost;

// not finished - only asserts are checked:
TEST(VerticalCost_F, Calculate)
{
	FLAGS_gtest_break_on_failure = true;

	vector<pair<int16_t, char>> chord;
	chord.reserve(6);
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NUMBER OF NOTES IN A CHORD MUST BE BETWEEN TWO AND FIVE");
# endif
	chord.push_back(make_pair(0i16, '\0'));
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NUMBER OF NOTES IN A CHORD MUST BE BETWEEN TWO AND FIVE");
# endif

	chord.push_back(make_pair(0i16, '\0'));
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL NOTES IN A CHORD MUST BE DIFFERENT");
	chord.front().first = 1i16;
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NOTES SHOULD BE SORTED IN ASCENDING ORDER BEFORE CHORD COST CALCULATION");
	chord.back().first = 2i16;
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL FINGERS IN A CHORD MUST BE DIFFERENT");
	chord.back().second = '\2';
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "WRONG FINGER");
	chord.front().second = '\3';
# ifdef NDEBUG
	ASSERT_EQ(36, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ONLY THUMB PASSING IS ALLOWED BY CURRENT ALGORITHM");
	chord.back().second = '\4';
	ASSERT_EQ(NULL, VerticalCost::Calculate(chord));

	chord.push_back(make_pair(1i16, '\4'));
# ifdef NDEBUG
	ASSERT_EQ(NULL, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NOTES SHOULD BE SORTED IN ASCENDING ORDER BEFORE CHORD COST CALCULATION");
	chord.back().first = 2i16;
# ifdef NDEBUG
	ASSERT_EQ(NULL, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL NOTES IN A CHORD MUST BE DIFFERENT");
	chord.back().first = 3i16;
# ifdef NDEBUG
	ASSERT_EQ(NULL, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL FINGERS IN A CHORD MUST BE DIFFERENT");
	chord.back().second = '\6';
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "WRONG FINGER");
	chord.back().second = '\2';
# ifdef NDEBUG
	ASSERT_EQ(44, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ONLY THUMB PASSING IS ALLOWED BY CURRENT ALGORITHM");
	chord.back().second = '\1';
	ASSERT_EQ(12, VerticalCost::Calculate(chord));

	chord.push_back(make_pair(1i16, '\4'));
# ifdef NDEBUG
	ASSERT_EQ(30, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NOTES SHOULD BE SORTED IN ASCENDING ORDER BEFORE CHORD COST CALCULATION");
	chord.back().first = 3i16;
# ifdef NDEBUG
	ASSERT_EQ(22, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL NOTES IN A CHORD MUST BE DIFFERENT");
	chord.back().first = 4i16;
# ifdef NDEBUG
	ASSERT_EQ(20, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL FINGERS IN A CHORD MUST BE DIFFERENT");
	chord.back().second = '\6';
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "WRONG FINGER");
	chord.back().second = '\2';
# ifdef NDEBUG
	ASSERT_EQ(12, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ONLY THUMB PASSING IS ALLOWED BY CURRENT ALGORITHM");
	chord.back().second = '\5';
	ASSERT_EQ(24, VerticalCost::Calculate(chord));
	
	chord.push_back(make_pair(1i16, '\4'));
# ifdef NDEBUG
	ASSERT_EQ(28, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NOTES SHOULD BE SORTED IN ASCENDING ORDER BEFORE CHORD COST CALCULATION");
	chord.back().first = 4i16;
# ifdef NDEBUG
	ASSERT_EQ(42, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL NOTES IN A CHORD MUST BE DIFFERENT");
	chord.back().first = 5i16;
# ifdef NDEBUG
	ASSERT_EQ(60, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ONLY THUMB PASSING IS ALLOWED BY CURRENT ALGORITHM");
	chord.back().second = '\5';
# ifdef NDEBUG
	ASSERT_EQ(24, VerticalCost::Calculate(chord));
# endif
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "ALL FINGERS IN A CHORD MUST BE DIFFERENT");
	chord.back().second = '\6';
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "WRONG FINGER");

	chord.push_back(pair<int16_t, char>());
	ASSERT_DEBUG_DEATH(VerticalCost::Calculate(chord), "NUMBER OF NOTES IN A CHORD MUST BE BETWEEN TWO AND FIVE");
}