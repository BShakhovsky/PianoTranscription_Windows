# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\ChordFingVariations.h"

using namespace std;
using namespace testing;
using namespace Model::Fingering;
using GraphStruct::NodeList_;

class ChordFingVariations_F : public Test
{
public:
	NodeList_ result_;

	ChordFingVariations_F() : Test(),
		result_()
	{}
	virtual ~ChordFingVariations_F() override = default;

	virtual void SetUp() override final
	{
		FLAGS_gtest_break_on_failure = true;
	}
	virtual void TearDown() override final {}

	void CalcResult(vector<int16_t> notes, size_t resultSize)
	{
		result_ = ChordFingVariations::CreateCombinations(notes);
		ASSERT_EQ(resultSize, result_.size());
		for (const auto& subResult : result_) ASSERT_EQ(notes.size(), subResult->first.size());
	}
};

TEST_F(ChordFingVariations_F, CreateCombinations_Empty)
{
	ASSERT_DEBUG_DEATH(ChordFingVariations::CreateCombinations(vector<int16_t>()), "WRONG NUMBER OF FINGERS");
}

TEST_F(ChordFingVariations_F, CreateCombinations_One)
{
	CalcResult({ 1i16 }, 5);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.front()->first.front());
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(1)->first.front());
	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(2)->first.front());
	ASSERT_EQ(make_pair(1i16, '\4'), result_.at(3)->first.front());
	ASSERT_EQ(make_pair(1i16, '\5'), result_.back()->first.front());

	ASSERT_EQ(NULL, result_.front()->second);
	ASSERT_EQ(NULL, result_.at(1)->second);
	ASSERT_EQ(NULL, result_.at(2)->second);
	ASSERT_EQ(NULL, result_.at(3)->second);
	ASSERT_EQ(NULL, result_.back()->second);
}

TEST_F(ChordFingVariations_F, CreateCombinations_Two)
{
	CalcResult({ 1i16, 2i16 }, 14);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.front()->first.back());
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(1)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(1)->first.back());
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(2)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(2)->first.back());
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(3)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(3)->first.back());
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(4)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(4)->first.back());
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(5)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(5)->first.back());
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(6)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(6)->first.back());
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(7)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(7)->first.back());
	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(8)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(8)->first.back());
	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(9)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(9)->first.back());
	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(10)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(10)->first.back());
	ASSERT_EQ(make_pair(1i16, '\4'), result_.at(11)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(11)->first.back());
	ASSERT_EQ(make_pair(1i16, '\4'), result_.at(12)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(12)->first.back());
	ASSERT_EQ(make_pair(1i16, '\5'), result_.back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.back()->first.back());

	ASSERT_EQ(NULL, result_.front()->second);
	ASSERT_EQ(4, result_.at(1)->second);
	ASSERT_EQ(8, result_.at(2)->second);
	ASSERT_EQ(12, result_.at(3)->second);
	ASSERT_EQ(4, result_.at(4)->second);
	ASSERT_EQ(NULL, result_.at(5)->second);
	ASSERT_EQ(8, result_.at(6)->second);
	ASSERT_EQ(30, result_.at(7)->second) << "beyond comfort and practical limits as well";
	ASSERT_EQ(8, result_.at(8)->second);
	ASSERT_EQ(NULL, result_.at(9)->second);
	ASSERT_EQ(8, result_.at(10)->second);
	ASSERT_EQ(12, result_.at(11)->second);
	ASSERT_EQ(NULL, result_.at(12)->second);
	ASSERT_EQ(24, result_.back()->second) << "comfort limit is also exceeded";
}

TEST_F(ChordFingVariations_F, CreateCombinations_Three)
{
	CalcResult({ 1i16, 2i16, 3i16 }, 22);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.front()->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.front()->first.back());
	ASSERT_EQ(NULL, result_.front()->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(1)->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.at(1)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(1)->first.back());
	ASSERT_EQ(8, result_.at(1)->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(2)->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.at(2)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(2)->first.back());
	ASSERT_EQ(30, result_.at(2)->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(3)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(3)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(3)->first.back());
	ASSERT_EQ(4, result_.at(3)->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(4)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(4)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(4)->first.back());
	ASSERT_EQ(12, result_.at(4)->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(5)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(5)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(5)->first.back());
	ASSERT_EQ(8, result_.at(5)->second);


	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(6)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(6)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(6)->first.back());
	ASSERT_EQ(8, result_.at(6)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(7)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(7)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(7)->first.back());
	ASSERT_EQ(20, result_.at(7)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(8)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(8)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(8)->first.back());
	ASSERT_EQ(54, result_.at(8)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(9)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(9)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.at(9)->first.back());
	ASSERT_EQ(8, result_.at(9)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(10)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(10)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(10)->first.back());
	ASSERT_EQ(12, result_.at(10)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(11)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(11)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(11)->first.back());
	ASSERT_EQ(16, result_.at(11)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(12)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(12)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(12)->first.back());
	ASSERT_EQ(NULL, result_.at(12)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(13)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(13)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(13)->first.back());
	ASSERT_EQ(8, result_.at(13)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(14)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(14)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(14)->first.back());
	ASSERT_EQ(8, result_.at(14)->second);


	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(15)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(15)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(15)->first.back());
	ASSERT_EQ(12, result_.at(15)->second);
	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(16)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(16)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(16)->first.back());
	ASSERT_EQ(32, result_.at(16)->second);

	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(17)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(17)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(17)->first.back());
	ASSERT_EQ(16, result_.at(17)->second);
	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(18)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(18)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(18)->first.back());
	ASSERT_EQ(20, result_.at(18)->second);

	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(19)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(19)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(19)->first.back());
	ASSERT_EQ(NULL, result_.at(19)->second);


	ASSERT_EQ(make_pair(1i16, '\4'), result_.at(20)->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), result_.at(20)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(20)->first.back());
	ASSERT_EQ(24, result_.at(20)->second);
	ASSERT_EQ(make_pair(1i16, '\4'), result_.back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.back()->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.back()->first.back());
	ASSERT_EQ(24, result_.back()->second);
}

