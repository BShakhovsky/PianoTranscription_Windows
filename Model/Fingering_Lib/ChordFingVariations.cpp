# include "stdafx.h"
# include "ChordFingVariations.h"
# include "VerticalCost.h"

using namespace std;
using namespace Model::Fingering;
using namespace GraphStruct;

NodeList_ ChordFingVariations::CreateCombinations(const vector<int16_t> chord)
{
	const auto fingerings(GetFingerCombinations(chord.size()));
	NodeList_ result;
	result.reserve(fingerings.size());

	for (const auto& variant : fingerings)
	{
		assert("NUMBER OF FINGERS SHOULD BE EQUAL TO THE NUMBER OF NOTES IN A CHORD" && variant.size() == chord.size());
		auto subResult(make_shared<Node_>());
		
		subResult->first.reserve(chord.size());
		for (size_t i(NULL); i < chord.size(); ++i)	subResult->first.emplace_back(make_pair(chord.at(i), variant.at(i)));
		if		(chord.size() >= 2)					subResult->second = VerticalCost::Calculate(subResult->first);
		else if (chord.size() == 1)					subResult->second = 0.0f;
		else										assert(!"THERE HAVE TO BE AT LEAST ONE NOTE :)");
		
		result.push_back(subResult);
	}

	return result;
}

vector<vector<char>> ChordFingVariations::GetFingerCombinations(const size_t numberOfFingers)
{
	switch (numberOfFingers)
	{
	case 1: return OneFinger();		break;
	case 2: return TwoFingers();	break;
	case 3: return ThreeFingers();	break;
	case 4: return FourFingers();	break;
	case 5: return FiveFingers();	break;
	default: assert(!"WRONG NUMBER OF FINGERS");
			return{ {'\0'} };
	}
}

vector<vector<char>> ChordFingVariations::OneFinger()
{
	return{ { 1 }, { 2 }, { 3 }, { 4 }, { 5 } };
}

vector<vector<char>> ChordFingVariations::TwoFingers()
{
	return
	{
		{ 1, 2 },	{ 1, 3 },	{ 1, 4 },	{ 1, 5 },
		{ 2, 1 },	{ 2, 3 },	{ 2, 4 },	{ 2, 5 },
		{ 3, 1 },				{ 3, 4 },	{ 3, 5 },
		{ 4, 1 },							{ 4, 5 },
		{ 5, 1 }
	};
}

vector<vector<char>> ChordFingVariations::ThreeFingers()
{
	return
	{
		{ 1, 2, 3 },{ 1, 2, 4 },{ 1, 2, 5 },
					{ 1, 3, 4 },{ 1, 3, 5 },
								{ 1, 4, 5 },
		{ 2, 3, 1 },{ 2, 4, 1 },{ 2, 5, 1 },
		{ 2, 1, 3 },{ 2, 1, 4 },{ 2, 1, 5 },
					{ 2, 3, 4 },{ 2, 3, 5 },
								{ 2, 4, 5 },
		{ 3, 4, 1 },{ 3, 5, 1},
					{ 3, 1, 4 },{ 3, 1, 5 },
								{ 3, 4, 5 },
		{ 4, 5, 1 },
								{ 4, 1, 5 }
	};
}

vector<vector<char>> ChordFingVariations::FourFingers()
{
	return
	{
		{ 1, 2, 3, 4 },{ 1, 2, 3, 5 },{ 1, 2, 4, 5 },{ 1, 3, 4, 5 },{ 2, 3, 4, 5 },
		{ 2, 1, 3, 4 },{ 2, 1, 3, 5 },{ 2, 1, 4, 5 },{ 3, 1, 4, 5 },
		{ 2, 3, 1, 4 },{ 2, 3, 1, 5 },{ 2, 4, 1, 5 },{ 3, 4, 1, 5 },
		{ 2, 3, 4, 1 },{ 2, 3, 5, 1 },{ 2, 4, 5, 1 },{ 3, 4, 5, 1 }
	};
}

vector<vector<char>> ChordFingVariations::FiveFingers()
{
	return{ { 1, 2, 3, 4, 5 }, { 2, 1, 3, 4, 5 }, { 2, 3, 1, 4, 5 }, { 2, 3, 4, 1, 5 }, { 2, 3, 4, 5, 1 } };
}