# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\CostRules.h"
# include "..\..\Model\MidiParserLib\NoteNames.h"

using namespace std;
using namespace testing;
using Model::Fingering::CostRules;
using Model::MidiParser::NoteNames;

class CostRules_F : public Test
{
	static const int comfort[10][21];
public:
	int16_t whiteNote, blackNote, randNote1, randNote2, distance;
	char randFinger1, randFinger2, nonThumb;
	const char UNUSED[3] = { '\0', '\0', '\0' };	// three padding bytes

	CostRules_F() :
		whiteNote(NULL),
		blackNote(NULL),
		randNote1(NULL),
		randNote2(NULL),
		distance(NULL),
		randFinger1('\0'),
		randFinger2('\0'),
		nonThumb('\0')
	{
		srand(static_cast<unsigned>(time(NULL)));
		while (NoteNames::IsBlack(whiteNote = static_cast<int16_t>(rand() % (INT16_MAX - 1) + 1)));
		while (NoteNames::IsWhite(blackNote = static_cast<int16_t>(rand() % (INT16_MAX - 1) + 1)));

		randNote1 = static_cast<int16_t>(rand() % (INT16_MAX - 1) + 1);
		randNote2 = static_cast<int16_t>(rand() % (INT16_MAX - 1) + 1);

		randFinger1 = static_cast<char>(rand() % 5 + 1);
		randFinger2 = static_cast<char>(rand() % 5 + 1);
		nonThumb = static_cast<char>(rand() % (CHAR_MAX - 2) + 2);

		distance = randFinger1 > randFinger2 ? randNote1 - randNote2 : randNote2 - randNote1;
	}
	virtual ~CostRules_F() override = default;

	virtual void SetUp() override final
	{
		FLAGS_gtest_break_on_failure = true;
	}
	virtual void TearDown() override final {}

	void TestRule4_MONOPHONIC(size_t tableRow, size_t tableCol, char finger1, char finger3, int16_t note1, int16_t note3) const
	{
		ASSERT_EQ(comfort[tableRow][tableCol] / 2,
			CostRules::Rule4_PositionSize(make_pair(note1, finger1), make_pair(note3, finger3)));
		ASSERT_EQ(NULL, CostRules::Rule4_PositionSize(make_pair(note1, finger1), make_pair(note3, finger1)));
		ASSERT_EQ(NULL, CostRules::Rule4_PositionSize(make_pair(note1, finger3), make_pair(note3, finger3)));
	}

