# include "stdafx.h"
# include "HorizontalCost.h"
# include "MonoCosts.h"

using namespace std;
using namespace placeholders;
using namespace Model::Fingering;

int HorizontalCost::Calculate(const vector<pair<int16_t, char>> chord1,
	const vector<pair<int16_t, char>> chord2, const vector<pair<int16_t, char>>* chord3)
{
	assert("THERE SHOULD BE AT LEAST ONE NOTE BEFORE AND ONE AFTER TO CALCULATE MOVING COST"
		&& !chord1.empty() && !chord2.empty());

	auto result(CalcSingles(&chord1) + CalcSingles(&chord2) + CalcPairs(&chord1, &chord2));
	if (chord3) result += CalcSingles(chord3) + CalcPairs(&chord2, chord3) + CalcTriples(&chord1, &chord2, chord3);
	return result;
}

int HorizontalCost::CalcSingles(const vector<pair<int16_t, char>>* chord)
{
	auto result(NULL);
	for (const auto& note : *chord) result += MonoCosts().CostOfSingle(note.second);
	return result;
}

int HorizontalCost::CalcPairs(const vector<pair<int16_t, char>>* chord1, const vector<pair<int16_t, char>>* chord2)
{
	auto result(NULL);
	for (const auto& note1 : *chord1)
		for (const auto& note2 : *chord2)
			result += MonoCosts().CostOfPair(note1, note2);
	return result / chord1->size() / chord2->size();
}

int HorizontalCost::CalcTriples(const vector<pair<int16_t, char>>* chord1,
	const vector<pair<int16_t, char>>* chord2, const vector<pair<int16_t, char>>* chord3)
{
	auto result(NULL);
	for (const auto& note1 : *chord1)
		for (const auto& note2 : *chord2)
			for (const auto& note3 : *chord3)
				result += MonoCosts().CostOfThree(note1, note2, note3);
	return result / chord1->size() / chord2->size() / chord3->size();
}