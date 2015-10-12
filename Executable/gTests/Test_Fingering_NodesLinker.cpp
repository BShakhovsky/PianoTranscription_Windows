# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\NodesLinker.h"
# include "TrellisGraph_Common.h"

using std::make_pair;
using Model::Fingering::NodesLinker;
using gTest::TrellisGraph_Common;

class NodesLinker_F : public TrellisGraph_Common
{
public:
	NodesLinker_F() : TrellisGraph_Common() {}
	NodesLinker_F(const NodesLinker_F&) = default;
	NodesLinker_F& operator = (const NodesLinker_F&) = default;

	virtual ~NodesLinker_F() override = default;

	virtual void SetUp() override final
	{
		TrellisGraph_Common::SetUp();
	}
	virtual void TearDown() override final
	{
		TrellisGraph_Common::TearDown();
	}
};

TEST_F(NodesLinker_F, LinkNewNodes)
{
	NodesLinker graph;
	graph.LinkNewNodes({ 1i16, 2i16 });
	ASSERT_EQ(14, graph.GetResultedGraph().size());
	for (const auto& path : graph.GetResultedGraph()) ASSERT_EQ(1, path.size());
	ASSERT_EQ(NULL, graph.GetResultedGraph().front().front()->second);
	ASSERT_EQ(4, graph.GetResultedGraph().at(1).front()->second);
	ASSERT_EQ(9, graph.GetResultedGraph().at(2).front()->second);
	ASSERT_EQ(13, graph.GetResultedGraph().at(3).front()->second);

	ASSERT_EQ(4, graph.GetResultedGraph().at(4).front()->second);
	ASSERT_EQ(NULL, graph.GetResultedGraph().at(5).front()->second);
	ASSERT_EQ(9, graph.GetResultedGraph().at(6).front()->second);
	ASSERT_EQ(31, graph.GetResultedGraph().at(7).front()->second);

	ASSERT_EQ(8, graph.GetResultedGraph().at(8).front()->second);
	ASSERT_EQ(1, graph.GetResultedGraph().at(9).front()->second);
	ASSERT_EQ(9, graph.GetResultedGraph().at(10).front()->second);

	ASSERT_EQ(13, graph.GetResultedGraph().at(11).front()->second);
	ASSERT_EQ(2, graph.GetResultedGraph().at(12).front()->second);

	ASSERT_EQ(25, graph.GetResultedGraph().back().front()->second);


	graph.LinkNewNodes({ 3i16 });
	ASSERT_EQ(5, graph.GetResultedGraph().size());
	for (const auto& path : graph.GetResultedGraph())
	{
		ASSERT_EQ(2, path.size());
		ASSERT_EQ(1, path.back()->first.size());
		ASSERT_EQ(2, path.front()->first.size());
	}
	ASSERT_EQ(make_pair(3i16, '\1'), graph.GetResultedGraph().front().back()->first.front());
	ASSERT_EQ(make_pair(3i16, '\2'), graph.GetResultedGraph().at(1).back()->first.front());
	ASSERT_EQ(make_pair(3i16, '\3'), graph.GetResultedGraph().at(2).back()->first.front());
	ASSERT_EQ(make_pair(3i16, '\4'), graph.GetResultedGraph().at(3).back()->first.front());
	ASSERT_EQ(make_pair(3i16, '\5'), graph.GetResultedGraph().back().back()->first.front());

	ASSERT_EQ(make_pair(1i16, '\2'), graph.GetResultedGraph().front().front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), graph.GetResultedGraph().front().front()->first.back());
	ASSERT_EQ(6, graph.GetResultedGraph().front().back()->second);
	ASSERT_EQ(make_pair(1i16, '\1'), graph.GetResultedGraph().at(1).front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), graph.GetResultedGraph().at(1).front()->first.back());
	ASSERT_EQ(4.25, graph.GetResultedGraph().at(1).back()->second);
	ASSERT_EQ(make_pair(1i16, '\1'), graph.GetResultedGraph().at(2).front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), graph.GetResultedGraph().at(2).front()->first.back());
	ASSERT_EQ(0.75, graph.GetResultedGraph().at(2).back()->second);
	ASSERT_EQ(make_pair(1i16, '\2'), graph.GetResultedGraph().at(3).front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), graph.GetResultedGraph().at(3).front()->first.back());
	ASSERT_EQ(3, graph.GetResultedGraph().at(3).back()->second);
	ASSERT_EQ(make_pair(1i16, '\3'), graph.GetResultedGraph().back().front()->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), graph.GetResultedGraph().back().front()->first.back());
	ASSERT_EQ(3.5, graph.GetResultedGraph().back().back()->second);


	graph.LinkNewNodes({ 2i16 });
	ASSERT_EQ(5, graph.GetResultedGraph().size());
	for (const auto& path : graph.GetResultedGraph())
	{
		ASSERT_EQ(3, path.size());
		ASSERT_EQ(1, path.back()->first.size());
		ASSERT_EQ(2, path.front()->first.size());
		ASSERT_EQ(1, path.at(1)->first.size());
	}
	ASSERT_EQ(make_pair(2i16, '\1'), graph.GetResultedGraph().front().back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\2'), graph.GetResultedGraph().at(1).back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\3'), graph.GetResultedGraph().at(2).back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\4'), graph.GetResultedGraph().at(3).back()->first.front());
	ASSERT_EQ(make_pair(2i16, '\5'), graph.GetResultedGraph().back().back()->first.front());

	ASSERT_EQ(make_pair(3i16, '\3'), graph.GetResultedGraph().front().at(1)->first.front());
	ASSERT_EQ(3.25, graph.GetResultedGraph().front().back()->second);
	ASSERT_EQ(make_pair(3i16, '\3'), graph.GetResultedGraph().at(1).at(1)->first.front());
	ASSERT_EQ(0.75, graph.GetResultedGraph().at(1).back()->second);
	ASSERT_EQ(make_pair(3i16, '\4'), graph.GetResultedGraph().at(2).at(1)->first.front());
	ASSERT_EQ(5, graph.GetResultedGraph().at(2).back()->second);
	ASSERT_EQ(make_pair(3i16, '\5'), graph.GetResultedGraph().at(3).at(1)->first.front());
	ASSERT_EQ(6, graph.GetResultedGraph().at(3).back()->second);
	ASSERT_EQ(make_pair(3i16, '\5'), graph.GetResultedGraph().back().at(1)->first.front());
	ASSERT_EQ(14, graph.GetResultedGraph().back().back()->second);


	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ(0.75, graph.GetResultedGraph().front().back()->second);
}

