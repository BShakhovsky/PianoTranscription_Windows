# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\MonoCosts.h"
# include "..\..\Model\MidiParserLib\NoteNames.h"
# include "Fingering_CostCommon.h"
# include "Fingering_CostTable.h"

using std::make_pair;
using Model::Fingering::MonoCosts;
using Model::MidiParser::NoteNames;
using namespace gTest;

class MonoCosts_F : public CostCommon
{
public:
	MonoCosts_F() = default;
	virtual ~MonoCosts_F() override = default;

//	virtual void SetUp() override final {}
//	virtual void TearDown() override final {}
private:
	static const int partSum[][21];
	friend void Test_PartialMonoSum(size_t, size_t, char, char, int16_t, int16_t);
	friend void Test_White_White(size_t, size_t, char, char, int16_t, int16_t);
	friend void Test_White_Black(size_t, size_t, char, char, int16_t, int16_t);
	friend void Test_Black_White(size_t, size_t, char, char, int16_t, int16_t);
	friend void Test_Black_Black(size_t, size_t, char, char, int16_t, int16_t);
};

// includes Rules 1, 2, 14 (as in polyphonic sum), 7 (three-four), 15 (same note)
// not includes 8, 9(float), 10, 11, 12 as they are dependant on key color
const int MonoCosts_F::partSum[][21] =
{
	{ 10, 7, 4, 3, 2, 2, 0, 0, 0, 0, 0, 1, 2, 3, 6, 9, 22, 35, 48, 61, 74 },
	{ 24, 11, 8, 5, 4, 4, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 6, 9, 22, 35, 48 },
	{ 38, 25, 12, 9, 6, 6, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 6, 9, 22 },
	{ 64, 51, 38, 25, 12, 10, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 1, 2, 3, 6, 9 },
	{ 84, 70, 56, 42, 28, 15, 0, 0, 2, 6, 10, 24, 38, 52, 66, 80, 94, 108, 122, 136, 150 },
	{ 88, 74, 60, 46, 32, 19, 4, 2, 0, 0, 2, 6, 10, 24, 38, 52, 66, 80, 94, 108, 122 },
	{ 104, 90, 76, 62, 48, 35, 20, 6, 4, 2, 0, 0, 2, 4, 8, 12, 26, 40, 54, 68, 82 },
	{ 85, 71, 57, 43, 29, 16, 1, 1, 5, 9, 23, 37, 51, 65, 79, 93, 107, 121, 135, 149, 163 },
	{ 88, 74, 60, 46, 32, 19, 4, 2, 0, 0, 2, 6, 10, 24, 38, 52, 66, 80, 94, 108, 122 },
	{ 84, 70, 56, 42, 28, 15, 0, 0, 2, 6, 10, 24, 38, 52, 66, 80, 94, 108, 122, 136, 150 }
};

void Test_PartialMonoSum(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2);

void Test_White_White(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2);
void Test_White_Black(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2);
void Test_Black_White(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2);
void Test_Black_Black(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2);

/************************************************************
CostOfTriples is not checked yet:
Rule3_PositionChange(note1, note2, note3)
Rule4_PositionSize(note1, note3)
Rule6_ThreeFourFive(note1.second, note2.second, note3.second)
Rule13_ThreeSameFinger(note1, note2, note3);
************************************************************/

