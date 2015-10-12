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

			GraphStruct::Graph_ GetGraph() const
			{
				return graph_;
			}
		private:
			GraphStruct::Graph_ graph_;
		};
	}
}