	void TestRule1_Stretch(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2) const
	{
		ASSERT_EQ(comfort[tableRow][tableCol],
			CostRules::Rule1_StretchComf(make_pair(note1, finger1), make_pair(note2, finger2)));
		ASSERT_EQ(NULL, CostRules::Rule1_StretchComf(make_pair(note1, finger1), make_pair(note2, finger1)));
		ASSERT_EQ(NULL, CostRules::Rule1_StretchComf(make_pair(note1, finger2), make_pair(note2, finger2)));
	}
	void TestRule2_Span(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2) const
	{
		static const int relaxed[][21] =
		{
			{ 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 },
			{ 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6, 7, 8 },
			{ 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6 },
			{ 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 1, 2, 3, 4, 5 },
			{ 12, 10, 8, 6, 4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26 },
			{ 16, 14, 12, 10, 8, 6, 4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22 },
			{ 20, 18, 16, 14, 12, 10, 8, 6, 4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18 },
			{ 12, 10, 8, 6, 4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26 },
			{ 16, 14, 12, 10, 8, 6, 4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22 },
			{ 12, 10, 8, 6, 4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26 },
		};
		ASSERT_EQ(relaxed[tableRow][tableCol],
			CostRules::Rule2_SpanRel(make_pair(note1, finger1), make_pair(note2, finger2)));
		ASSERT_EQ(NULL, CostRules::Rule2_SpanRel(make_pair(note1, finger1), make_pair(note2, finger1)));
		ASSERT_EQ(NULL, CostRules::Rule2_SpanRel(make_pair(note1, finger2), make_pair(note2, finger2)));
	}
	void TestRule14_MaxPractical(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2) const
	{
		static const int practical[][21] =
		{
			{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50 },
			{ 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30 },
			{ 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10 },
			{ 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{ 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 },
			{ 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50, 60, 70, 80 },
			{ 70, 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50 },
			{ 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 110 },
			{ 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50, 60, 70, 80 },
			{ 60, 50, 40, 30, 20, 10, 0, 0, 0, 0, 0, 10, 20, 30, 40, 50, 60, 70, 80, 90, 100 }
		};
		ASSERT_EQ(practical[tableRow][tableCol],
			CostRules::Rule14_MaxPractical(make_pair(note1, finger1), make_pair(note2, finger2)));
		ASSERT_EQ(NULL, CostRules::Rule14_MaxPractical(make_pair(note1, finger1), make_pair(note2, finger1)));
		ASSERT_EQ(NULL, CostRules::Rule14_MaxPractical(make_pair(note1, finger2), make_pair(note2, finger2)));
	}

	void Test_PolyphonicSum(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2) const
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

		const auto result(
			CostRules::Rule1_StretchComf(make_pair(note1, finger1), make_pair(note2, finger2)) * 2
			+ CostRules::Rule2_SpanRel(make_pair(note1, finger1), make_pair(note2, finger2)) * 2
			+ CostRules::Rule14_MaxPractical(make_pair(note1, finger1), make_pair(note2, finger2)));
		ASSERT_EQ(polyphonic[tableRow][tableCol], result);
		ASSERT_EQ(NULL, CostRules::Rule14_MaxPractical(make_pair(note1, finger1), make_pair(note2, finger1)));
		ASSERT_EQ(NULL, CostRules::Rule14_MaxPractical(make_pair(note1, finger2), make_pair(note2, finger2)));
	}
	void Test_PartialMonoSum(size_t tableRow, size_t tableCol, char finger1, char finger2, int16_t note1, int16_t note2) const
	{
		static const int partSum[][21] =
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

		const auto result(
			CostRules::Rule1_StretchComf(make_pair(note1, finger1), make_pair(note2, finger2))
			+ CostRules::Rule2_SpanRel(make_pair(note1, finger1), make_pair(note2, finger2))
			+ CostRules::Rule7_ThreeFour(finger1, finger2)
			+ CostRules::Rule14_MaxPractical(make_pair(note1, finger1), make_pair(note2, finger2))
			+ CostRules::Rule15_SameNote(make_pair(note1, finger1), make_pair(note2, finger2)));
			ASSERT_EQ(partSum[tableRow][tableCol], result);
		ASSERT_EQ(NULL, CostRules::Rule14_MaxPractical(make_pair(note1, finger1), make_pair(note2, finger1)));
		ASSERT_EQ(NULL, CostRules::Rule14_MaxPractical(make_pair(note1, finger2), make_pair(note2, finger2)));
	}
};

const int CostRules_F::comfort[10][21] =
{
	{ 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8, 10, 12, 14 },
	{ 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8, 10 },
	{ 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6 },
	{ 12, 10, 8, 6,	4, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 4 },
	{ 12, 10, 8, 6,	4, 2, 0, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 },
	{ 12, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 },
	{ 14, 12, 10, 8, 6, 4, 2, 0, 0, 0, 0, 0, 0, 0, 2, 4, 6, 8, 10, 12, 14 },
	{ 12, 10, 8, 6,	4, 2, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24, 26 },
	{ 12, 10, 8, 6,	4, 2, 0, 0, 0, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 },
	{ 12, 10, 8, 6,	4, 2, 0, 0, 0, 2, 4, 6, 8, 10, 12, 14, 16, 18, 20, 22, 24 }
};

// http://www.scriptiebank.be/sites/default/files/webform/scriptie/balliauw-matteo-2014_0.pdf
// Page 16, Table 2