TEST_F(NodesLinker_F, RemoveExpensivePaths)
{
	NodesLinker graph;
	graph.LinkNewNodes({ 0i16 });
	graph.LinkNewNodes({ 9i16 });
	ASSERT_EQ(5, graph.GetResultedGraph().size());
	ASSERT_EQ(8, graph.GetResultedGraph().front().back()->second) << "same finger rule";
	ASSERT_EQ(6, graph.GetResultedGraph().at(1).back()->second);
	ASSERT_EQ(2, graph.GetResultedGraph().at(2).back()->second);
	ASSERT_EQ(1, graph.GetResultedGraph().at(3).back()->second);
	ASSERT_EQ(1, graph.GetResultedGraph().back().back()->second);

	graph.RemoveExpensivePaths();
	ASSERT_EQ(2, graph.GetResultedGraph().size());
	ASSERT_EQ(1, graph.GetResultedGraph().front().back()->second);
	ASSERT_EQ(1, graph.GetResultedGraph().back().back()->second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().back()->first.front().second);
}

// http://ismir2007.ismir.net/proceedings/ISMIR2007_p355_kasimi.pdf

TEST_F(NodesLinker_F, Figure4)	// Page 2, Figure 4
{
	NodesLinker graph;
	graph.LinkNewNodes({ DO, MI, SO });
	graph.LinkNewNodes({ FA, LA });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\1', graph.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().front()->first.at(1).second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().front()->first.back().second);

	ASSERT_EQ('\3', graph.GetResultedGraph().front().back()->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.back().second);
}

