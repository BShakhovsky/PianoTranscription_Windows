# include "stdafx.h"
# include "..\TrellisGraph_Facade.h"
# include "NodesLinker.h"

using namespace std;
using namespace Model::Fingering;
using GraphStruct::Chord_;

TrellisGraph::TrellisGraph(const vector<vector<int16_t>> chords, const bool leftHand) :
	result_()
{
	const auto NOTE_MI(136i16);		// note "mi" for left hand is mirrow-equivalent to note "do" for right hand

	NodesLinker trellis;
	for (const auto& chord : chords)
	{
		auto mirrowedChord(chord);
		if (leftHand) transform(chord.crbegin(), chord.crend(), mirrowedChord.begin(), bind1st(minus<int16_t>(), NOTE_MI));
		trellis.LinkNewNodes(mirrowedChord);
	}

	trellis.RemoveExpensivePaths();

	result_.reserve(trellis.GetResultedGraph().size());
	for (const auto& path : trellis.GetResultedGraph())
	{
		result_.emplace_back(vector<Chord_>());
		result_.back().reserve(path.size());
		for (const auto& node : path)
		{
			result_.back().push_back(node->first);
			if (leftHand) for (auto& note : result_.back().back()) note.first = NOTE_MI - note.first;
		}
	}
}