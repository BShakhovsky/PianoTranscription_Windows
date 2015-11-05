# include "stdafx.h"
# include "KeyboardStruct.h"
# include "NoteNames.h"

using namespace std;
using namespace Interface::View;

/******************************************************************************************
-  b1    aa  aa      aa  aa  aa      aa  aa      aa  aa  aa      aa  aa      aa  aa  aa
-  b2     |   |       |   |   |       |   |       |   |   |       |   |       |   |   |
-		 ### ###     ### ### ###     ### ###     ### ### ###     ### ###     ### ### ###
-  b3    ### # #     # # ### # #     # # # #     # # # # # #     # # # #     # # # # # #
w1 b3 +==###-# #--+--# #=###-# #--+--# #-# #--+--# #-# #-# #--+--# #-# #--+--# #-# #-# #--+
w2 b3 |xx### # #  |  # #x### # #  |  # # # #  |  # # # # # #  |  # # # #  |  # # # # # #  |
w2    |xx### ###  |  ###x### ###  |  ### ###  |  ### ### ###  |  ### ###  |  ### ### ###  |
w2    |xxx|   |   |   |xxx|   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |   |
w1    +===+---+---+---+===+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
w3      C   D   E   F   G   A   B   C   D   E   F   G   A   B   C   D   E   F   G   A   B
******************************************************************************************/

const unsigned KeyboardStruct::NUM_OCTAVES_ = 0x80 / NoteNames::NUM_NOTES + 1;	// octaves 0...10

char
KeyboardStruct::C1_1 = 'C', KeyboardStruct::D1_1 = 'D', KeyboardStruct::F1_1 = 'F', KeyboardStruct::G1_1 = 'G', KeyboardStruct::A1_1 = 'A',
KeyboardStruct::C1_2 = '#', KeyboardStruct::D1_2 = '#', KeyboardStruct::F1_2 = '#', KeyboardStruct::G1_2 = '#', KeyboardStruct::A1_2 = '#',
KeyboardStruct::C2_1 = '|', KeyboardStruct::D2_1 = '|', KeyboardStruct::F2_1 = '|', KeyboardStruct::G2_1 = '|', KeyboardStruct::A2_1 = '|',
KeyboardStruct::C3_1 = ' ', KeyboardStruct::D3_1 = ' ', KeyboardStruct::F3_1 = ' ', KeyboardStruct::G3_1 = ' ', KeyboardStruct::A3_1 = ' ',
KeyboardStruct::C1 = '=', KeyboardStruct::D1 = '=', KeyboardStruct::E1 = '=', KeyboardStruct::F1 = '=', KeyboardStruct::G1 = '=', KeyboardStruct::A1 = '=', KeyboardStruct::B1 = '=',
KeyboardStruct::C2 = ' ', KeyboardStruct::D2 = ' ', KeyboardStruct::E2 = ' ', KeyboardStruct::F2 = ' ', KeyboardStruct::G2 = ' ', KeyboardStruct::A2 = ' ', KeyboardStruct::B2 = ' ',
KeyboardStruct::C3 = 'C', KeyboardStruct::D3 = 'D', KeyboardStruct::E3 = 'E', KeyboardStruct::F3 = 'F', KeyboardStruct::G3 = 'G', KeyboardStruct::A3 = 'A', KeyboardStruct::B3 = 'B';

