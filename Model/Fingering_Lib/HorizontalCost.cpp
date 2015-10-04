# include "stdafx.h"
# include "HorizontalCost.h"
# include "MonoCosts.h"

using namespace std;
using namespace placeholders;
using namespace Model::Fingering;

// just partially tested

float HorizontalCost::Calculate(const vector<pair<int16_t, char>> chord1,
	const vector<pair<int16_t, char>> chord2, const vector<pair<int16_t, char>> chord3)
{
	if		(!chord3.empty()) return CalcSingles(chord3) + CalcPairs(chord2, chord3) + CalcTriples(chord1, chord2, chord3);
	else if (!chord2.empty()) return CalcSingles(chord2) + CalcPairs(chord1, chord2);	// beginning of Trellis graph
	else if (!chord1.empty()) return static_cast<float>(CalcSingles(chord1));	// otherwise warning 4244
	else
	{
		assert(!"THERE MUST BE AT LEAST ONE CHORD TO CALCULATE MOVING COST");
		return NULL;
	}
}

int HorizontalCost::CalcSingles(const vector<pair<int16_t, char>> chord)
{
	assert("THERE SHOULD BE AT LEAST ONE NOTE IN THE CHORD TO CALCULATE MOVING COST OF SINGLES" && !chord.empty());
	auto result(NULL);
	for (const auto& note : chord) result += MonoCosts().CostOfSingle(note.second);
	return result;
}

float HorizontalCost::CalcPairs(const vector<pair<int16_t, char>> chord1, const vector<pair<int16_t, char>> chord2)
{
	assert("THERE SHOULD BE AT LEAST ONE NOTE IN BOTH CHORDS TO CALCULATE MOVING COST OF PAIR"
		&& !chord1.empty() && !chord2.empty());

	auto result(0.0f);
	for (const auto& note1 : chord1)
		for (const auto& note2 : chord2)
			result += MonoCosts().CostOfPair(note1, note2);
	return result / chord1.size() / chord2.size();
}

float HorizontalCost::CalcTriples(const vector<pair<int16_t, char>> chord1,
	const vector<pair<int16_t, char>> chord2, const vector<pair<int16_t, char>> chord3)
{
	assert("THERE SHOULD BE AT LEAST ONE NOTE IN ALL THREE CHORDS TO CALCULATE MOVING COST OF TRIPLE"
		&& !chord1.empty() && !chord2.empty());

	auto result(0.0f);
	for (const auto& note1 : chord1)
		for (const auto& note2 : chord2)
			for (const auto& note3 : chord3)
				result += MonoCosts().CostOfThree(note1, note2, note3);
	return result / chord1.size() / chord2.size() / chord3.size();
}