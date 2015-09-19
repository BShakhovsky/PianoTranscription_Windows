# include "stdafx.h"
/****************************************************************
-16		F0	System event
-11		F5	System event
-1		FF	Meta event

0		00	running status
15		0F	running status
30		1E	running status
45		2D	running status
60		3C	running status
75		4B	running status
90		5A	running status
105		69	running status
120		78	running status

-121	87	Note off (key is released)
				15		Note
				31		Velocity
-106	96	Note on (key is pressed)
				60		Note
				0		Velocity
55 > 0 ==> running status = note
				1		New velocity
-91		A5	Key after-touch
				-5		Note
				150		Velocity
-76		B4	Control change
				-150	Controller number
				-21		New value
-61		C3	New program (patch)
				46		Number
				-130	Not used
-46		D2	Channel after-touch
				150		Number
				0		Not used
-31		E1	Pitch wheel change (2'000H is normal or no change)
				0		Bottom(least significant) 7 bits of value
				5		Top (most significant) 7 bits of value
1 > 0 ==> running status = new bottom value
				4		New top value
****************************************************************/

# include "..\..\Model\MidiParserLib\MidiEvent.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "EventCommon.h"

using std::runtime_error;
using Model::MidiParser::Event;

FIXTURE(MidiEvent, 47);

# define CHECK_RESULT(STATUS, NOTE, VELOC, MESSG) {	result_ = CHECK_WHAT;								\
													ASSERT_EQ((STATUS),	result_->status)	<< (MESSG);	\
													ASSERT_EQ((NOTE),	result_->note)		<< (MESSG);	\
													ASSERT_EQ((VELOC),	result_->velocity)	<< (MESSG);	}
TEST_F(Test_MidiEvent, Read_impl)
{
	ASSERT_NO_FATAL_FAILURE(Event::GetInstance(file_))	<< "3rd line (F0) = system event";
	ASSERT_NO_FATAL_FAILURE(Event::GetInstance(file_))	<< "4th line (F5) = system event";
	ASSERT_NO_FATAL_FAILURE(file_->SkipData(1))			<< "5th line (FF) = meta event";

	ASSERT_THROW(CHECK_WHAT, runtime_error) << "Empty line";		// Reads one status-byte

	ASSERT_THROW(CHECK_WHAT, runtime_error) << "00 running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "0F running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "1E running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "2D running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "3C running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "4B running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "5A running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "69 running status";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "78 running status";

	ASSERT_THROW(CHECK_WHAT, runtime_error) << "Empty line";

	CHECK_RESULT('\x87', 15, 31, "note off");						// Reads three bytes: status, note and velocity
	CHECK_RESULT('\x96', 60, 0, "note on");

	CHECK_RESULT('\x96', 55, 1, "55 > 0 = running status = note");	// running status reads two bytes
	
# pragma warning(push)
# pragma warning(disable:4309)	// truncations of constant value:
	CHECK_RESULT('\xA5', 		-5,			static_cast<char>(150),		"A5 key after - touch");
	CHECK_RESULT('\xB4', static_cast<char>(-150),	-21,				"B4 control change");
	CHECK_RESULT('\xC3', 		46,			static_cast<char>(-130),	"C3 new program(patch) number");
	CHECK_RESULT('\xD2', static_cast<char>(150),	0,					"D2 channel after - touch number");
	CHECK_RESULT('\xE1', 		0,					5,					"E1 pitch wheel change");
# pragma warning(pop)

	CHECK_RESULT('\xE1', 1, 4, "1 > 0 = running status = new bottom value");

	CHECK_RESULT('\xE1', NULL, NULL, "/************ = zero running status = new bottom value");
	CHECK_RESULT('\xE1', NULL, NULL, "# include ... = zero running status = new bottom value");
	CHECK_RESULT('\xE1', NULL, NULL, "# include ... = zero running status = new bottom value");
}