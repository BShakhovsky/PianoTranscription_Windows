# include "stdafx.h"
# include "VerticalCost.h"
# include "PolyCosts.h"

using namespace std;
using namespace Model::Fingering;

class IsThumb
{
public:
	IsThumb() = default;
	~IsThumb() = default;
	IsThumb(const IsThumb&) = default;
	IsThumb& operator = (const IsThumb&) = default;

	bool operator () (const pair<int16_t, char>& note) const
	{
		return 1 == note.second;
	}
	bool operator () (const pair<int16_t, char>& lhs, const pair<int16_t, char>& rhs) const
	{
		return lhs.first < lhs.second;
	}
};

int VerticalCost::Calculate(const vector<pair<int16_t, char>> notes_fingers)
{
	assert("NUMBER OF NOTES IN A CHORD MUST BE BETWEEN TWO AND FIVE"
		&& notes_fingers.size() >= 2 && notes_fingers.size() <= 5);
	assert("ONLY THUMB PASSING IS ALLOWED BY CURRENT ALGORITHM" && 1 == notes_fingers.front().second || (
		is_sorted(notes_fingers.cbegin(), find_if(notes_fingers.cbegin(), notes_fingers.cend(), IsThumb()) - 1, IsThumb())
		&& is_sorted(find_if(notes_fingers.cbegin(), notes_fingers.cend(), IsThumb()), notes_fingers.cend(), IsThumb())));
	assert("NOTES SHOULD BE SORTED IN ASCENDING ORDER BEFORE CHORD COST CALCULATION"
		&& is_sorted(notes_fingers.cbegin(), notes_fingers.cend()));

	auto result(PolyCosts().CostOfPair(notes_fingers.at(1), notes_fingers.at(2)));
	if (notes_fingers.size() >= 3) result += PolyCosts().CostOfPair(notes_fingers.at(2), notes_fingers.at(3));
	if (notes_fingers.size() >= 4) result += PolyCosts().CostOfPair(notes_fingers.at(3), notes_fingers.at(4));
	if (notes_fingers.size() >= 5) result += PolyCosts().CostOfPair(notes_fingers.at(4), notes_fingers.at(5));
	return result;
}