TEST_F(ChordFingVariations_F, CreateCombinations_Four)
{
	CalcResult({ 1i16, 2i16, 3i16, 4i16 }, 17);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.front()->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.front()->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\4'), result_.front()->first.back());
	ASSERT_EQ(NULL, result_.front()->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(1)->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.at(1)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.at(1)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(1)->first.back());
	ASSERT_EQ(8, result_.at(1)->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(2)->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.at(2)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(2)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(2)->first.back());
	ASSERT_EQ(8, result_.at(2)->second);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.at(3)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(3)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(3)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(3)->first.back());
	ASSERT_EQ(4, result_.at(3)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(4)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(4)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(4)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(4)->first.back());
	ASSERT_EQ(NULL, result_.at(4)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(5)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(5)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.at(5)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\4'), result_.at(5)->first.back());
	ASSERT_EQ(8, result_.at(5)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(6)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(6)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.at(6)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(6)->first.back());
	ASSERT_EQ(16, result_.at(6)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(7)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(7)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(7)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(7)->first.back());
	ASSERT_EQ(12, result_.at(7)->second);

	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(8)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(8)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(8)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(8)->first.back());
	ASSERT_EQ(16, result_.at(8)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(9)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(9)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(9)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\4'), result_.at(9)->first.back());
	ASSERT_EQ(16, result_.at(9)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(10)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(10)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(10)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(10)->first.back());
	ASSERT_EQ(20, result_.at(10)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(11)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(11)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(11)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(11)->first.back());
	ASSERT_EQ(32, result_.at(11)->second);

	ASSERT_EQ(make_pair(1i16, '\3'), result_.at(12)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(12)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(12)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.at(12)->first.back());
	ASSERT_EQ(24, result_.at(12)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(13)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(13)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(13)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\1'), result_.at(13)->first.back());
	ASSERT_EQ(12, result_.at(13)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(14)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(14)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(14)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\1'), result_.at(14)->first.back());
	ASSERT_EQ(32, result_.at(14)->second);
	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(15)->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.at(15)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.at(15)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\1'), result_.at(15)->first.back());
	ASSERT_EQ(32, result_.at(15)->second);

	ASSERT_EQ(make_pair(1i16, '\3'), result_.back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), result_.back()->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\5'), result_.back()->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\1'), result_.back()->first.back());
	ASSERT_EQ(24, result_.back()->second);
}

TEST_F(ChordFingVariations_F, CreateCombinations_Five)
{
	CalcResult({ 1i16, 2i16, 3i16, 4i16, 5i16 }, 5);
	ASSERT_EQ(make_pair(1i16, '\1'), result_.front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), result_.front()->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.front()->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\4'), result_.front()->first.at(3));
	ASSERT_EQ(make_pair(5i16, '\5'), result_.front()->first.back());

	ASSERT_EQ(NULL, result_.front()->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(1)->first.front());
	ASSERT_EQ(make_pair(2i16, '\1'), result_.at(1)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\3'), result_.at(1)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\4'), result_.at(1)->first.at(3));
	ASSERT_EQ(make_pair(5i16, '\5'), result_.at(1)->first.back());

	ASSERT_EQ(8, result_.at(1)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(2)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(2)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\1'), result_.at(2)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\4'), result_.at(2)->first.at(3));
	ASSERT_EQ(make_pair(5i16, '\5'), result_.at(2)->first.back());

	ASSERT_EQ(16, result_.at(2)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.at(3)->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.at(3)->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.at(3)->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\1'), result_.at(3)->first.at(3));
	ASSERT_EQ(make_pair(5i16, '\5'), result_.at(3)->first.back());

	ASSERT_EQ(24, result_.at(3)->second);

	ASSERT_EQ(make_pair(1i16, '\2'), result_.back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), result_.back()->first.at(1));
	ASSERT_EQ(make_pair(3i16, '\4'), result_.back()->first.at(2));
	ASSERT_EQ(make_pair(4i16, '\5'), result_.back()->first.at(3));
	ASSERT_EQ(make_pair(5i16, '\1'), result_.back()->first.back());

	ASSERT_EQ(24, result_.back()->second);
}

TEST_F(ChordFingVariations_F, CreateCombinations_Six)
{
# ifdef NDEBUG
	try
	{
# endif
		ASSERT_DEBUG_DEATH(ChordFingVariations::CreateCombinations
			({ 1i16, 2i16, 3i16, 4i16, 5i16, 6i16 }), "WRONG NUMBER OF FINGERS");
# ifdef NDEBUG
	}
	catch (const out_of_range& e)
	{
		ASSERT_STREQ("invalid vector<T> subscript", e.what()) << "size = 0, but 6 fingers";
	}
# endif
}