TEST_F(NodesLinker_F, EllmenreichSpinningSong)	// Page 2, Figure 6
{
	NodesLinker graph;
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA, FA_1 });
	graph.LinkNewNodes({ DO_1, SO_1 });
	graph.LinkNewNodes({ FA_1, LA_1 });

	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA, DO_1, FA_1 });
	graph.LinkNewNodes({ SI_B, DO_1, MI_1 });
	graph.LinkNewNodes({ LA, DO_1, FA_1 });

	graph.LinkNewNodes({ MI_1, SO_1 });
	graph.LinkNewNodes({ FA_1, LA_B_1 });
	graph.LinkNewNodes({ FA_S_1, LA_1 });
	graph.LinkNewNodes({ SO_1, SI_B_1 });
	graph.LinkNewNodes({ SI_B_1, RE_2 });
	graph.LinkNewNodes({ LA_1, DO_2 });
	graph.LinkNewNodes({ SO_1, SI_B_1 });
	graph.LinkNewNodes({ FA_1, LA_1 });
	graph.LinkNewNodes({ MI_1, SO_1 });
	graph.LinkNewNodes({ FA_1, SO_1, SI_1 });
	graph.LinkNewNodes({ MI_1, SO_1, DO_2 });

	graph.LinkNewNodes({ MI_1, SO_1 });
	graph.LinkNewNodes({ FA_1, LA_B_1 });
	graph.LinkNewNodes({ FA_S_1, LA_1 });
	graph.LinkNewNodes({ SO_1, SI_B_1 });
	graph.LinkNewNodes({ SI_B_1, RE_2 });
	graph.LinkNewNodes({ LA_1, DO_2 });
	graph.LinkNewNodes({ SO_1, SI_B_1 });
	graph.LinkNewNodes({ FA_1, LA_1 });
	graph.LinkNewNodes({ MI_1, SO_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ SI_B });

	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA, FA_1 });
	graph.LinkNewNodes({ DO_1, SO_1 });
	graph.LinkNewNodes({ FA_1, LA_1 });

	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\1', graph.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(2)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(3)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(5)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(5)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(6)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(6)->first.back().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(7)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(8)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(9)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(10)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(11)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(12)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(13)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(13)->first.at(1).second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(13)->first.back().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(14)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(14)->first.at(1).second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(14)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(15)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(15)->first.at(1).second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(15)->first.back().second);

//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(16)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(16)->first.back().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(17)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(17)->first.back().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(18)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(18)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(19)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(19)->first.back().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(20)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(20)->first.back().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(21)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(21)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(22)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(22)->first.back().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(23)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(23)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(24)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(24)->first.back().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(25)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(25)->first.at(1).second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(25)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(26)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(26)->first.at(1).second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(26)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(27)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(27)->first.back().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(28)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(28)->first.back().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(29)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(29)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(30)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(30)->first.back().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(31)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(31)->first.back().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(32)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(32)->first.back().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(33)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(33)->first.back().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(34)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(34)->first.back().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(35)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(35)->first.back().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(36)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(37)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(38)->first.front().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(39)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(40)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(41)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(42)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(43)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(44)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(45)->first.back().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(46)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(46)->first.back().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().back()->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.back().second);
}

// http://web.cs.ucdavis.edu/~neff/papers/cav1477.pdf
// Page 9, Figure 13

