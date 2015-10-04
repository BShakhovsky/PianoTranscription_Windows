# include "stdafx.h"
/**********************************************
-1	FF	MetaEvent
		zero = Sequence number
			zero = Wrong sequence chunk length
-1	FF	MetaEvent
		zero = Sequence number
			1 = Wrong sequence chunk length
				o
-1	FF	MetaEvent
		zero = Sequence number
			2 = Ok
				t
				w
-1	FF	MetaEvent
		zero = Sequence number
			3 = Wrong sequence chunk length
				t
				h
				r
-1	FF	MetaEvent
		1 = Text event
			5 lines:
				B
				o
				r
				i
				s
-1	FF	MetaEvent
		2 = Copyright notice
			-1	ReadVarLenFormat
			2	Two + 1 = 3 lines:
				B
				r
				s
-1	FF	MetaEvent
		3 = Sequence or track name
			-2	ReadVarLenFormat
			-1	ReadVarLenFormat
			3	Three + 1 + 2 = 6 lines:
				B
				o
				R
				i
				S
				s
-1	FF	MetaEvent
		4 = Instrument name
			zero lines
-1	FF	MetaEvent
		5 = Lyric text
			1 line:
				4
-1	FF	MetaEvent
		6  = Marker text
			2 lines:
				5
				a
-1	FF	MetaEvent
		7 = Cue point
			-15	ReadVarLenFormat
			-4	ReadVarLenFormat
			-6	ReadVarLenFormat
			7	Seven + 6 + 4 + 15 = 32 lines:
**********************************************/

# include "..\..\Model\MidiParserLib\MetaEvent_Text.h"
# include "MidiParser_EventCommon.h"

using namespace testing;

FIXTURE(MetaEvent_Text, 86);

TEST_F(Test_MetaEvent_Text, Read_impl)
{
	FLAGS_gtest_break_on_failure = false;
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong sequence chunk length");
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong sequence chunk length");

	FLAGS_gtest_break_on_failure = true;
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "length = 2 - Ok";

	FLAGS_gtest_break_on_failure = false;
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong sequence chunk length");

	FLAGS_gtest_break_on_failure = true;
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Text event";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Copyright notice";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Sequence or track name";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Instrument name";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Lyric text";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Marker text";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "Cue point";
}