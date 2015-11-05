# include "stdafx.h"
# include "..\..\Model\Fingering_Lib\BlackWhiteKeys.h"

using Model::Fingering::BlackWhiteKeys;

TEST(Keys, WhiteBlack)
{
	ASSERT_DEBUG_DEATH(BlackWhiteKeys::IsWhite(-1), "NOTE MUST BE A POSITIVE NUMBER");
	ASSERT_DEBUG_DEATH(BlackWhiteKeys::IsBlack(-1), "NOTE MUST BE A POSITIVE NUMBER");

	enum class NOTE_NAMES { C, C1, D, D1, E, F, F1, G, G1, A, A1, B };

	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::C)));
	ASSERT_TRUE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::C1)));
	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::D)));
	ASSERT_TRUE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::D1)));
	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::E)));
	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::F)));
	ASSERT_TRUE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::F1)));
	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::G)));
	ASSERT_TRUE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::G1)));
	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::A)));
	ASSERT_TRUE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::A1)));
	ASSERT_TRUE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::B)));

	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::C)));
	ASSERT_FALSE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::C1)));
	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::D)));
	ASSERT_FALSE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::D1)));
	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::E)));
	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::F)));
	ASSERT_FALSE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::F1)));
	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::G)));
	ASSERT_FALSE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::G1)));
	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::A)));
	ASSERT_FALSE(BlackWhiteKeys::IsWhite(static_cast<int16_t>(NOTE_NAMES::A1)));
	ASSERT_FALSE(BlackWhiteKeys::IsBlack(static_cast<int16_t>(NOTE_NAMES::B)));
}