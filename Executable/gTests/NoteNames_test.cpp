# include "stdafx.h"
# include "..\..\Model\MidiParserLib\NoteNames.h"

using Model::MidiParser::NoteNames;

TEST(Notes, OctaveNo)
{
	ASSERT_DEBUG_DEATH(NoteNames::GetOctaveNumber(-1), "NOTE MUST BE A POSITIVE NUMBER");
	ASSERT_EQ(0, NoteNames::GetOctaveNumber(0));
	ASSERT_EQ(0, NoteNames::GetOctaveNumber(6));
	ASSERT_EQ(1, NoteNames::GetOctaveNumber(12));
}

TEST(Notes, NoteName)
{
	ASSERT_DEBUG_DEATH(NoteNames::GetNoteName(-1), "NOTE MUST BE A POSITIVE NUMBER");
	ASSERT_STREQ("C",	NoteNames::GetNoteName(0));
	ASSERT_STREQ("C#",	NoteNames::GetNoteName(13));
	ASSERT_STREQ("D",	NoteNames::GetNoteName(26));
	ASSERT_STREQ("D#",	NoteNames::GetNoteName(39));
	ASSERT_STREQ("E",	NoteNames::GetNoteName(52));
	ASSERT_STREQ("F",	NoteNames::GetNoteName(65));
	ASSERT_STREQ("F#",	NoteNames::GetNoteName(78));
	ASSERT_STREQ("G",	NoteNames::GetNoteName(91));
	ASSERT_STREQ("G#",	NoteNames::GetNoteName(104));
	ASSERT_STREQ("A",	NoteNames::GetNoteName(117));
	ASSERT_STREQ("A#",	NoteNames::GetNoteName(130));
	ASSERT_STREQ("B",	NoteNames::GetNoteName(143));
}