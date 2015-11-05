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

	ASSERT_EQ(1, trellis.GetResult().size());
	ASSERT_EQ(RE_1, trellis.GetResult().front().front().front().first);
	ASSERT_EQ('\1', trellis.GetResult().front().front().front().second);
	ASSERT_EQ(SI, trellis.GetResult().front().front().at(1).first);
	ASSERT_EQ('\2', trellis.GetResult().front().front().at(1).second);
	ASSERT_EQ(SO, trellis.GetResult().front().front().back().first);
	ASSERT_EQ('\4', trellis.GetResult().front().front().back().second);

	ASSERT_EQ('\3', trellis.GetResult().front().at(1).front().second);
	ASSERT_EQ('\2', trellis.GetResult().front().at(2).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(3).front().second);
	ASSERT_EQ('\2', trellis.GetResult().front().at(4).front().second);
	ASSERT_EQ('\3', trellis.GetResult().front().at(5).front().second);
	ASSERT_EQ('\4', trellis.GetResult().front().at(6).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(7).front().second);
	ASSERT_EQ('\2', trellis.GetResult().front().at(8).front().second);
//	ASSERT_EQ('\3', trellis.GetResult().front().at(9).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(10).front().second);
//	ASSERT_EQ('\5', trellis.GetResult().front().at(11).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(12).front().second);
	ASSERT_EQ('\2', trellis.GetResult().front().at(13).front().second);
	ASSERT_EQ('\3', trellis.GetResult().front().back().front().second);
}

TEST_F(TrellisGraph_F, GFHandel_DSuite_HWV437_Saraband_LeftHand)	// Page 28, Figure 14
{
	vector<vector<int16_t>> chords({ { RE_1 }, { FA_1 }, { RE_1 }, { MI_1, LA_1 },
	{ LA }, { SO_1 }, { LA_1 }, { MI_1 }, { FA_1 }, { LA_1 }, { FA_1 }, { DO_1 },
	{ SI_1 }, { DO_2 }, { FA_S_1 }, { SO_1 }, { SI_B_1 }, { SO_1 }, { RE_2 },
	{ RE_1 }, { DO_2 }, { SI_B_1 }, { SI_B },
	{ SI_B_1 }, { LA_1 }, { SO_1 }, { FA_1 }, { MI_1 } });

	TrellisGraph trellis(chords, true);
	ASSERT_EQ(1, trellis.GetResult().size());
	ASSERT_EQ('\3', trellis.GetResult().front().front().front().second) << "may also be 4";
	ASSERT_EQ('\1', trellis.GetResult().front().at(1).front().second);
	ASSERT_EQ('\3', trellis.GetResult().front().at(2).front().second) << "may also be 4";

	ASSERT_EQ(LA_1, trellis.GetResult().front().at(3).front().first);
	ASSERT_EQ('\1', trellis.GetResult().front().at(3).front().second);
	ASSERT_EQ(MI_1, trellis.GetResult().front().at(3).back().first);
	ASSERT_EQ('\2', trellis.GetResult().front().at(3).back().second);

	ASSERT_EQ('\5', trellis.GetResult().front().at(4).front().second);
//	ASSERT_EQ('\2', trellis.GetResult().front().at(5).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(6).front().second);
	ASSERT_EQ('\3', trellis.GetResult().front().at(7).front().second) << "may also be 4";
	ASSERT_EQ('\2', trellis.GetResult().front().at(8).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(9).front().second);
	ASSERT_EQ('\2', trellis.GetResult().front().at(10).front().second);
	ASSERT_EQ('\5', trellis.GetResult().front().at(11).front().second);

//	ASSERT_EQ('\3', trellis.GetResult().front().at(11).front().second);
//	ASSERT_EQ('\2', trellis.GetResult().front().at(12).front().second);
//	ASSERT_EQ('\5', trellis.GetResult().front().at(13).front().second);
//	ASSERT_EQ('\4', trellis.GetResult().front().at(14).front().second);
//	ASSERT_EQ('\3', trellis.GetResult().front().at(15).front().second);
//	ASSERT_EQ('\2', trellis.GetResult().front().at(16).front().second);
//	ASSERT_EQ('\5', trellis.GetResult().front().at(17).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(18).front().second);

	ASSERT_EQ('\5', trellis.GetResult().front().at(19).front().second);
//	ASSERT_EQ('\2', trellis.GetResult().front().at(20).front().second);
	ASSERT_EQ('\1', trellis.GetResult().front().at(21).front().second);
	ASSERT_EQ('\5', trellis.GetResult().front().at(22).front().second);

	ASSERT_EQ('\1', trellis.GetResult().front().at(23).front().second) << "may also be 2";
//	ASSERT_EQ('\1', trellis.GetResult().front().at(24).front().second);
//	ASSERT_EQ('\2', trellis.GetResult().front().at(25).front().second);
//	ASSERT_EQ('\3', trellis.GetResult().front().at(26).front().second);
//	ASSERT_EQ('\4', trellis.GetResult().front().at(27).front().second);
}