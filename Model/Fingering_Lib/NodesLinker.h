# pragma once
# include "..\Fingering_Include\GraphStruct.h"

namespace Model
{
	namespace Fingering
	{
		class NodesLinker : private boost::noncopyable
		{
			GraphStruct::NodeList_ chordVariations_;

			GraphStruct::Graph_ graph_;
			std::vector<GraphStruct::Graph_::const_iterator> minPaths_;
		public:
			NodesLinker() : noncopyable(),
				chordVariations_(),
				graph_(),
				minPaths_()
			{}
			~NodesLinker() = default;

			GraphStruct::Graph_ GetResultedGraph() const
			{
				return graph_;
			}
			void RemoveExpensivePaths();

			void LinkNewNodes(std::vector<int16_t> chord);
		private:
			float MinPathFinder(std::shared_ptr<GraphStruct::Node_>);
			void MinValCompare(float* minVal, float* newVal, GraphStruct::Graph_::const_iterator path);
		};
	}
}