TEST_F(MonoCosts_F, PartialMonoSum)
{
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(Test_PartialMonoSum(0, 0, 1, 1 - rand() % (CHAR_MAX - 2) - 2, randNote1, randNote2), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(Test_PartialMonoSum(0, 0, 2, NULL,							randNote1, randNote2), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(Test_PartialMonoSum(0, 0, 3, 6,								randNote1, randNote2), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(Test_PartialMonoSum(0, 0, 1 - rand() % (CHAR_MAX - 2) - 2, 3, randNote1, randNote2), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(Test_PartialMonoSum(0, 0, NULL, 4,							randNote1, randNote2), "WRONG FINGER");
	ASSERT_DEBUG_DEATH(Test_PartialMonoSum(0, 0, rand() % (CHAR_MAX - 6) + 6, 5,	randNote1, randNote2), "WRONG FINGER");
# endif
	for (int16_t i(NULL); i <= 5; ++i)
	{
		if (NoteNames::IsBlack(whiteNote - i))
		{
			Test_PartialMonoSum(0, static_cast<size_t>(5 - i), '\1', '\2', whiteNote, whiteNote - i);
			Test_PartialMonoSum(1, static_cast<size_t>(5 - i), '\1', '\3', whiteNote, whiteNote - i);
			Test_PartialMonoSum(2, static_cast<size_t>(5 - i), '\1', '\4', whiteNote, whiteNote - i);
//			Test_PartialMonoSum(3, static_cast<size_t>(5 - i), '\1', '\5', whiteNote, whiteNote - i);

			Test_PartialMonoSum(0, static_cast<size_t>(5 - i), '\2', '\1', whiteNote - i, whiteNote);
			Test_PartialMonoSum(1, static_cast<size_t>(5 - i), '\3', '\1', whiteNote - i, whiteNote);
			Test_PartialMonoSum(2, static_cast<size_t>(5 - i), '\4', '\1', whiteNote - i, whiteNote);
//			Test_PartialMonoSum(3, static_cast<size_t>(5 - i), '\5', '\1', whiteNote - i, whiteNote);
		}
		else
		{
			Test_PartialMonoSum(6, static_cast<size_t>(5 - i), '\2', '\5', whiteNote, whiteNote - i);
			Test_PartialMonoSum(7, static_cast<size_t>(5 - i), '\3', '\4', whiteNote, whiteNote - i);
			Test_PartialMonoSum(8, static_cast<size_t>(5 - i), '\3', '\5', whiteNote, whiteNote - i);
			Test_PartialMonoSum(9, static_cast<size_t>(5 - i), '\4', '\5', whiteNote, whiteNote - i);

			Test_PartialMonoSum(6, static_cast<size_t>(5 - i), '\5', '\2', whiteNote - i, whiteNote);
			Test_PartialMonoSum(7, static_cast<size_t>(5 - i), '\4', '\3', whiteNote - i, whiteNote);
			Test_PartialMonoSum(8, static_cast<size_t>(5 - i), '\5', '\3', whiteNote - i, whiteNote);
			Test_PartialMonoSum(9, static_cast<size_t>(5 - i), '\5', '\4', whiteNote - i, whiteNote);
		}
		
		if (NoteNames::IsWhite(blackNote + i))
		{
			Test_PartialMonoSum(0, static_cast<size_t>(5 - i), '\1', '\2', blackNote + i, blackNote);
			Test_PartialMonoSum(1, static_cast<size_t>(5 - i), '\1', '\3', blackNote + i, blackNote);
			Test_PartialMonoSum(2, static_cast<size_t>(5 - i), '\1', '\4', blackNote + i, blackNote);
//			Test_PartialMonoSum(3, static_cast<size_t>(5 - i), '\1', '\5', blackNote + i, blackNote);

			Test_PartialMonoSum(0, static_cast<size_t>(5 - i), '\2', '\1', blackNote, blackNote + i);
			Test_PartialMonoSum(1, static_cast<size_t>(5 - i), '\3', '\1', blackNote, blackNote + i);
			Test_PartialMonoSum(2, static_cast<size_t>(5 - i), '\4', '\1', blackNote, blackNote + i);
//			Test_PartialMonoSum(3, static_cast<size_t>(5 - i), '\5', '\1', blackNote, blackNote + i);
		}
		else
		{
			Test_PartialMonoSum(6, static_cast<size_t>(5 - i), '\2', '\5', blackNote, blackNote - i);
			Test_PartialMonoSum(7, static_cast<size_t>(5 - i), '\3', '\4', blackNote, blackNote - i);
			Test_PartialMonoSum(8, static_cast<size_t>(5 - i), '\3', '\5', blackNote, blackNote - i);
			Test_PartialMonoSum(9, static_cast<size_t>(5 - i), '\4', '\5', blackNote, blackNote - i);

			Test_PartialMonoSum(6, static_cast<size_t>(5 - i), '\5', '\2', blackNote - i, blackNote);
			Test_PartialMonoSum(7, static_cast<size_t>(5 - i), '\4', '\3', blackNote - i, blackNote);
			Test_PartialMonoSum(8, static_cast<size_t>(5 - i), '\5', '\3', blackNote - i, blackNote);
			Test_PartialMonoSum(9, static_cast<size_t>(5 - i), '\5', '\4', blackNote - i, blackNote);
		}

		Test_PartialMonoSum(4, static_cast<size_t>(5 - i), '\2', '\3', whiteNote, whiteNote - i);
		Test_PartialMonoSum(5, static_cast<size_t>(5 - i), '\2', '\4', whiteNote, whiteNote - i);

		Test_PartialMonoSum(4, static_cast<size_t>(5 - i), '\3', '\2', whiteNote - i, whiteNote);
		Test_PartialMonoSum(5, static_cast<size_t>(5 - i), '\4', '\2', whiteNote - i, whiteNote);
	}

	for (int16_t i(NULL); i <= 15; ++i)
	{
		if (NoteNames::IsWhite(whiteNote - i))
		{
			Test_PartialMonoSum(0, static_cast<size_t>(i + 5), '\1', '\2', whiteNote - i, whiteNote);
			Test_PartialMonoSum(1, static_cast<size_t>(i + 5), '\1', '\3', whiteNote - i, whiteNote);
			Test_PartialMonoSum(2, static_cast<size_t>(i + 5), '\1', '\4', whiteNote - i, whiteNote);
			Test_PartialMonoSum(3, static_cast<size_t>(i + 5), '\1', '\5', whiteNote - i, whiteNote);

			Test_PartialMonoSum(0, static_cast<size_t>(i + 5), '\2', '\1', whiteNote, whiteNote - i);
			Test_PartialMonoSum(1, static_cast<size_t>(i + 5), '\3', '\1', whiteNote, whiteNote - i);
			Test_PartialMonoSum(2, static_cast<size_t>(i + 5), '\4', '\1', whiteNote, whiteNote - i);
			Test_PartialMonoSum(3, static_cast<size_t>(i + 5), '\5', '\1', whiteNote, whiteNote - i);
		}
		else if (NoteNames::IsWhite(blackNote - i))
		{
			Test_PartialMonoSum(0, static_cast<size_t>(i + 5), '\1', '\2', blackNote - i, blackNote);
			Test_PartialMonoSum(1, static_cast<size_t>(i + 5), '\1', '\3', blackNote - i, blackNote);
			Test_PartialMonoSum(2, static_cast<size_t>(i + 5), '\1', '\4', blackNote - i, blackNote);
//			Test_PartialMonoSum(3, static_cast<size_t>(i + 5), '\1', '\5', blackNote - i, blackNote);

			Test_PartialMonoSum(0, static_cast<size_t>(i + 5), '\2', '\1', blackNote, blackNote - i);
			Test_PartialMonoSum(1, static_cast<size_t>(i + 5), '\3', '\1', blackNote, blackNote - i);
			Test_PartialMonoSum(2, static_cast<size_t>(i + 5), '\4', '\1', blackNote, blackNote - i);
//			Test_PartialMonoSum(3, static_cast<size_t>(i + 5), '\5', '\1', blackNote, blackNote - i);
		}

		Test_PartialMonoSum(4, static_cast<size_t>(i + 5), '\2', '\3', whiteNote - i, whiteNote);
		Test_PartialMonoSum(5, static_cast<size_t>(i + 5), '\2', '\4', whiteNote - i, whiteNote);
		Test_PartialMonoSum(6, static_cast<size_t>(i + 5), '\2', '\5', whiteNote - i, whiteNote);
		Test_PartialMonoSum(7, static_cast<size_t>(i + 5), '\3', '\4', whiteNote - i, whiteNote);
		Test_PartialMonoSum(8, static_cast<size_t>(i + 5), '\3', '\5', whiteNote - i, whiteNote);
		Test_PartialMonoSum(9, static_cast<size_t>(i + 5), '\4', '\5', whiteNote - i, whiteNote);

		Test_PartialMonoSum(4, static_cast<size_t>(i + 5), '\3', '\2', whiteNote, whiteNote - i);
		Test_PartialMonoSum(5, static_cast<size_t>(i + 5), '\4', '\2', whiteNote, whiteNote - i);
		Test_PartialMonoSum(6, static_cast<size_t>(i + 5), '\5', '\2', whiteNote, whiteNote - i);
		Test_PartialMonoSum(7, static_cast<size_t>(i + 5), '\4', '\3', whiteNote, whiteNote - i);
		Test_PartialMonoSum(8, static_cast<size_t>(i + 5), '\5', '\3', whiteNote, whiteNote - i);
		Test_PartialMonoSum(9, static_cast<size_t>(i + 5), '\5', '\4', whiteNote, whiteNote - i);
	}
}

# pragma warning(push)
# ifdef NDEBUG
#	pragma warning(disable:4738)	// storing 32-bit float in memory
# endif

void Test_PartialMonoSum(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2)
{
	ASSERT_EQ(MonoCosts_F::partSum[tableRow][tableCol],
		MonoCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger2)));

	const auto nonThumb(MonoCosts_F_PartialMonoSum_Test().nonThumb);
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note1, nonThumb), make_pair(note2, nonThumb)));
}