TEST_F(NodesLinker_F, BilderEinerAusstellung)
{
	NodesLinker graph;
	graph.LinkNewNodes({ MI, SO, DO_1 });
	graph.LinkNewNodes({ DO, RE, FA, LA });
	graph.LinkNewNodes({ MI, SO, DO_1 });
	graph.LinkNewNodes({ FA, FA_1 });
	graph.LinkNewNodes({ MI_B, SO, SI_B, MI_B_1 });
	graph.LinkNewNodes({ RE, RE_1 });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\1', graph.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().front()->first.at(1).second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().front()->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.at(1).second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(1)->first.at(2).second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(1)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(2)->first.at(1).second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(2)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(3)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(4)->first.at(1).second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(4)->first.at(2).second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(4)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().back()->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.back().second);
}

// http://www.diva-portal.org/smash/get/diva2:769592/FULLTEXT01.pdf
// Page 16

TEST_F(NodesLinker_F, Chopin_FirstScherzo)	// Figure 10
{
	NodesLinker graph;
	graph.LinkNewNodes({ FA });
	graph.LinkNewNodes({ FA_S });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ LA_S });
	graph.LinkNewNodes({ DO_S_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ FA_S_1 });
	graph.LinkNewNodes({ SI_1 });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\1', graph.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(5)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(6)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(7)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(8)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.front().second);
}

