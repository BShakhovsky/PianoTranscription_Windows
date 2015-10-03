# include "stdafx.h"
# include "CostRules.h"
# include "DistanceTable.h"
# include "..\MidiParserLib\NoteNames.h"

using std::pair;
using namespace Model;
using namespace Fingering;
using namespace MidiParser;

int CostRules::Rule1_StretchComf(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	auto distance(note2.first - note1.first);
	if (note1.second == note2.second) return NULL;
	else if (note1.second > note2.second) distance *= -1;

	if (distance < DistanceTable::MinComf(note1.second, note2.second))
		return (DistanceTable::MinComf(note1.second, note2.second) - distance) * 2;
	else if (distance > DistanceTable::MaxComf(note1.second, note2.second))
		return (distance - DistanceTable::MaxComf(note1.second, note2.second)) * 2;
	else return NULL;
}
int CostRules::Rule2_SpanRel(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	auto distance(note2.first - note1.first);
	if (note1.second == note2.second) return NULL;
	else if (note1.second > note2.second) distance *= -1;

	auto coeff(NULL);
	if (1 == note1.second || 1 == note2.second) coeff = 1;
	else										coeff = 2;

	if (distance < DistanceTable::MinRel(note1.second, note2.second))
		return (DistanceTable::MinRel(note1.second, note2.second) - distance) * coeff;
	else if (distance > DistanceTable::MaxRel(note1.second, note2.second))
		return (distance - DistanceTable::MaxRel(note1.second, note2.second)) * coeff;
	else return NULL;
}

// not covered by unit tests:
char CostRules::Rule3_PositionChange(const pair<int16_t, char> note1, const pair<int16_t, char> note2,
	const pair<int16_t, char> note3)
{
	auto result('\0');

	auto distance(note3.first - note1.first);
	if (note1.second == note2.second || note2.second == note3.second ||
		note3.second == note1.second)									return NULL;
	else if (note1.second > note3.second)								distance *= -1;

	if (distance < DistanceTable::MinComf(note1.second, note3.second) ||
		distance > DistanceTable::MaxComf(note1.second, note3.second))	++result;	// half-change
	if (1 == note2.second && (
		distance < DistanceTable::MinPrac(note1.second, note3.second) ||
		distance > DistanceTable::MaxPrac(note1.second, note3.second)
		) && (note1.first < note2.first && note2.first < note3.first ||
			note1.first > note2.first && note2.first > note3.first))	++result;	// full-change
	if (note1.first == note3.first /*&& note1.second != note3.second*/)	++result;

	return result;
}
char CostRules::Rule4_PositionSize(const pair<int16_t, char> note1, const pair<int16_t, char> note3)
{
	auto distance(note3.first - note1.first);
	if (note1.second == note3.second) return NULL;
	else if (note1.second > note3.second) distance *= -1;

	if (distance < DistanceTable::MinComf(note1.second, note3.second))
		return (DistanceTable::MinComf(note1.second, note3.second) - distance);
	else if (distance > DistanceTable::MaxComf(note1.second, note3.second))
		return (distance - DistanceTable::MaxComf(note1.second, note3.second));
	else return NULL;
}

char CostRules::Rule5_WeakFinger(const char finger)
{
	switch (finger)
	{
	case 1:	case 2:	case 3: return NULL;	break;
	case 4:	case 5:			return 1;		break;
	default: assert(!"WRONG FINGER NUMBER"); return NULL;
	}
}
char CostRules::Rule6_ThreeFourFive(const char finger1, const char finger2, const char finger3)
{
	if		(3 == finger1 && 4 == finger2 && 5 == finger3)	return 1;
	else if (3 == finger1 && 5 == finger2 && 4 == finger3)	return 1;
	else if (4 == finger1 && 5 == finger2 && 3 == finger3)	return 1;
	else if (4 == finger1 && 3 == finger2 && 5 == finger3)	return 1;
	else if (5 == finger1 && 3 == finger2 && 4 == finger3)	return 1;
	else if (5 == finger1 && 4 == finger2 && 3 == finger3)	return 1;
	else													return NULL;
}
char CostRules::Rule7_ThreeFour(const char finger1, const char finger2)
{
	if		(3 == finger1 && 4 == finger2)	return 1;
	else if (4 == finger1 && 3 == finger2)	return 1;
	else									return NULL;
}