void Test_White_White(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2)
{
	static const int white_white[][21] =
	{
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
	};
	ASSERT_EQ(white_white[tableRow][tableCol] + MonoCosts_F::partSum[tableRow][tableCol],
		MonoCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger2)));

	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger1)));
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note2, finger2), make_pair(note1, finger2)));
}

void Test_White_Black(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2)
{
	static const int white_black[][21] =
	{
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },	// 100 means impossible combination
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1, 1, 1 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1, 1, 1 },
		{ 1, 1, 1, 1, 1, 100, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 100, 1, 1, 1 }
	};
	ASSERT_EQ(white_black[tableRow][tableCol] + MonoCosts_F::partSum[tableRow][tableCol],
		MonoCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger2)));

	const auto nonThumb(MonoCosts_F_PartialMonoSum_Test().nonThumb);
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note1, nonThumb), make_pair(note2, nonThumb)));
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note2, nonThumb), make_pair(note1, nonThumb)));
}

void Test_Black_White(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2)
{
	static const float black_white[][21] =
	{
		{ 3.5f, 3.5f, 3.5f, 3.5f, 3.5f, 100, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 100, 1.5f, 1.5f, 1.5f },
		{ 3.5f, 3.5f, 3.5f, 3.5f, 3.5f, 100, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 100, 1.5f, 1.5f, 1.5f },
		{ 3.5f, 3.5f, 3.5f, 3.5f, 3.5f, 100, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 100, 1.5f, 1.5f, 1.5f },
		{ 3.5f, 3.5f, 3.5f, 3.5f, 3.5f, 100, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 1.5f, 100, 1.5f, 1.5f, 1.5f },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },	// 100 means impossible combination
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
		{ 0, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 100, 0, 0, 0 },
	};
	ASSERT_EQ(black_white[tableRow][tableCol] + MonoCosts_F::partSum[tableRow][tableCol],
		MonoCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger2)));

	const auto nonThumb(MonoCosts_F_PartialMonoSum_Test().nonThumb);
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note1, nonThumb), make_pair(note2, nonThumb)));
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note2, nonThumb), make_pair(note1, nonThumb)));
}

