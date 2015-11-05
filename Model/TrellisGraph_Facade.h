# pragma once
# include "Fingering_Include\GraphStruct.h"

namespace Model
{
	namespace Fingering
	{
		class TrellisGraph : private boost::noncopyable
		{
			TrellisGraph() = delete;
		public:
			explicit TrellisGraph(std::vector<std::vector<int16_t>> chords, bool leftHand = false);
			~TrellisGraph() = default;

			std::vector<std::vector<GraphStruct::Chord_>> GetResult() const
			{
				return result_;
			}
		private:
			std::vector<std::vector<GraphStruct::Chord_>> result_;
		};
	}
}