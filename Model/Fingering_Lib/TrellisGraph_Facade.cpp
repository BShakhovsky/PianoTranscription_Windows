# include "stdafx.h"
# include "..\TrellisGraph_Facade.h"
# include "NodesLinker.h"

using namespace std;
using namespace Model::Fingering;

# define NOTE_MI 136i16		// note "mi" for left hand is mirrowed equivalent to note "do" for right hand

TrellisGraph::TrellisGraph(const vector<vector<int16_t>> chords, const bool leftHand) :
	graph_()
{
	NodesLinker trellis;
	if (leftHand) for (const auto& chord : chords)
	{
		auto mirrowedChord(chord);
		transform(chord.crbegin(), chord.crend(), mirrowedChord.begin(), bind1st(minus<int16_t>(), NOTE_MI));
												trellis.LinkNewNodes(mirrowedChord);
	}
	else		for (const auto& chord : chords) trellis.LinkNewNodes(chord);

	trellis.RemoveExpensivePaths();

	graph_ = trellis.GetResultedGraph();
	if (leftHand) for (auto& path : graph_) for (auto& node : path)
		for (auto& note : node->first) note.first = NOTE_MI - note.first;
}