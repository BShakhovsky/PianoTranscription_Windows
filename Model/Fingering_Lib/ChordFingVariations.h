# pragma once
# include "..\Fingering_Include\GraphStruct.h"

namespace Model
{
	namespace Fingering
	{
		class ChordFingVariations abstract : private boost::noncopyable
		{
			ChordFingVariations() = delete;
			~ChordFingVariations() = delete;
		public:
			static GraphStruct::NodeList_ CreateCombinations(std::vector<int16_t>);
		private:
			static std::vector<std::vector<char>> GetFingerCombinations(size_t numberOfFingers);

			static std::vector<std::vector<char>> OneFinger();
			static std::vector<std::vector<char>> TwoFingers();
			static std::vector<std::vector<char>> ThreeFingers();
			static std::vector<std::vector<char>> FourFingers();
			static std::vector<std::vector<char>> FiveFingers();
		};
	}
}