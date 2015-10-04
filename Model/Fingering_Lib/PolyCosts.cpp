# include "stdafx.h"
# include "PolyCosts.h"
# include "CostRules.h"

using std::pair;
using namespace Model::Fingering;

float PolyCosts::CostOfPair_impl(const pair<int16_t, char> note1, const pair<int16_t, char> note2) const
{
	return static_cast<float>(	// otherwise warning 4244
		CostRules::Rule1_StretchComf(note1, note2) * 2
		+ CostRules::Rule2_SpanRel(note1, note2) * 2
		+ CostRules::Rule14_MaxPractical(note1, note2));
}