const char* KeyboardStruct::keyboard_[KB_HEIGHT_][OCTAVE_WIDTH_] =
{
	{ &sp,&sp,&sp,&C1_1,&C1_2,&sp,&sp,&D1_1,&D1_2,&sp,&sp,&sp,&sp,&sp,&sp,&F1_1,&F1_2,&sp,&sp,&G1_1,&G1_2,&sp,&sp,&A1_1,&A1_2,&sp,&sp,&sp,&sp },
	{ &sp,&sp,&sp,&sp,  &C2_1,&sp,&sp,&sp,  &D2_1,&sp,&sp,&sp,&sp,&sp,&sp,&sp,  &F2_1,&sp,&sp,&sp,  &G2_1,&sp,&sp,&sp,  &A2_1,&sp,&sp,&sp,&sp },
	{ &sp,&sp,&sp,&sh,&sh,    &sh,&sp,&sh,&sh,    &sh,&sp,&sp,&sp,&sp,&sp,&sh,&sh,    &sh,&sp,&sh,&sh,    &sh,&sp,&sh,&sh,    &sh,&sp,&sp,&sp },
	{ &sp,&sp,&sp,&sh,&C3_1,  &sh,&sp,&sh,&D3_1,  &sh,&sp,&sp,&sp,&sp,&sp,&sh,&F3_1,  &sh,&sp,&sh,&G3_1,  &sh,&sp,&sh,&A3_1,  &sh,&sp,&sp,&sp },
	{ &sp,&sp,&sp,&sh,&C3_1,  &sh,&sp,&sh,&D3_1,  &sh,&sp,&sp,&sp,&sp,&sp,&sh,&F3_1,  &sh,&sp,&sh,&G3_1,  &sh,&sp,&sh,&A3_1,  &sh,&sp,&sp,&sp },
	{ &sp,&sp,&sp,&sh,&C3_1,  &sh,&sp,&sh,&D3_1,  &sh,&sp,&sp,&sp,&sp,&sp,&sh,&F3_1,  &sh,&sp,&sh,&G3_1,  &sh,&sp,&sh,&A3_1,  &sh,&sp,&sp,&sp },
	{ &xx,&C1,&C1,&sh,&C3_1,  &sh,&D1,&sh,&D3_1,  &sh,&E1,&E1,&xx,&F1,&F1,&sh,&F3_1,  &sh,&G1,&sh,&G3_1,  &sh,&A1,&sh,&A3_1,  &sh,&B1,&B1,&xx },
	{ &ii,&C2,&C2,&sh,&C3_1,  &sh,&D2,&sh,&D3_1,  &sh,&E2,&E2,&ii,&F2,&F2,&sh,&F3_1,  &sh,&G2,&sh,&G3_1,  &sh,&A2,&sh,&A3_1,  &sh,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&sh,&C3_1,  &sh,&D2,&sh,&D3_1,  &sh,&E2,&E2,&ii,&F2,&F2,&sh,&F3_1,  &sh,&G2,&sh,&G3_1,  &sh,&A2,&sh,&A3_1,  &sh,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&sh,&C3_1,  &sh,&D2,&sh,&D3_1,  &sh,&E2,&E2,&ii,&F2,&F2,&sh,&F3_1,  &sh,&G2,&sh,&G3_1,  &sh,&A2,&sh,&A3_1,  &sh,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&sh,&sh,    &sh,&D2,&sh,&sh,    &sh,&E2,&E2,&ii,&F2,&F2,&sh,&sh,    &sh,&G2,&sh,&sh,    &sh,&A2,&sh,&sh,    &sh,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&C2,        &ii,&D2,&D2,&D2,&ii,&E2,&E2,&E2,&ii,&F2,&F2,&F2,&ii,    &G2,&G2,&G2,&ii,    &A2,&A2,&A2,&ii,    &B2,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&C2,        &ii,&D2,&D2,&D2,&ii,&E2,&E2,&E2,&ii,&F2,&F2,&F2,&ii,    &G2,&G2,&G2,&ii,    &A2,&A2,&A2,&ii,    &B2,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&C2,        &ii,&D2,&D2,&D2,&ii,&E2,&E2,&E2,&ii,&F2,&F2,&F2,&ii,    &G2,&G2,&G2,&ii,    &A2,&A2,&A2,&ii,    &B2,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&C2,        &ii,&D2,&D2,&D2,&ii,&E2,&E2,&E2,&ii,&F2,&F2,&F2,&ii,    &G2,&G2,&G2,&ii,    &A2,&A2,&A2,&ii,    &B2,&B2,&B2,&ii },
	{ &ii,&C2,&C2,&C2,        &ii,&D2,&D2,&D2,&ii,&E2,&E2,&E2,&ii,&F2,&F2,&F2,&ii,    &G2,&G2,&G2,&ii,    &A2,&A2,&A2,&ii,    &B2,&B2,&B2,&ii },
	{ &xx,&C1,&C1,&C1,        &xx,&D1,&D1,&D1,&xx,&E1,&E1,&E1,&xx,&F1,&F1,&F1,&xx,    &G1,&G1,&G1,&xx,    &A1,&A1,&A1,&xx,    &B1,&B1,&B1,&xx },
	{ &sp,&sp,&C3,&sp,        &sp,&sp,&D3,&sp,&sp,&sp,&E3,&sp,&sp,&sp,&F3,&sp,&sp,    &sp,&G3,&sp,&sp,    &sp,&A3,&sp,&sp,    &sp,&B3,&sp,&sp }
};

