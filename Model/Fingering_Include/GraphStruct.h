# pragma once

namespace Model
{
	namespace Fingering
	{
		namespace GraphStruct
		{
			typedef std::pair<int16_t, char>			Pitch_;		// pair of note and finger
			typedef std::vector<Pitch_>					Chord_;		// one to five notes and fingers
			typedef std::pair<Chord_, float>			Node_;		// pair of chord and its static cost
			typedef std::vector<std::shared_ptr<Node_>>	NodeList_;	// Graph path or chord variations
			typedef std::vector<NodeList_>				Graph_;
		}
	}
}