TEST_F(CostRules_F, PolyphonicRules)
{
	vector<void(CostRules_F::*)(size_t, size_t, char, char, int16_t, int16_t) const> funcs(6);
	funcs.at(0) = &CostRules_F::TestRule4_MONOPHONIC;

	funcs.at(1) = &CostRules_F::TestRule1_Stretch;
	funcs.at(2) = &CostRules_F::TestRule2_Span;
	funcs.at(3) = &CostRules_F::TestRule14_MaxPractical;

	funcs.at(4) = &CostRules_F::Test_PolyphonicSum;
	funcs.at(5) = &CostRules_F::Test_PartialMonoSum;

	for (const auto& func : funcs)
		for (int16_t i(-5); i <= 15; ++i)
		{
# ifdef _DEBUG
			ASSERT_DEBUG_DEATH((this->*func)(0, static_cast<size_t>(i + 5), 1, 1 - rand() % (CHAR_MAX - 2) - 2,
				randNote1, randNote1 + i), "WRONG FINGER");
			ASSERT_DEBUG_DEATH((this->*func)(0, static_cast<size_t>(i + 5), 2, 0,
				randNote1, randNote1 + i), "WRONG FINGER");
			ASSERT_DEBUG_DEATH((this->*func)(0, static_cast<size_t>(i + 5), 3, 6,
				randNote1, randNote1 + i), "WRONG FINGER");
			ASSERT_DEBUG_DEATH((this->*func)(0, static_cast<size_t>(i + 5), 1 - rand() % (CHAR_MAX - 2) - 2, 3,
				randNote1, randNote1 + i), "WRONG FINGER");
			ASSERT_DEBUG_DEATH((this->*func)(0, static_cast<size_t>(i + 5), 0, 4,
				randNote1, randNote1 + i), "WRONG FINGER");
			ASSERT_DEBUG_DEATH((this->*func)(0, static_cast<size_t>(i + 5), rand() % (CHAR_MAX - 6) + 6, 5,
				randNote1, randNote1 + i), "WRONG FINGER");
# endif
			(this->*func)(0, static_cast<size_t>(i + 5), '\1', '\2', randNote1, randNote1 + i);
			(this->*func)(1, static_cast<size_t>(i + 5), '\1', '\3', randNote1, randNote1 + i);
			(this->*func)(2, static_cast<size_t>(i + 5), '\1', '\4', randNote1, randNote1 + i);
			(this->*func)(3, static_cast<size_t>(i + 5), '\1', '\5', randNote1, randNote1 + i);
			(this->*func)(4, static_cast<size_t>(i + 5), '\2', '\3', randNote1, randNote1 + i);
			(this->*func)(5, static_cast<size_t>(i + 5), '\2', '\4', randNote1, randNote1 + i);
			(this->*func)(6, static_cast<size_t>(i + 5), '\2', '\5', randNote1, randNote1 + i);
			(this->*func)(7, static_cast<size_t>(i + 5), '\3', '\4', randNote1, randNote1 + i);
			(this->*func)(8, static_cast<size_t>(i + 5), '\3', '\5', randNote1, randNote1 + i);
			(this->*func)(9, static_cast<size_t>(i + 5), '\4', '\5', randNote1, randNote1 + i);

			(this->*func)(0, static_cast<size_t>(i + 5), '\2', '\1', randNote1 + i, randNote1);
			(this->*func)(1, static_cast<size_t>(i + 5), '\3', '\1', randNote1 + i, randNote1);
			(this->*func)(2, static_cast<size_t>(i + 5), '\4', '\1', randNote1 + i, randNote1);
			(this->*func)(3, static_cast<size_t>(i + 5), '\5', '\1', randNote1 + i, randNote1);
			(this->*func)(4, static_cast<size_t>(i + 5), '\3', '\2', randNote1 + i, randNote1);
			(this->*func)(5, static_cast<size_t>(i + 5), '\4', '\2', randNote1 + i, randNote1);
			(this->*func)(6, static_cast<size_t>(i + 5), '\5', '\2', randNote1 + i, randNote1);
			(this->*func)(7, static_cast<size_t>(i + 5), '\4', '\3', randNote1 + i, randNote1);
			(this->*func)(8, static_cast<size_t>(i + 5), '\5', '\3', randNote1 + i, randNote1);
			(this->*func)(9, static_cast<size_t>(i + 5), '\5', '\4', randNote1 + i, randNote1);
		}
}