static bool octave0[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave1[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave2[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave3[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave4[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave5[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave6[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave7[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
static bool octave8[NoteNames::NUM_NOTES] = { false, false, false, false, false, false, false, false, false, false, false, false };
bool* KeyboardStruct::pressedNotes_[NUM_OCTAVES_] =
{
	octave0, octave1, octave2, octave3, octave4, octave5, octave6, octave7, octave8	// 9th and 10th are unused
};

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
void KeyboardStruct::PressKey(const int16_t note)
{
	pressedNotes_[NoteNames::GetOctaveNumber(note)][note % NoteNames::NUM_NOTES] = true;
}
void KeyboardStruct::ReleaseKey(const int16_t note)
{
	pressedNotes_[NoteNames::GetOctaveNumber(note)][note % NoteNames::NUM_NOTES] = false;
}
# pragma warning(pop)
void KeyboardStruct::ResetKeys()
{
	C1_1 = D1_1 = F1_1 = G1_1 = A1_1 =
		C1_2 = D1_2 = F1_2 = G1_2 = A1_2 =
		C2_1 = D2_1 = F2_1 = G2_1 = A2_1 =
		C3_1 = D3_1 = F3_1 = G3_1 = A3_1 =
		' ';
	C1 = D1 = E1 = F1 = G1 = A1 = B1 =
		'-';
	C2 = D2 = E2 = F2 = G2 = A2 = B2 =
		C3 = D3 = E3 = F3 = G3 = A3 = B3 =
		' ';
}


void KeyboardStruct::PrintKeyboard(HANDLE hStdOut)
{
	for (SHORT row(NULL); row < KB_HEIGHT_; ++row)
	{
		SetConsoleCursorPosition(hStdOut, { 5i16, 5i16 + row });

		PrintKeybPart(row, 1, OCTAVE_WIDTH_ - 7, OCTAVE_WIDTH_);
		for (unsigned octave(2); octave < 7; ++octave)
			PrintKeybPart(row, octave, NULL, OCTAVE_WIDTH_);
		PrintKeybPart(row, 7, NULL, 3);
	}
	cout << "\n\n\n\n\n" << endl;
}

void KeyboardStruct::PrintKeybPart(const SHORT row, const unsigned octave, const unsigned colStart, const unsigned colEnd)
{
	ResetKeys();
	for (int16_t i(NULL); i < NoteNames::NUM_NOTES; ++i)	if (pressedNotes_[octave][i]) HighlightKey(i);
	for (auto column(colStart); column < colEnd; ++column)	cout << *keyboard_[row][column];
}

void KeyboardStruct::HighlightKey(const int16_t note)
{
	switch (note)
	{
	case 0:		C1		= '='; C2	= 'x'; C3	= 'C';				break;
	case 1:		C1_1	= 'C'; C1_2 = '#'; C2_1 = '|'; C3_1 = 'x';	break;
	case 2:		D1		= '='; D2	= 'x'; D3	= 'D';				break;
	case 3:		D1_1	= 'D'; D1_2 = '#'; D2_1 = '|'; D3_1 = 'x';	break;
	case 4:		E1		= '='; E2	= 'x'; E3	= 'E';				break;
	case 5:		F1		= '='; F2	= 'x'; F3	= 'F';				break;
	case 6:		F1_1	= 'F'; F1_2 = '#'; F2_1 = '|'; F3_1 = 'x';	break;
	case 7:		G1		= '='; G2	= 'x'; G3	= 'G';				break;
	case 8:		G1_1	= 'G'; G1_2 = '#'; G2_1 = '|'; G3_1 = 'x';	break;
	case 9:		A1		= '='; A2	= 'x'; A3	= 'A';				break;
	case 10:	A1_1	= 'A'; A1_2 = '#'; A2_1 = '|'; A3_1 = 'x';	break;
	case 11:	B1		= '='; B2	= 'x'; B3	= 'B';				break;
	default: assert(!"WRONG NOTE NUMBER");
	}
}