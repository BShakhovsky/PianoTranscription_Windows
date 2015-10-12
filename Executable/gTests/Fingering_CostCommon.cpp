# include "stdafx.h"
# include "Fingering_CostCommon.h"
# include "..\..\Model\MidiParser_Include\NoteNames.h"

using testing::Test;
using Model::MidiParser::NoteNames;
using gTest::CostCommon;

CostCommon::CostCommon() : Test(),
	whiteNote(NULL),
	blackNote(NULL),
	randNote1(NULL),
	randNote2(NULL),
	distance(NULL),
	randFinger1('\0'),
	randFinger2('\0'),
	nonThumb('\0')
{
	srand(static_cast<unsigned>(time(NULL)));

	while (NoteNames::IsBlack(whiteNote = static_cast<int16_t>(rand() % (INT16_MAX - 30) + 15)));
	while (NoteNames::IsWhite(blackNote = static_cast<int16_t>(rand() % (INT16_MAX - 30) + 15)));

	randNote1 = static_cast<int16_t>(rand() % (INT16_MAX - 30) + 15);
	randNote2 = static_cast<int16_t>(rand() % (INT16_MAX - 30) + 15);

	randFinger1 = static_cast<char>(rand() % 5 + 1);
	randFinger2 = static_cast<char>(rand() % 5 + 1);
	nonThumb = static_cast<char>(rand() % 4 + 2);

	distance = randFinger1 > randFinger2 ? randNote1 - randNote2 : randNote2 - randNote1;
}