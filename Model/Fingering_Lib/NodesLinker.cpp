# include "stdafx.h"
# include "NodesLinker.h"
# include "ChordFingVariations.h"
# include "HorizontalCost.h"

using namespace std;
using namespace Model::Fingering;
using namespace GraphStruct;

void NodesLinker::LinkNewNodes(const vector<int16_t> chord)
{
	chordVariations_ = ChordFingVariations::CreateCombinations(chord);
	vector<NodeList_> result;

	if (graph_.empty())	for (const auto& node : chordVariations_)
	{
		result.emplace_back(NodeList_({ node }));
		result.back().back()->second += HorizontalCost::Calculate(result.back().back()->first, {}, {});
	}
	else				for (const auto& node : chordVariations_)
	{
		const auto minVal(MinPathFinder(node));
		for (const auto& minPth : minPaths_)
		{
			result.push_back(*minPth);
			result.back().push_back(node);
		}
		node->second = minVal;
	}

	graph_.swap(result);
}

float NodesLinker::MinPathFinder(const shared_ptr<Node_> node)
{
	auto minVal(FLT_MAX);

	minPaths_ = { graph_.cbegin() };
	for (auto path(graph_.cbegin()); path != graph_.cend(); ++path)
	{
		auto newVal(0.0f);
		if (path->size() >= 2)
		{
			auto pathPrev(path->crbegin());
			++pathPrev;
			newVal = path->back()->second + node->second + HorizontalCost::Calculate(pathPrev->get()->first,
				path->back()->first, node->first);
		}
		else newVal = path->back()->second + node->second + HorizontalCost::Calculate(
			path->back()->first, node->first, {});

		MinValCompare(&minVal, &newVal, path);
	}

	return minVal;
}

void NodesLinker::MinValCompare(float* minVal, float* newVal, Graph_::const_iterator path)
{
	if (*newVal < *minVal)
	{
		*minVal = *newVal;
		minPaths_.clear();
		minPaths_.push_back(path);
	}
	else if (*newVal == *minVal)
		minPaths_.push_back(path);
}

class PathCompare
{
public:
	PathCompare() = default;
	PathCompare(const PathCompare&) = default;
	PathCompare& operator = (const PathCompare&) = default;
	~PathCompare() = default;

	bool operator () (const NodeList_& lhs, const NodeList_& rhs) const
	{
		return lhs.back()->second < rhs.back()->second;
	}
};

void NodesLinker::RemoveExpensivePaths()
{
	assert("CHORD MAY HAVE AT LEAST FIVE FINGER COMBINATION" && graph_.size() >= 5);
	for (auto i(2); graph_.cbegin() + (i - 1) != graph_.cend(); ++i)
	{
		partial_sort(graph_.begin(), graph_.begin() + i, graph_.end(), PathCompare());
		if (PathCompare()(graph_.front(), graph_.at(i - 1)))
		{
			graph_.erase(graph_.cbegin() + i - 1, graph_.cend());
			break;
		}
	}
}