// Rule 3 Position Change Count IS NOT FINISHED YET:
TEST_F(CostRules_F, RulesForTriples)
{
	// Rule 3 Position Change Count (NOT FINISHED):
	ASSERT_EQ(NULL, CostRules::Rule3_PositionChange(
		make_pair(randNote1, randFinger1), make_pair(randNote1, randFinger2), make_pair(randNote2, randFinger1)));
	ASSERT_EQ(NULL, CostRules::Rule3_PositionChange(
		make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger1), make_pair(randNote1, randFinger2)));
	ASSERT_EQ(NULL, CostRules::Rule3_PositionChange(
		make_pair(randNote2, randFinger1), make_pair(randNote2, randFinger2), make_pair(randNote1, randFinger2)));

	ASSERT_LE(1, CostRules::Rule3_PositionChange(
		make_pair(randNote1, '\2'), make_pair(randNote2, '\3'), make_pair(randNote1, '\4')));

	// Rule 6 Middle-Ring-Pinky:
	// http://www.csc.kth.se/utbildning/kth/kurser/DD143X/dkand13/Group7Anders/final/Bassam.Alfarhan.David.Sandberg.report.pdf
	// Page 6
	ASSERT_EQ(1, CostRules::Rule6_ThreeFourFive('\3', '\4', '\5'));
	ASSERT_EQ(1, CostRules::Rule6_ThreeFourFive('\4', '\5', '\3'));
	ASSERT_EQ(1, CostRules::Rule6_ThreeFourFive('\5', '\3', '\4'));
	ASSERT_EQ(1, CostRules::Rule6_ThreeFourFive('\5', '\4', '\3'));
	ASSERT_EQ(1, CostRules::Rule6_ThreeFourFive('\4', '\3', '\5'));
	ASSERT_EQ(1, CostRules::Rule6_ThreeFourFive('\3', '\5', '\4'));

	const auto result(CostRules::Rule6_ThreeFourFive(randFinger1, randFinger2, nonThumb));
	if (randFinger1 != 3 && randFinger1 != 4 && randFinger1 != 5 ||
		randFinger2 != 3 && randFinger2 != 4 && randFinger2 != 5
		|| nonThumb != 3 && nonThumb != 4 && nonThumb != 5 ||
		randFinger1 == randFinger2 || randFinger2 == nonThumb || nonThumb == randFinger1)	ASSERT_EQ(NULL, result);
	else																					ASSERT_EQ(1, result);

	// Rule 13 Three Same Fingers
	ASSERT_EQ(NULL, CostRules::Rule13_ThreeSameFinger(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger2),
		make_pair(randNote2, !randFinger1))) << "Different 1st and 3rd fingers";
	const auto
		noteMin(min(min(randNote1, randNote2), min(whiteNote, blackNote))),
		noteMax(max(max(randNote1, randNote2), max(whiteNote, blackNote))),
		noteAvg(static_cast<int16_t>((noteMin + noteMax) / 2));
	const auto result2(noteMin < noteAvg ? 1 : NULL);
	ASSERT_EQ(result2, CostRules::Rule13_ThreeSameFinger(make_pair(noteMin, randFinger1), make_pair(noteAvg, randFinger2),
		make_pair(noteMax, randFinger1)));
	ASSERT_EQ(result2, CostRules::Rule13_ThreeSameFinger(make_pair(noteMax, randFinger2), make_pair(noteAvg, randFinger1),
		make_pair(noteMin, randFinger2)));
	ASSERT_EQ(NULL, CostRules::Rule13_ThreeSameFinger(make_pair(noteAvg, randFinger1), make_pair(noteMin, randFinger2),
		make_pair(noteMax, randFinger1)));
	ASSERT_EQ(NULL, CostRules::Rule13_ThreeSameFinger(make_pair(noteMin, randFinger2), make_pair(noteMax, randFinger1),
		make_pair(noteAvg, randFinger2)));
}

TEST_F(CostRules_F, Rule5_WeakFinger)
{
	ASSERT_DEBUG_DEATH(CostRules::Rule5_WeakFinger(1 - rand() % (CHAR_MAX - 2) - 2), "WRONG FINGER NUMBER");
	ASSERT_DEBUG_DEATH(CostRules::Rule5_WeakFinger(NULL), "WRONG FINGER NUMBER");

	ASSERT_EQ(NULL, CostRules::Rule5_WeakFinger(1));
	ASSERT_EQ(NULL, CostRules::Rule5_WeakFinger(2));
	ASSERT_EQ(NULL, CostRules::Rule5_WeakFinger(3));
	ASSERT_EQ(1, CostRules::Rule5_WeakFinger(4));
	ASSERT_EQ(1, CostRules::Rule5_WeakFinger(5));

	ASSERT_DEBUG_DEATH(CostRules::Rule5_WeakFinger(rand() % (CHAR_MAX - 6) + 6), "WRONG FINGER NUMBER");
}