TEST_F(NodesLinker_F, Entertainer)	// Figure 11
{
	NodesLinker graph1, graph2, graph3;
	graph1.LinkNewNodes({ RE });
	graph1.LinkNewNodes({ RE_S });
	graph1.LinkNewNodes({ MI });
	graph1.LinkNewNodes({ DO_1 });
	graph1.LinkNewNodes({ MI });
	graph1.LinkNewNodes({ DO_1 });
	graph1.LinkNewNodes({ MI });
	graph1.LinkNewNodes({ DO_1 });

	graph2.LinkNewNodes({ DO_1 });
	graph2.LinkNewNodes({ RE_1 });
	graph2.LinkNewNodes({ RE_S_1 });
	graph2.LinkNewNodes({ MI_1 });
	graph2.LinkNewNodes({ DO_1 });
	graph2.LinkNewNodes({ RE_1 });
	graph2.LinkNewNodes({ MI_1 });
	graph2.LinkNewNodes({ SI });
	graph2.LinkNewNodes({ RE_1 });
	graph2.LinkNewNodes({ DO_1 });
	
	graph3.LinkNewNodes({ RE });
	graph3.LinkNewNodes({ RE_S });
	graph3.LinkNewNodes({ MI });
	graph3.LinkNewNodes({ DO_1 });
	graph3.LinkNewNodes({ MI });
	graph3.LinkNewNodes({ DO_1 });
	graph3.LinkNewNodes({ MI });
	graph3.LinkNewNodes({ DO_1 });
	graph3.LinkNewNodes({ LA });
	graph3.LinkNewNodes({ SO });
	graph3.LinkNewNodes({ FA_S });
	graph3.LinkNewNodes({ LA });
	graph3.LinkNewNodes({ DO_1 });
	graph3.LinkNewNodes({ MI_1 });
	graph3.LinkNewNodes({ RE_1 });
	graph3.LinkNewNodes({ DO_1 });
	graph3.LinkNewNodes({ LA });
	graph3.LinkNewNodes({ RE_1 });

	graph1.RemoveExpensivePaths();
	graph2.RemoveExpensivePaths();
	graph3.RemoveExpensivePaths();
	ASSERT_EQ(1, graph1.GetResultedGraph().size());
	ASSERT_EQ(2, graph2.GetResultedGraph().size());
	ASSERT_EQ(2, graph3.GetResultedGraph().size());
	ASSERT_EQ('\1', graph1.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph1.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\1', graph1.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\5', graph1.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\1', graph1.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\5', graph1.GetResultedGraph().front().at(5)->first.front().second);
	ASSERT_EQ('\1', graph1.GetResultedGraph().front().at(6)->first.front().second);
	ASSERT_EQ('\5', graph1.GetResultedGraph().front().back()->first.front().second);
	

	ASSERT_EQ('\1', graph2.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\1', graph2.GetResultedGraph().back().front()->first.front().second);
	ASSERT_EQ('\2', graph2.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\2', graph2.GetResultedGraph().back().at(1)->first.front().second);
	ASSERT_EQ('\3', graph2.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\3', graph2.GetResultedGraph().back().at(2)->first.front().second);
	ASSERT_EQ('\4', graph2.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\4', graph2.GetResultedGraph().back().at(3)->first.front().second);
	ASSERT_EQ('\1', graph2.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\1', graph2.GetResultedGraph().back().at(4)->first.front().second);
//	ASSERT_EQ('\3', graph2.GetResultedGraph().front().at(5)->first.front().second);
//	ASSERT_EQ('\3', graph2.GetResultedGraph().back().at(5)->first.front().second);
//	ASSERT_EQ('\4', graph2.GetResultedGraph().front().at(6)->first.front().second);
//	ASSERT_EQ('\4', graph2.GetResultedGraph().back().at(6)->first.front().second);
	ASSERT_EQ('\1', graph2.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\1', graph2.GetResultedGraph().back().at(7)->first.front().second);

//	ASSERT_EQ('\4', graph2.GetResultedGraph().front().at(8)->first.front().second);
	ASSERT_EQ('\3', graph2.GetResultedGraph().back().at(8)->first.front().second);
//	ASSERT_EQ('\3', graph2.GetResultedGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\2', graph2.GetResultedGraph().back().at(9)->first.front().second);


	ASSERT_EQ('\1', graph3.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().back().front()->first.front().second);
	ASSERT_EQ('\2', graph3.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\2', graph3.GetResultedGraph().back().at(1)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().back().at(2)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().back().at(3)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().back().at(4)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().front().at(5)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().back().at(5)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().front().at(6)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().back().at(6)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().back().at(7)->first.front().second);

//	ASSERT_EQ('\4', graph3.GetResultedGraph().front().at(8)->first.front().second);
	ASSERT_EQ('\3', graph3.GetResultedGraph().back().at(8)->first.front().second);
//	ASSERT_EQ('\3', graph3.GetResultedGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\2', graph3.GetResultedGraph().back().at(9)->first.front().second);
//	ASSERT_EQ('\2', graph3.GetResultedGraph().front().at(10)->first.front().second);
	ASSERT_EQ('\1', graph3.GetResultedGraph().back().at(10)->first.front().second);
//	ASSERT_EQ('\1', graph3.GetResultedGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\2', graph3.GetResultedGraph().back().at(11)->first.front().second);

	ASSERT_EQ('\3', graph3.GetResultedGraph().front().at(12)->first.front().second);
	ASSERT_EQ('\3', graph3.GetResultedGraph().back().at(12)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().front().at(13)->first.front().second);
	ASSERT_EQ('\5', graph3.GetResultedGraph().back().at(13)->first.front().second);
//	ASSERT_EQ('\4', graph3.GetResultedGraph().front().at(14)->first.front().second);
//	ASSERT_EQ('\4', graph3.GetResultedGraph().back().at(14)->first.front().second);
//	ASSERT_EQ('\3', graph3.GetResultedGraph().front().at(15)->first.front().second);
//	ASSERT_EQ('\3', graph3.GetResultedGraph().back().at(15)->first.front().second);
//	ASSERT_EQ('\2', graph3.GetResultedGraph().front().at(16)->first.front().second);
//	ASSERT_EQ('\2', graph3.GetResultedGraph().back().at(16)->first.front().second);

//	ASSERT_EQ('\5', graph3.GetResultedGraph().front().back()->first.front().second);
//	ASSERT_EQ('\4', graph3.GetResultedGraph().back().back()->first.front().second);
}

TEST_F(NodesLinker_F, FurElise)	// Figure 12
{
	NodesLinker graph;
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_S_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_S_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ DO });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ SO_S });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ DO });

	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_S_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_S_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ DO });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ LA });
	
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ FA_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ FA });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ MI_1 });

	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\3', graph.GetResultedGraph().front().front()->first.front().second) << "may also be 5";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.front().second) << "may also be 4";
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(2)->first.front().second) << "may also be 5";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(3)->first.front().second) << "may also be 4";
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(4)->first.front().second) << "may also be 5";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(5)->first.front().second) << "may also be 2";
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(6)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(7)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(8)->first.front().second) << "may also be 1";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(10)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(12)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(13)->first.front().second);
	
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(14)->first.front().second) << "may also be 2";
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(15)->first.front().second) << "may also be 3";
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(16)->first.front().second) << "may also be 4";

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(17)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(18)->first.front().second) << "may also be 3";
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(19)->first.front().second) << "may also be 2";
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(20)->first.front().second) << "may also be 3";
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(21)->first.front().second) << "may also be 2";
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(22)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(23)->first.front().second) << "may also be 1";
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(24)->first.front().second) << "may also be 3";
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(25)->first.front().second) << "may also be 2";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(26)->first.front().second) << "may also be 1";
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(27)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(28)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(29)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(30)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(31)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(32)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(33)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(34)->first.front().second);

	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(35)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(36)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(37)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(38)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(39)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(40)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(41)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(42)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(43)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(44)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(45)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(46)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(47)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(48)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(49)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(50)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(51)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(52)->first.front().second);
}

