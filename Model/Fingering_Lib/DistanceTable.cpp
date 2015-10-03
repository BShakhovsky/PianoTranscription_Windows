# include "stdafx.h"
# include "DistanceTable.h"

using Model::Fingering::DistanceTable;

// http://www.diva-portal.org/smash/get/diva2:769592/FULLTEXT01.pdf
// Page 9, Table 1
const char DistanceTable::distanceMatrix_[10][6] =
{
	{ -5, -3, 1, 5, 8, 10 },
	{ -4, -2, 3, 7, 10, 12 },
	{ -3, -1, 5, 9, 12, 14 },
	{ -1, 1, 7, 10, 13, 15 },
	{ 1, 1, 1, 2, 3, 5 },
	{ 1, 1, 3, 4, 5, 7 },
	{ 2, 2, 5, 6, 8, 10 },
	{ 1, 1, 1, 2, 2, 4 },
	{ 1, 1, 3, 4, 5, 7 },
	{ 1, 1, 1, 2, 3, 5 }
};

int DistanceTable::TableRow(const char finger1, const char finger2)
{
	assert("WRONG FINGER" && finger1 > 0 && finger1 <= 5 && finger2 > 0 && finger2 <= 5);
	auto fingerPair('\0');
	if (finger1 < finger2) fingerPair = finger1 * 10 + finger2;
	else if (finger1 > finger2) fingerPair = finger2 * 10 + finger1;
	else assert(!"FINGER PAIR MUST CONSIST OF TWO DIFFERENT FINGERS");

	switch (fingerPair)
	{
	case 12: return NULL;
	case 13: return 1;
	case 14: return 2;
	case 15: return 3;

	case 23: return 4;
	case 24: return 5;
	case 25: return 6;

	case 34: return 7;
	case 35: return 8;

	case 45: return 9;

	default: assert(!"WRONG FINGER PAIR"); return -1;
	}
}