TEST_F(CostRules_F, Rule7_ThreeFour)
{
	ASSERT_EQ(1, CostRules::Rule7_ThreeFour(3, 4));
	ASSERT_EQ(1, CostRules::Rule7_ThreeFour(4, 3));

	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(3, 1 - rand() % (CHAR_MAX - 2) - 2));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(1 - rand() % (CHAR_MAX - 2) - 2, 4));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(4, NULL));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(NULL, 3));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(4, rand() % (CHAR_MAX - 6) + 6));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(rand() % (CHAR_MAX - 6) + 6, 3));

	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(3, 1));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(1, 4));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(3, 2));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(2, 4));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(3, 5));
	ASSERT_EQ(NULL, CostRules::Rule7_ThreeFour(5, 4));

	const auto result(CostRules::Rule7_ThreeFour(randFinger1, randFinger2));
	if (3 == randFinger1 && 4 == randFinger2 || 4 == randFinger1 && 3 == randFinger2)	ASSERT_EQ(1, result);
	else																				ASSERT_EQ(NULL, result);
}

TEST_F(CostRules_F, Rule8_FourOnBlack)
{
	ASSERT_EQ(1, CostRules::Rule8_FourOnBlack(make_pair(whiteNote, '\3'), make_pair(blackNote, '\4')));
	ASSERT_EQ(1, CostRules::Rule8_FourOnBlack(make_pair(blackNote, '\4'), make_pair(whiteNote, '\3')));

	ASSERT_EQ(NULL, CostRules::Rule8_FourOnBlack(make_pair(whiteNote, '\5'), make_pair(blackNote, '\4')));
	ASSERT_EQ(NULL, CostRules::Rule8_FourOnBlack(make_pair(blackNote, '\4'), make_pair(whiteNote, '\5')));
	ASSERT_EQ(NULL, CostRules::Rule8_FourOnBlack(make_pair(blackNote, '\3'), make_pair(blackNote, '\4')));
	ASSERT_EQ(NULL, CostRules::Rule8_FourOnBlack(make_pair(blackNote, '\4'), make_pair(blackNote, '\3')));
	ASSERT_EQ(NULL, CostRules::Rule8_FourOnBlack(make_pair(whiteNote, '\3'), make_pair(whiteNote, '\4')));
	ASSERT_EQ(NULL, CostRules::Rule8_FourOnBlack(make_pair(whiteNote, '\4'), make_pair(whiteNote, '\3')));

	const auto result(CostRules::Rule8_FourOnBlack(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger2)));
	if		(3 == randFinger1 && 4 == randFinger2
		&& NoteNames::IsWhite(randNote1) && NoteNames::IsBlack(randNote2))	ASSERT_EQ(1, result);
	else if (4 == randFinger1 && 3 == randFinger2
		&& NoteNames::IsBlack(randNote1) && NoteNames::IsWhite(randNote2))	ASSERT_EQ(1, result);
	else																	ASSERT_EQ(NULL, result);
}

TEST_F(CostRules_F, Rule9_ThumbOnBlack)
{
	ASSERT_EQ(1.5, CostRules::Rule9_ThumbOnBlack(make_pair(whiteNote, nonThumb), make_pair(blackNote, '\1')));
	ASSERT_EQ(1.5, CostRules::Rule9_ThumbOnBlack(make_pair(blackNote, '\1'), make_pair(whiteNote, nonThumb)));

	ASSERT_EQ(0.5, CostRules::Rule9_ThumbOnBlack(make_pair(blackNote, nonThumb), make_pair(blackNote, '\1')));
	ASSERT_EQ(0.5, CostRules::Rule9_ThumbOnBlack(make_pair(blackNote, '\1'), make_pair(blackNote, nonThumb)));
	ASSERT_EQ(0.5, CostRules::Rule9_ThumbOnBlack(make_pair(whiteNote, '\1'), make_pair(blackNote, '\1')));
	ASSERT_EQ(0.5, CostRules::Rule9_ThumbOnBlack(make_pair(blackNote, '\1'), make_pair(whiteNote, '\1')));

	ASSERT_EQ(NULL, CostRules::Rule9_ThumbOnBlack(make_pair(whiteNote, '\1'), make_pair(whiteNote, nonThumb)));
	ASSERT_EQ(NULL, CostRules::Rule9_ThumbOnBlack(make_pair(whiteNote, nonThumb), make_pair(whiteNote, '\1')));
	ASSERT_EQ(NULL, CostRules::Rule9_ThumbOnBlack(make_pair(whiteNote, '\1'), make_pair(blackNote, nonThumb)));
	ASSERT_EQ(NULL, CostRules::Rule9_ThumbOnBlack(make_pair(blackNote, nonThumb), make_pair(whiteNote, '\1')));
	ASSERT_EQ(NULL, CostRules::Rule9_ThumbOnBlack(make_pair(whiteNote, '\1'), make_pair(whiteNote, '\1')));

	const auto result(CostRules::Rule9_ThumbOnBlack(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger2)));
	if		(1 == randFinger1 && NoteNames::IsBlack(randNote1))
		if	(1 != randFinger2 && NoteNames::IsWhite(randNote2))	ASSERT_EQ(1.5, result);
		else													ASSERT_EQ(0.5, result);
	else if	(1 == randFinger2 && NoteNames::IsBlack(randNote2))
		if	(1 != randFinger1 && NoteNames::IsWhite(randNote1))	ASSERT_EQ(1.5, result);
		else													ASSERT_EQ(0.5, result);
	else														ASSERT_EQ(NULL, result);
}