// http://www.scriptiebank.be/sites/default/files/webform/scriptie/balliauw-matteo-2014_0.pdf

TEST_F(NodesLinker_F, JSBach_Menuet4_BWVanh114)	// Page 27, Figure 12
{
	NodesLinker graph;
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ SO });
	
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ FA_S_1 });
	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ SO });
	
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ FA_S });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ LA });

	graph.RemoveExpensivePaths();
	ASSERT_EQ(2, graph.GetResultedGraph().size());
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().front()->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().back().front()->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(1)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(2)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(3)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(4)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(5)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(5)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(6)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(6)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(7)->first.front().second);

	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(8)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(8)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(9)->first.front().second) << "may also be 2";
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(9)->first.front().second) << "may also be 2";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(10)->first.front().second) << "may also be 1";
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(10)->first.front().second) << "may also be 1";
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(11)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(12)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(12)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(13)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(13)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(14)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(14)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(15)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(15)->first.front().second);

//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(16)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(16)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(17)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(17)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(18)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(18)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(19)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(19)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(20)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(20)->first.front().second);
	
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(21)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(21)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(22)->first.front().second) << "may also be 5";
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(22)->first.front().second) << "may also be 5";
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(23)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(23)->first.front().second) << "may also be 4";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(24)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(24)->first.front().second) << "may also be 3";

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(25)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(25)->first.front().second);

	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(26)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(26)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(27)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(27)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(28)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(28)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(29)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(29)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(30)->first.front().second) << "may also be 2";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(30)->first.front().second) << "may also be 2";
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(31)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(31)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().back()->first.front().second) << "may also be 3";
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().back()->first.front().second) << "may also be 3";
}

