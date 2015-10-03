# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\DistanceTable.h"

using testing::FLAGS_gtest_break_on_failure;
using Model::Fingering::DistanceTable;

// http://www.diva-portal.org/smash/get/diva2:769592/FULLTEXT01.pdf
// Page 9, Table 1

void TestColumn(char(*func)(char, char), char fin12, char fin13, char fin14, char fin15,
	char fin23, char fin24, char fin25, char fin34, char fin35, char fin45)
{
	srand(static_cast<unsigned>(time(NULL)));
	ASSERT_DEBUG_DEATH(func(1, 1 - rand() % (CHAR_MAX - 2) - 2), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(2, 0), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(3, rand() % (CHAR_MAX - 6) + 6), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(1 - rand() % (CHAR_MAX - 2) - 2, 3), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(0, 4), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(func(rand() % (CHAR_MAX - 6) + 6, 5), "WRONG FINGER");

	ASSERT_DEBUG_DEATH(func(1, 1), "FINGER PAIR MUST CONSIST OF TWO DIFFERENT FINGERS");
	ASSERT_EQ(fin12, func(1, 2)); ASSERT_EQ(fin13, func(1, 3)); ASSERT_EQ(fin14, func(1, 4)); ASSERT_EQ(fin15, func(1, 5));
	ASSERT_EQ(fin12, func(2, 1)); ASSERT_EQ(fin13, func(3, 1)); ASSERT_EQ(fin14, func(4, 1)); ASSERT_EQ(fin15, func(5, 1));

	ASSERT_DEBUG_DEATH(func(2, 2), "FINGER PAIR MUST CONSIST OF TWO DIFFERENT FINGERS");
	ASSERT_EQ(fin23, func(2, 3)); ASSERT_EQ(fin24, func(2, 4)); ASSERT_EQ(fin25, func(2, 5));
	ASSERT_EQ(fin23, func(3, 2)); ASSERT_EQ(fin24, func(4, 2)); ASSERT_EQ(fin25, func(5, 2));

	ASSERT_DEBUG_DEATH(func(3, 3), "FINGER PAIR MUST CONSIST OF TWO DIFFERENT FINGERS");
	ASSERT_EQ(fin34, func(3, 4)); ASSERT_EQ(fin35, func(3, 5));
	ASSERT_EQ(fin34, func(4, 3)); ASSERT_EQ(fin35, func(5, 3));

	ASSERT_DEBUG_DEATH(func(4, 4), "FINGER PAIR MUST CONSIST OF TWO DIFFERENT FINGERS");
	ASSERT_EQ(fin45, func(4, 5)); ASSERT_EQ(fin45, func(5, 4));
	ASSERT_DEBUG_DEATH(func(5, 5), "FINGER PAIR MUST CONSIST OF TWO DIFFERENT FINGERS");
}

TEST(DistanceTable, MinMax)
{
	FLAGS_gtest_break_on_failure = true;
	TestColumn(&DistanceTable::MinPrac, -5, -4, -3, -1, 1, 1, 2, 1, 1, 1);
	TestColumn(&DistanceTable::MaxPrac, 10, 12, 14, 15, 5, 7, 10, 4, 7, 5);
	TestColumn(&DistanceTable::MinRel, 1, 3, 5, 7, 1, 3, 5, 1, 3, 1);
	TestColumn(&DistanceTable::MaxRel, 5, 7, 9, 10, 2, 4, 6, 2, 4, 2);
	TestColumn(&DistanceTable::MinComf, -3, -2, -1, 1, 1, 1, 2, 1, 1, 1);
	TestColumn(&DistanceTable::MaxComf, 8, 10, 12, 13, 3, 5, 8, 2, 5, 3);
}