void Test_Black_Black(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2)
{
	static const float black_black[][21] =
	{
		{ 1.5f, 1.5f, 1.5f, 1.5f, 100, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 100, 0.5f, 0.5f },
		{ 1.5f, 1.5f, 1.5f, 1.5f, 100, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 100, 0.5f, 0.5f },
		{ 1.5f, 1.5f, 1.5f, 1.5f, 100, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 100, 0.5f, 0.5f },
		{ 1.5f, 1.5f, 1.5f, 1.5f, 100, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 0.5f, 0.5f, 0.5f, 100, 0.5f, 100, 0.5f, 0.5f },
		{ 0, 0, 0, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 100, 0, 0 },
		{ 0, 0, 0, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 100, 0, 0 },
		{ 0, 0, 0, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 100, 0, 0 },	// 100 means impossible combination
		{ 0, 0, 0, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 100, 0, 0 },
		{ 0, 0, 0, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 100, 0, 0 },
		{ 0, 0, 0, 0, 100, 0, 100, 0, 0, 0, 0, 100, 0, 0, 0, 0, 100, 0, 100, 0, 0 },
	};
	ASSERT_EQ(black_black[tableRow][tableCol] + MonoCosts_F::partSum[tableRow][tableCol],
		MonoCosts().CostOfPair(make_pair(note1, finger1), make_pair(note2, finger2)));

	const auto nonThumb(MonoCosts_F_PartialMonoSum_Test().nonThumb);
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note1, nonThumb), make_pair(note2, nonThumb)));
	ASSERT_EQ(NULL, MonoCosts().CostOfPair(make_pair(note2, nonThumb), make_pair(note1, nonThumb)));
}

# pragma warning(pop)

TEST_F(MonoCosts_F, CostOfPairs)
{
	int16_t note(NULL);
	for (int16_t i(-5); i <= 15; ++i)
	{
		while (NoteNames::IsBlack(note = rand() % (INT16_MAX - 30) + 15) || NoteNames::IsBlack(note + i));
		CostTable::CheckTableCells(&Test_White_White, note, note + i);
		if (i && i != 12)
		{
			while (NoteNames::IsBlack(note = rand() % (INT16_MAX - 30) + 15) || NoteNames::IsWhite(note + i));
			CostTable::CheckTableCells(&Test_White_Black, note, note + i);
			while (NoteNames::IsWhite(note = rand() % (INT16_MAX - 30) + 15) || NoteNames::IsBlack(note + i));
			CostTable::CheckTableCells(&Test_Black_White, note, note + i);
		}
		if (i != -1 && i != 1 && i != 6 && i != 11 && i != 13)
		{
			while (NoteNames::IsWhite(note = rand() % (INT16_MAX - 30) + 15) || NoteNames::IsWhite(note + i));
			CostTable::CheckTableCells(&Test_Black_Black, note, note + i);
		}
	}
}