char CostRules::Rule8_FourOnBlack(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	if		(3 == note1.second && 4 == note2.second
		&& NoteNames::IsWhite(note1.first) && NoteNames::IsBlack(note2.first))	return 1;
	else if (4 == note1.second && 3 == note2.second
		&& NoteNames::IsBlack(note1.first) && NoteNames::IsWhite(note2.first))	return 1;
	else																		return NULL;
}
float CostRules::Rule9_ThumbOnBlack(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	auto result(0.f);

	if (1 == note1.second && NoteNames::IsBlack(note1.first))
	{
		result += 0.5;
		if (1 != note2.second && NoteNames::IsWhite(note2.first))
			++result;
	}
	else if (1 == note2.second && NoteNames::IsBlack(note2.first))
	{
		result += 0.5;
		if (1 != note1.second && NoteNames::IsWhite(note1.first))
			++result;
	}

	return result;
}
char CostRules::Rule10_PinkyOnBlack(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	if		(5 == note1.second && NoteNames::IsBlack(note1.first)
		&& 5 != note2.second && NoteNames::IsWhite(note2.first))	return 1;
	else if (5 == note2.second && NoteNames::IsBlack(note2.first)
		&& 5 != note1.second && NoteNames::IsWhite(note1.first))	return 1;
	else															return NULL;
}

char CostRules::Rule11_ThumbPassing(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	auto distance(note2.first - note1.first);
	if 		(note1.second == note2.second)	return NULL;
	else if (note1.second > note2.second)	distance *= -1;

	if (distance < 0 && (1 == note1.second || 1 == note2.second) && (
		NoteNames::IsWhite(note1.first) && NoteNames::IsWhite(note2.first) ||
		NoteNames::IsBlack(note1.first) && NoteNames::IsBlack(note2.first)))	return 1;
	else																		return NULL;
}
char CostRules::Rule12_ThumbCross_Black(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	auto distance(note2.first - note1.first);
	if (note1.second == note2.second)		return NULL;
	else if (note1.second > note2.second)	distance *= -1;

	if (distance < 0)
		if		(1 == note1.second && NoteNames::IsBlack(note1.first) && NoteNames::IsWhite(note2.first))	return 1;
		else if (1 == note2.second && NoteNames::IsWhite(note1.first) && NoteNames::IsBlack(note2.first))	return 1;
		else																								return NULL;
	else																									return NULL;
}

char CostRules::Rule13_ThreeSameFinger(const pair<int16_t, char> note1, const pair<int16_t, char> note2,
	const pair<int16_t, char> note3)
{
	if (note1.second == note3.second && (
		note1.first < note2.first && note2.first < note3.first ||
		note1.first > note2.first && note2.first > note3.first))	return 1;
	else															return NULL;
}

int CostRules::Rule14_MaxPractical(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	auto distance(note2.first - note1.first);
	if (note1.second == note2.second) return NULL;
	else if (note1.second > note2.second) distance *= -1;

	if (distance < DistanceTable::MinPrac(note1.second, note2.second))
		return (DistanceTable::MinPrac(note1.second, note2.second) - distance) * 10;
	else if (distance > DistanceTable::MaxPrac(note1.second, note2.second))
		return (distance - DistanceTable::MaxPrac(note1.second, note2.second)) * 10;
	else return NULL;
}
char CostRules::Rule15_SameNote(const pair<int16_t, char> note1, const pair<int16_t, char> note2)
{
	if (note1.first == note2.first && note1.second != note2.second)	return 1;
	else															return NULL;
}