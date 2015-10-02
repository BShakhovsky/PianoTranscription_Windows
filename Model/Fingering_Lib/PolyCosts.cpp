# include "stdafx.h"
# include "PolyCosts.h"
# include "CostRules.h"

using std::pair;
using namespace Model::Fingering;

int PolyCosts::CostOfPair_impl(const pair<int16_t, char> note1, const pair<int16_t, char> note2) const
{
	assert("FINGER NUMBERS SHOULD BE NON-ZERO" && note1.second && note2.second);

	return CostRules::Rule1_StretchComf(note1, note2) * 2
		+ CostRules::Rule2_SpanRel(note1, note2) * 2
		+ CostRules::Rule14_MaxPractical(note1, note2);
}