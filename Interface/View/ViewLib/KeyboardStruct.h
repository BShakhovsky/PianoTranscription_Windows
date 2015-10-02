# pragma once

namespace Interface
{
	namespace View
	{
		class KeyboardStruct abstract : private boost::noncopyable
		{
			/*************************************************************************************
			C#  D#      F#  G#  A#      C#  D#      F#  G#  A#      C#  D#      F#  G#  A#
			|   |       |   |   |       |   |       |   |   |       |   |       |   |   |
			### ###     ### ### ###     ### ###     ### ### ###     ### ###     ### ### ###
			# # # #     # # # # # #     # # # #     # # # # # #     # # # #     # # # # # #
			# # # #     # # # # # #     # # # #     # # # # # #     # # # #     # # # # # #
			# # # #     # # # # # #     # # # #     # # # # # #     # # # #     # # # # # #
			+--# #-# #--+--# #-# #-# #--+--# #-# #--+--# #-# #-# #--+--# #-# #--+--# #-# #-# #--+
			|  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |
			|  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |
			|  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |
			|  ### ###  |  ### ### ###  |  ### ###  |  ### ### ###  |  ### ###  |  ### ### ###  |
			|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
			|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
			|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
			|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
			|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
			+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
			C   D   E   F   G   A   B   C   D   E   F   G   A   B   C   D   E   F   G   A   B
			************************************************************************************/

			static constexpr unsigned
				KB_HEIGHT_ = 18,
				OCTAVE_WIDTH_ = 7 * 4 + 1;		// 7 white keys, 4 blocks each
			static const unsigned NUM_OCTAVES_;	// octaves 0...10

			static char C1_1, C1_2, D1_1, D1_2, F1_1, F1_2, G1_1, G1_2, A1_1, A1_2,
				C2_1, D2_1, F2_1, G2_1, A2_1, C3_1, D3_1, F3_1, G3_1, A3_1,
				C1, D1, E1, F1, G1, A1, B1, C2, D2, E2, F2, G2, A2, B2, C3, D3, E3, F3, G3, A3, B3;
			static constexpr char sp = ' ', sh = '#', ii = '|', xx = '+';

			static const char* keyboard_[KB_HEIGHT_][OCTAVE_WIDTH_];
			static bool* pressedNotes_[];
		public:
			static void PressKey(int16_t note);
			static void ReleaseKey(int16_t note);
			static void ResetKeys();
			static void PrintKeyboard(HANDLE hStdOut);
		private:
			static void PrintKeybPart(SHORT row, unsigned octave, unsigned columnStart, unsigned columnEnd);
			static void HighlightKey(int16_t note);

			KeyboardStruct() = delete;
			~KeyboardStruct() = delete;
		};
	}
}