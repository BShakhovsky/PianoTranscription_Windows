# include "stdafx.h"
# include "..\..\Model\TrellisGraph_Facade.h"
# include "TrellisGraph_Common.h"

using std::vector;
using Model::Fingering::TrellisGraph;
using gTest::TrellisGraph_Common;

class TrellisGraph_F : public TrellisGraph_Common
{
public:
	TrellisGraph_F() : TrellisGraph_Common() {}
	TrellisGraph_F(const TrellisGraph_F&) = default;
	TrellisGraph_F& operator = (const TrellisGraph_F&) = default;

	virtual ~TrellisGraph_F() override = default;

	virtual void SetUp() override final
	{
		TrellisGraph_Common::SetUp();
	}
	virtual void TearDown() override final
	{
		TrellisGraph_Common::TearDown();
	}
};

// http://www.scriptiebank.be/sites/default/files/webform/scriptie/balliauw-matteo-2014_0.pdf

TEST_F(TrellisGraph_F, JSBach_Menuet4_BWVanh114_LeftHand)	// Page 27, Figure 12
{
	vector<vector<int16_t>> chords( { { SO, SI, RE_1 }, { LA }, { SI }, { DO_1 }, { SI },
	{ LA }, { SO }, { RE_1 }, { SI }, { SO }, { RE_1 }, { RE }, { DO_1 }, { SI }, { LA } } );
	TrellisGraph trellis(chords, true);

	ASSERT_EQ(1, trellis.GetGraph().size());
	ASSERT_EQ(RE_1, trellis.GetGraph().front().front()->first.front().first);
	ASSERT_EQ('\1', trellis.GetGraph().front().front()->first.front().second);
	ASSERT_EQ(SI, trellis.GetGraph().front().front()->first.at(1).first);
	ASSERT_EQ('\2', trellis.GetGraph().front().front()->first.at(1).second);
	ASSERT_EQ(SO, trellis.GetGraph().front().front()->first.back().first);
	ASSERT_EQ('\4', trellis.GetGraph().front().front()->first.back().second);

	ASSERT_EQ('\3', trellis.GetGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\2', trellis.GetGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\2', trellis.GetGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\3', trellis.GetGraph().front().at(5)->first.front().second);
	ASSERT_EQ('\4', trellis.GetGraph().front().at(6)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\2', trellis.GetGraph().front().at(8)->first.front().second);
//	ASSERT_EQ('\3', trellis.GetGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(10)->first.front().second);
//	ASSERT_EQ('\5', trellis.GetGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(12)->first.front().second);
	ASSERT_EQ('\2', trellis.GetGraph().front().at(13)->first.front().second);
	ASSERT_EQ('\3', trellis.GetGraph().front().back()->first.front().second);
}

TEST_F(TrellisGraph_F, GFHandel_DSuite_HWV437_Saraband_LeftHand)	// Page 28, Figure 14
{
	vector<vector<int16_t>> chords({ { RE_1 }, { FA_1 }, { RE_1 }, { MI_1, LA_1 },
	{ LA }, { SO_1 }, { LA_1 }, { MI_1 }, { FA_1 }, { LA_1 }, { FA_1 }, { DO_1 },
	{ SI_1 }, { DO_2 }, { FA_S_1 }, { SO_1 }, { SI_B_1 }, { SO_1 }, { RE_2 },
	{ RE_1 }, { DO_2 }, { SI_B_1 }, { SI_B },
	{ SI_B_1 }, { LA_1 }, { SO_1 }, { FA_1 }, { MI_1 } });

	TrellisGraph trellis(chords, true);
	ASSERT_EQ(1, trellis.GetGraph().size());
	ASSERT_EQ('\3', trellis.GetGraph().front().front()->first.front().second) << "may also be 4";
	ASSERT_EQ('\1', trellis.GetGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\3', trellis.GetGraph().front().at(2)->first.front().second) << "may also be 4";

	ASSERT_EQ(LA_1, trellis.GetGraph().front().at(3)->first.front().first);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(3)->first.front().second);
	ASSERT_EQ(MI_1, trellis.GetGraph().front().at(3)->first.back().first);
	ASSERT_EQ('\2', trellis.GetGraph().front().at(3)->first.back().second);

	ASSERT_EQ('\5', trellis.GetGraph().front().at(4)->first.front().second);
//	ASSERT_EQ('\2', trellis.GetGraph().front().at(5)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(6)->first.front().second);
	ASSERT_EQ('\3', trellis.GetGraph().front().at(7)->first.front().second) << "may also be 4";
	ASSERT_EQ('\2', trellis.GetGraph().front().at(8)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\2', trellis.GetGraph().front().at(10)->first.front().second);
	ASSERT_EQ('\5', trellis.GetGraph().front().at(11)->first.front().second);

//	ASSERT_EQ('\3', trellis.GetGraph().front().at(11)->first.front().second);
//	ASSERT_EQ('\2', trellis.GetGraph().front().at(12)->first.front().second);
//	ASSERT_EQ('\5', trellis.GetGraph().front().at(13)->first.front().second);
//	ASSERT_EQ('\4', trellis.GetGraph().front().at(14)->first.front().second);
//	ASSERT_EQ('\3', trellis.GetGraph().front().at(15)->first.front().second);
//	ASSERT_EQ('\2', trellis.GetGraph().front().at(16)->first.front().second);
//	ASSERT_EQ('\5', trellis.GetGraph().front().at(17)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(18)->first.front().second);

	ASSERT_EQ('\5', trellis.GetGraph().front().at(19)->first.front().second);
//	ASSERT_EQ('\2', trellis.GetGraph().front().at(20)->first.front().second);
	ASSERT_EQ('\1', trellis.GetGraph().front().at(21)->first.front().second);
	ASSERT_EQ('\5', trellis.GetGraph().front().at(22)->first.front().second);

	ASSERT_EQ('\1', trellis.GetGraph().front().at(23)->first.front().second) << "may also be 2";
//	ASSERT_EQ('\1', trellis.GetGraph().front().at(24)->first.front().second);
//	ASSERT_EQ('\2', trellis.GetGraph().front().at(25)->first.front().second);
//	ASSERT_EQ('\3', trellis.GetGraph().front().at(26)->first.front().second);
//	ASSERT_EQ('\4', trellis.GetGraph().front().at(27)->first.front().second);
}