# pragma once
namespace Model {
	namespace Fingering {
		class CostRules abstract : private boost::noncopyable {
			CostRules() = delete; ~CostRules() = delete;
		public:
// http://www.scriptiebank.be/sites/default/files/webform/scriptie/balliauw-matteo-2014_0.pdf
// Page 16, Table 2
			static int Rule1_StretchComf(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);
			static int Rule2_SpanRel(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);

			static int Rule3_PositionChange(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2,
				std::pair<int16_t, char> note3);
		private:
			static int Rule4_PositionSize(std::pair<int16_t, char> note1, std::pair<int16_t, char> note3)
			{
				return Rule1_StretchComf(note1, note3) / 2;
			}
		public:
			static char Rule5_WeakFinger(char finger);
// http://www.csc.kth.se/utbildning/kth/kurser/DD143X/dkand13/Group7Anders/final/Bassam.Alfarhan.David.Sandberg.report.pdf
// Page 6
			static char Rule6_ThreeFourFive(char finger1, char finger2, char finger3);
			static char Rule7_ThreeFour(char finger1, char finger2);

			static char Rule8_FourOnBlack(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);
			static float Rule9_ThumbOnBlack(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);
			static char Rule10_PinkyOnBlack(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);

			static char Rule11_ThumbPassing(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);
			static char Rule12_ThumbCross_Black(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);

			static char Rule13_ThreeSameFinger(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2,
				std::pair<int16_t, char> note3);

			static int Rule14_MaxPractical(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);
			static char Rule15_SameNote(std::pair<int16_t, char> note1, std::pair<int16_t, char> note2);
		};
	}
}