TEST_F(NodesLinker_F, GFHandel_DSuite_HWV437_Saraband)	// Page 28, Figure 14
{
	NodesLinker graph;
	graph.LinkNewNodes({ FA_1 });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ FA_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ DO_S_1 });
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ FA_1 });
	graph.LinkNewNodes({ DO_2 });
	graph.LinkNewNodes({ SI_B_1 });
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ MI_1 });
	
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ SI_B_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ RE_2 });
	graph.LinkNewNodes({ DO_2 });
	graph.LinkNewNodes({ SI_B_1 });
	graph.LinkNewNodes({ LA_1 });

	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ FA_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ RE_1 });
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ LA_1, RE_2 });
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ SO_1 });

	graph.LinkNewNodes({ FA_2 });
	graph.LinkNewNodes({ MI_2 });
	graph.LinkNewNodes({ RE_2 });
	graph.LinkNewNodes({ DO_S_2 });
	graph.LinkNewNodes({ MI_2 });
	graph.LinkNewNodes({ LA_1 });
	graph.LinkNewNodes({ DO_S_2 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ SO_1 });

	graph.RemoveExpensivePaths();
	ASSERT_EQ(3, graph.GetResultedGraph().size());
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().front()->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).front()->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().front()->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(1)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(1)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(2)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(2)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(3)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().at(1).at(3)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(3)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(4)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(4)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(5)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(5)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(5)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(6)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\3', graph.GetResultedGraph().at(1).at(6)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(6)->first.front().second) << "may also be 4";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(7)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(7)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(8)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(8)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(8)->first.front().second);

//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(9)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(9)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(9)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(10)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(10)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(10)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(11)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(11)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(12)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().at(1).at(12)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(12)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(13)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(13)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(13)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(14)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(14)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(14)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(15)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\3', graph.GetResultedGraph().at(1).at(15)->first.front().second) << "may also be 4";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(15)->first.front().second) << "may also be 4";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(16)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(16)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(16)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(17)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(17)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(17)->first.front().second);

//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(18)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(18)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(18)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(19)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().at(1).at(19)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(19)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(20)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(20)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(20)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(21)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(21)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(21)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(22)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().at(1).at(22)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(22)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(23)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(23)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(23)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(24)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(24)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(24)->first.front().second) << "may also be 3";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(25)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(25)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(25)->first.front().second);

//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(26)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(26)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(26)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(27)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().at(1).at(27)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(27)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(28)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(28)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(28)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(29)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(29)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(29)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(30)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().at(1).at(30)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(30)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(31)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(31)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(31)->first.front().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(31)->first.back().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(31)->first.back().second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(31)->first.back().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(32)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(32)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(32)->first.front().second) << "may also be 3";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(33)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(33)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(33)->first.front().second);

	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(34)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().at(1).at(34)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(34)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(35)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(35)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(35)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(36)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().at(1).at(36)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(36)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(37)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().at(1).at(37)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(37)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(38)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().at(1).at(38)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().back().at(38)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(39)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(39)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(39)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(40)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().at(1).at(40)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().back().at(40)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(41)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().at(1).at(41)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(41)->first.front().second);
//	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.front().second) << "may also be 4";
//	ASSERT_EQ('\5', graph.GetResultedGraph().at(1).back()->first.front().second) << "may also be 4";
//	ASSERT_EQ('\5', graph.GetResultedGraph().back().back()->first.front().second) << "may also be 4";
}

// http://www.csc.kth.se/utbildning/kth/kurser/DD143X/dkand13/Group7Anders/final/Bassam.Alfarhan.David.Sandberg.report.pdf
// Page 14

TEST_F(NodesLinker_F, AllOfMe)	// Figure 4.1
{
	NodesLinker graph;
	graph.LinkNewNodes({ DO_2 });
	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.LinkNewNodes({ DO_2 });
	graph.LinkNewNodes({ RE_2 });
	graph.LinkNewNodes({ DO_2 });
	graph.LinkNewNodes({ SI });
	graph.LinkNewNodes({ SO_S_1 });
	graph.LinkNewNodes({ MI_1 });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\5', graph.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(2)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(3)->first.front().second) << "may also be 2";
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(4)->first.front().second) << "may also be 3";
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(5)->first.front().second) << "may also be 2";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(6)->first.front().second) << "may also be 3";
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(7)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().back()->first.front().second);
}

