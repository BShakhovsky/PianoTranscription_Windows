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
};

class FingersSort
{
public:
	FingersSort() = default;
	~FingersSort() = default;
	FingersSort(const FingersSort&) = default;
	FingersSort& operator = (const FingersSort&) = default;

	bool operator () (const pair<int16_t, char>& lhs, const pair<int16_t, char>& rhs) const
	{
		return lhs.second < rhs.second;
	}
};

class NotesSort
{
public:
	NotesSort() = default;
	~NotesSort() = default;
	NotesSort(const NotesSort&) = default;
	NotesSort& operator = (const NotesSort&) = default;

	bool operator () (const pair<int16_t, char>& lhs, const pair<int16_t, char>& rhs) const
	{
		return lhs.first < rhs.first;
	}
};

class SameNote
{
public:
	SameNote() = default;
	~SameNote() = default;
	SameNote(const SameNote&) = default;
	SameNote& operator = (const SameNote&) = default;

	bool operator () (const pair<int16_t, char>& lhs, const pair<int16_t, char>& rhs) const
	{
		return lhs.first == rhs.first;
	}
};

class SameFinger
{
public:
	SameFinger() = default;
	~SameFinger() = default;
	SameFinger(const SameFinger&) = default;
	SameFinger& operator = (const SameFinger&) = default;

	bool operator () (const pair<int16_t, char>& lhs, const pair<int16_t, char>& rhs) const
	{
		return lhs.second == rhs.second;
	}
};

// only asserts are tested:
float VerticalCost::Calculate(const vector<pair<int16_t, char>> notes_fingers)
{
	assert("NUMBER OF NOTES IN A CHORD MUST BE BETWEEN TWO AND FIVE"
		&& notes_fingers.size() >= 2 && notes_fingers.size() <= 5);

	assert("NOTES SHOULD BE SORTED IN ASCENDING ORDER BEFORE CHORD COST CALCULATION"
		&& is_sorted(notes_fingers.cbegin(), notes_fingers.cend(), NotesSort()));
	assert("ALL NOTES IN A CHORD MUST BE DIFFERENT"
		&& adjacent_find(notes_fingers.cbegin(), notes_fingers.cend(), SameNote()) == notes_fingers.cend());
# ifdef _DEBUG
	auto temp(notes_fingers);
	const auto tempEnd(remove_if(temp.begin(), temp.end(), IsThumb()));

	assert("ONLY THUMB PASSING IS ALLOWED BY CURRENT ALGORITHM" && is_sorted(temp.begin(), tempEnd, FingersSort()));
	assert("ALL FINGERS IN A CHORD MUST BE DIFFERENT" && adjacent_find(temp.begin(), tempEnd, SameFinger()) == tempEnd);
# endif
	assert("ALL FINGERS IN A CHORD MUST BE DIFFERENT"
		&& adjacent_find(notes_fingers.cbegin(), notes_fingers.cend(), SameFinger()) == notes_fingers.cend());
	assert("THERE IS ONLY ONE THUMB IN A NORMAL HAND"
		&& count_if(notes_fingers.cbegin(), notes_fingers.cend(), IsThumb()) <= 1);

	auto result(PolyCosts().CostOfPair(notes_fingers.at(0), notes_fingers.at(1)));
	if (notes_fingers.size() >= 3) result += PolyCosts().CostOfPair(notes_fingers.at(1), notes_fingers.at(2));
	if (notes_fingers.size() >= 4) result += PolyCosts().CostOfPair(notes_fingers.at(2), notes_fingers.at(3));
	if (notes_fingers.size() == 5) result += PolyCosts().CostOfPair(notes_fingers.at(3), notes_fingers.at(4));
	return result;
}