TEST_F(CostRules_F, Rule10_PinkyOnBlack)
{
	const char nonPinky(static_cast<char>(4 - rand() % CHAR_MAX));
	ASSERT_EQ(1, CostRules::Rule10_PinkyOnBlack(make_pair(whiteNote, nonPinky), make_pair(blackNote, '\5')));
	ASSERT_EQ(1, CostRules::Rule10_PinkyOnBlack(make_pair(blackNote, '\5'), make_pair(whiteNote, nonPinky)));

	ASSERT_EQ(NULL, CostRules::Rule10_PinkyOnBlack(make_pair(whiteNote, nonPinky), make_pair(whiteNote, '\5')));
	ASSERT_EQ(NULL, CostRules::Rule10_PinkyOnBlack(make_pair(whiteNote, '\5'), make_pair(whiteNote, nonPinky)));
	ASSERT_EQ(NULL, CostRules::Rule10_PinkyOnBlack(make_pair(blackNote, nonPinky), make_pair(blackNote, '\5')));
	ASSERT_EQ(NULL, CostRules::Rule10_PinkyOnBlack(make_pair(blackNote, '\5'), make_pair(blackNote, nonPinky)));
	ASSERT_EQ(NULL, CostRules::Rule10_PinkyOnBlack(make_pair(whiteNote, '\5'), make_pair(whiteNote, '\5')));
	ASSERT_EQ(NULL, CostRules::Rule10_PinkyOnBlack(make_pair(blackNote, '\5'), make_pair(blackNote, '\5')));

	const auto result(CostRules::Rule10_PinkyOnBlack(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger2)));
	if (5 == randFinger1 && NoteNames::IsBlack(randNote1)
		&& 5 != randFinger2 && NoteNames::IsWhite(randNote2))	ASSERT_EQ(1, result);
	else if (5 == randFinger2 && NoteNames::IsBlack(randNote2)
		&& 5 != randFinger1 && NoteNames::IsWhite(randNote1))	ASSERT_EQ(1, result);
	else														ASSERT_EQ(NULL, result);
}


