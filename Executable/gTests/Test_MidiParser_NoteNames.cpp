# include "stdafx.h"
# include "..\..\Model\MidiParser_Include\NoteNames.h"

using namespace Model::MidiParser;

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
	ASSERT_EQ(NOTE_NAMES::C,	NoteNames::GetNoteName(0));
	ASSERT_EQ(NOTE_NAMES::C1,	NoteNames::GetNoteName(13));
	ASSERT_EQ(NOTE_NAMES::D,	NoteNames::GetNoteName(26));
	ASSERT_EQ(NOTE_NAMES::D1,	NoteNames::GetNoteName(39));
	ASSERT_EQ(NOTE_NAMES::E,	NoteNames::GetNoteName(52));
	ASSERT_EQ(NOTE_NAMES::F,	NoteNames::GetNoteName(65));
	ASSERT_EQ(NOTE_NAMES::F1,	NoteNames::GetNoteName(78));
	ASSERT_EQ(NOTE_NAMES::G,	NoteNames::GetNoteName(91));
	ASSERT_EQ(NOTE_NAMES::G1,	NoteNames::GetNoteName(104));
	ASSERT_EQ(NOTE_NAMES::A,	NoteNames::GetNoteName(117));
	ASSERT_EQ(NOTE_NAMES::A1,	NoteNames::GetNoteName(130));
	ASSERT_EQ(NOTE_NAMES::B,	NoteNames::GetNoteName(143));
}

TEST(Notes, WhiteBlack)
{
	ASSERT_DEBUG_DEATH(NoteNames::IsWhite(-1), "NOTE MUST BE A POSITIVE NUMBER");
	ASSERT_DEBUG_DEATH(NoteNames::IsBlack(-1), "NOTE MUST BE A POSITIVE NUMBER");

	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::C)));
	ASSERT_TRUE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::C1)));
	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::D)));
	ASSERT_TRUE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::D1)));
	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::E)));
	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::F)));
	ASSERT_TRUE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::F1)));
	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::G)));
	ASSERT_TRUE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::G1)));
	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::A)));
	ASSERT_TRUE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::A1)));
	ASSERT_TRUE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::B)));

	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::C)));
	ASSERT_FALSE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::C1)));
	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::D)));
	ASSERT_FALSE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::D1)));
	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::E)));
	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::F)));
	ASSERT_FALSE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::F1)));
	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::G)));
	ASSERT_FALSE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::G1)));
	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::A)));
	ASSERT_FALSE(NoteNames::IsWhite(static_cast<int16_t>(NOTE_NAMES::A1)));
	ASSERT_FALSE(NoteNames::IsBlack(static_cast<int16_t>(NOTE_NAMES::B)));
}