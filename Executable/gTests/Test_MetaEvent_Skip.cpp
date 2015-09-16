# include "stdafx.h"
/*************************************************
-1	FF	MetaEvent
		32	0x20	MIDI channel prefix assignment
					zero = wrong byte
-1	FF	MetaEvent
		32	0x20	MIDI channel prefix assignment
					1 = Ok
						-- skip --
-1	FF	MetaEvent
		33	0x21	MIDI port
					2 = wrong byte
						-- skip --
						-- skip --
-1	FF	MetaEvent
		33	0x21	MIDI port
					1 = Ok
						-- skip --
-1	FF	MetaEvent
		84	0x54	SMPTE offset
					-1 = wrong byte
					-2 ReadVarLenFormat
					zero + 2 + 1 = 3 line to skip:
						-- skip --
						-- skip --
						-- skip --
-1	FF	MetaEvent
		84	0x54	SMPTE offset
					5 = Ok
						-- skip --
						-- skip --
						-- skip --
						-- skip --
						-- skip --
-1	FF	MetaEvent
		88	0x58	time signature
					5 = wrong byte
						-- skip --
						-- skip --
						-- skip --
						-- skip --
						-- skip --
-1	FF	MetaEvent
		88	0x58	time signature
					4 = Ok
						-- skip --
						-- skip --
						-- skip --
						-- skip --
-1	FF	MetaEvent
		127	0x7F	Sequencer specific event
					zero = Ok
-1	FF	MetaEvent
		127	0x7F	Sequencer specific event
					10 = Ok as well
*************************************************/

# include "..\..\Model\MidiParserLib\MetaEvent_Skip.h"
# include "EventCommon.h"

FIXTURE(MetaEvent_Skip, 65);

# define CHECK_CASE(MESSG, BYTE_VAL)	{ 	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong " MESSG " chunk length");		\
											ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << (BYTE_VAL) << " = correct length";	}
TEST_F(Test_MetaEvent_Skip, Read_impl)
{
	CHECK_CASE("MIDI channel prefix assignment",	1);
	CHECK_CASE("MIDI port",							1);
	CHECK_CASE("SMPTE offset",						5);
	CHECK_CASE("time signature",					4);

	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Sequencer specific event, any length is Ok";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Sequencer specific event, any length is Ok";
}