TEST_F(NodesLinker_F, AintMisbehavin)	// Figure 4.2
{
	NodesLinker graph;
	graph.LinkNewNodes({ MI_B });
	graph.LinkNewNodes({ FA });
	graph.LinkNewNodes({ MI_B });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ FA });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ FA });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ SI_B });
	graph.LinkNewNodes({ MI_B_1 });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(2, graph.GetResultedGraph().size());
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().front()->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().back().front()->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(1)->first.front().second);
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(1)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(2)->first.front().second) << "may also be 2";
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(2)->first.front().second) << "may also be 2";
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(3)->first.front().second) << "may also be 5";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(3)->first.front().second) << "may also be 5";
//	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(4)->first.front().second) << "may also be 5";
//	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(4)->first.front().second) << "may also be 5";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(5)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(5)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(6)->first.front().second) << "may also be 3";
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(6)->first.front().second) << "may also be 3";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().back().at(7)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(8)->first.front().second) << "may also be 5";
	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(8)->first.front().second) << "may also be 5";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(9)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(10)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().back().at(10)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().back().at(11)->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().front().back()->first.front().second);
	ASSERT_EQ('\5', graph.GetResultedGraph().back().back()->first.front().second);
}

TEST_F(NodesLinker_F, AllaFaglarKommitRen)	// Figure 4.3
{
	NodesLinker graph;
	graph.LinkNewNodes({ DO });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ LA });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ FA });
	graph.LinkNewNodes({ SO });
	graph.LinkNewNodes({ MI });
	graph.LinkNewNodes({ DO });
	graph.LinkNewNodes({ RE });
	graph.LinkNewNodes({ DO });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
	ASSERT_EQ('\1', graph.GetResultedGraph().front().front()->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(2)->first.front().second) << "may also be 1";
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(3)->first.front().second) << "may also be 2";
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(4)->first.front().second) << "may also be 1";
	ASSERT_EQ('\5', graph.GetResultedGraph().front().at(5)->first.front().second) << "may also be 2";
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(6)->first.front().second) << "may also be 1";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(8)->first.front().second) << "may also be 3";
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(9)->first.front().second) << "may also be 2";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(10)->first.front().second) << "may also be 1";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(11)->first.front().second) << "may also be 2";
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(12)->first.front().second) << "may also be 3";
	ASSERT_EQ('\1', graph.GetResultedGraph().front().back()->first.front().second) << "may also be 2";
}

TEST_F(NodesLinker_F, InterleavedTriad)	// Figure 4.4
{
	NodesLinker graph;
	graph.LinkNewNodes({ SI_B_1 });
	graph.LinkNewNodes({ SO_1 });
	graph.LinkNewNodes({ MI_B_1 });
	graph.LinkNewNodes({ LA_B_1 });
	graph.LinkNewNodes({ FA_1 });
	graph.LinkNewNodes({ RE_B_1 });
	graph.LinkNewNodes({ DO_1, MI_B_1, SO_1 });
	graph.LinkNewNodes({ LA_B });
	graph.LinkNewNodes({ DO_1 });
	graph.LinkNewNodes({ RE_B_1 });
	graph.LinkNewNodes({ MI_B_1 });
	graph.LinkNewNodes({ RE_B_1 });
	graph.LinkNewNodes({ DO_1 });
	graph.RemoveExpensivePaths();
	ASSERT_EQ(1, graph.GetResultedGraph().size());
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().front()->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(1)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(2)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(3)->first.front().second);
//	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(4)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(5)->first.front().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(6)->first.front().second);
//	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(6)->first.at(1).second);
//	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(6)->first.back().second);

	ASSERT_EQ('\1', graph.GetResultedGraph().front().at(7)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().at(8)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(9)->first.front().second);
	ASSERT_EQ('\4', graph.GetResultedGraph().front().at(10)->first.front().second);
	ASSERT_EQ('\3', graph.GetResultedGraph().front().at(11)->first.front().second);
	ASSERT_EQ('\2', graph.GetResultedGraph().front().back()->first.front().second);
}