TEST_F(CostRules_F, Rule11_ThumbPassing)
{
	int16_t anotherWhite(NULL), anotherBlack(NULL);
	while (NoteNames::IsBlack(anotherWhite = static_cast<int16_t>(rand() % (INT16_MAX - 1) + 1))
		|| anotherWhite == whiteNote);
	while (NoteNames::IsWhite(anotherBlack = static_cast<int16_t>(rand() % (INT16_MAX - 1) + 1))
		|| anotherBlack == blackNote);

	ASSERT_EQ(1, CostRules::Rule11_ThumbPassing(make_pair(min(whiteNote, anotherWhite), nonThumb),
		make_pair(max(whiteNote, anotherWhite), '\1')));
	ASSERT_EQ(1, CostRules::Rule11_ThumbPassing(make_pair(max(whiteNote, anotherWhite), '\1'),
		make_pair(min(whiteNote, anotherWhite), nonThumb)));
	ASSERT_EQ(1, CostRules::Rule11_ThumbPassing(make_pair(min(blackNote, anotherBlack), nonThumb),
		make_pair(max(blackNote, anotherBlack), '\1')));
	ASSERT_EQ(1, CostRules::Rule11_ThumbPassing(make_pair(max(blackNote, anotherBlack), '\1'),
		make_pair(min(blackNote, anotherBlack), nonThumb)));

	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(max(whiteNote, anotherWhite), nonThumb),
		make_pair(min(whiteNote, anotherWhite), '\1')));
	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(min(whiteNote, anotherWhite), '\1'),
		make_pair(max(whiteNote, anotherWhite), nonThumb)));
	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(max(blackNote, anotherBlack), nonThumb),
		make_pair(min(blackNote, anotherBlack), '\1')));
	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(min(blackNote, anotherBlack), '\1'),
		make_pair(max(blackNote, anotherBlack), nonThumb)));

	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(randNote1, randFinger1), make_pair(randNote1, randFinger2)))
		<< "The same note";
	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger1)))
		<< "The same finger";
	ASSERT_EQ(NULL, CostRules::Rule11_ThumbPassing(make_pair(randNote1, '\1'), make_pair(randNote2, '\1')));

	const auto result(CostRules::Rule11_ThumbPassing(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger2)));
	if (distance < 0 && (1 == randFinger1 || 1 == randFinger2) && (
		NoteNames::IsWhite(randNote1) && NoteNames::IsWhite(randNote2) ||
		NoteNames::IsBlack(randNote1) && NoteNames::IsBlack(randNote2)))	ASSERT_EQ(1, result);
	else																	ASSERT_EQ(NULL, result);
}

TEST_F(CostRules_F, Rule12_ThumbCross_Black)
{
	auto result1(blackNote > whiteNote ? 1 : NULL);
	ASSERT_EQ(result1, CostRules::Rule12_ThumbCross_Black(make_pair(whiteNote, nonThumb), make_pair(blackNote, '\1')));
	ASSERT_EQ(result1, CostRules::Rule12_ThumbCross_Black(make_pair(blackNote, '\1'), make_pair(whiteNote, nonThumb)));

	ASSERT_EQ(NULL, CostRules::Rule12_ThumbCross_Black(make_pair(blackNote, '\1'),
		make_pair(blackNote, nonThumb))) << "The same note";
	ASSERT_EQ(NULL, CostRules::Rule12_ThumbCross_Black(make_pair(randNote1, randFinger1),
		make_pair(randNote2, randFinger1))) << "The same finger";
	ASSERT_EQ(NULL, CostRules::Rule12_ThumbCross_Black(make_pair(randNote1, '\1'), make_pair(randNote2, '\1')));
	ASSERT_EQ(NULL, CostRules::Rule12_ThumbCross_Black(make_pair(blackNote, '\1'), make_pair(whiteNote, '\1')));

	const auto result(CostRules::Rule12_ThumbCross_Black(
		make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger2)));
	if (distance < 0 && randFinger1 != randFinger2)
		if		(1 == randFinger1 && NoteNames::IsBlack(randNote1) && NoteNames::IsWhite(randNote2)) ASSERT_EQ(1, result);
		else if (1 == randFinger2 && NoteNames::IsWhite(randNote1) && NoteNames::IsBlack(randNote2)) ASSERT_EQ(1, result);
		else																					ASSERT_EQ(NULL, result);
	else																						ASSERT_EQ(NULL, result);
}


TEST_F(CostRules_F, Rule15_SameNote)
{
	auto result(randFinger1 == randFinger2 ? NULL : 1);
	ASSERT_EQ(result, CostRules::Rule15_SameNote(make_pair(randNote1, randFinger1), make_pair(randNote1, randFinger2)));
	ASSERT_EQ(result, CostRules::Rule15_SameNote(make_pair(randNote2, randFinger2), make_pair(randNote2, randFinger1)));

	ASSERT_EQ(NULL, CostRules::Rule15_SameNote(make_pair(randNote1, randFinger1), make_pair(randNote1, randFinger1)));
	ASSERT_EQ(NULL, CostRules::Rule15_SameNote(make_pair(randNote2, randFinger2), make_pair(randNote2, randFinger2)));

	ASSERT_EQ(NULL, CostRules::Rule15_SameNote(make_pair(randNote1, randFinger1), make_pair(randNote2, randFinger1)));
	ASSERT_EQ(NULL, CostRules::Rule15_SameNote(make_pair(randNote2, randFinger2), make_pair(randNote1, randFinger2)));
}