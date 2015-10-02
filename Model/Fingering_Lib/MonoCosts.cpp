# include "stdafx.h"
# include "MonoCosts.h"
# include "CostRules.h"

using std::pair;
using namespace Model::Fingering;

int MonoCosts::CostOfSingle_impl(const char finger1) const
{
	assert("FINGER NUMBERS SHOULD BE NON-ZERO" && finger1);
	return CostRules::Rule5_WeakFinger(finger1);
}

int MonoCosts::CostOfPair_impl(const pair<int16_t, char> note1, const std::pair<int16_t, char> note2) const
{
	assert("FINGER NUMBERS SHOULD BE NON-ZERO" && note1.second && note2.second);

	return static_cast<int>(
		CostRules::Rule1_StretchComf(note1, note2)
		+ CostRules::Rule2_SpanRel(note1, note2)
		+ CostRules::Rule7_ThreeFour(note1.second, note2.second)
		+ CostRules::Rule8_FourOnBlack(note1, note2)
		+ CostRules::Rule9_ThumbOnBlack(note1, note2)	// float
		+ CostRules::Rule10_PinkyOnBlack(note1, note2)
		+ CostRules::Rule11_ThumbPassing(note1, note2)
		+ CostRules::Rule12_ThumbCross_Black(note1, note2)
		+ CostRules::Rule14_MaxPractical(note1, note2)
		+ CostRules::Rule15_SameNote(note1, note2));
}

int MonoCosts::CostOfThree_impl(const pair<int16_t, char> note1, const pair<int16_t, char> note2,
	const pair<int16_t, char> note3) const
{
	assert("FINGER NUMBERS SHOULD BE NON-ZERO" && note1.second && note2.second && note3.second);

	return CostRules::Rule3_PositionChange(note1, note2, note3)
		+ CostRules::Rule4_PositionSize(note1, note3)
		+ CostRules::Rule6_ThreeFourFive(note1.second, note2.second, note3.second)
		+ CostRules::Rule13_ThreeSameFinger(note1, note2, note3);
}