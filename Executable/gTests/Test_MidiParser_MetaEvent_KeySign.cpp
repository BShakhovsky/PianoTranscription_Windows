# include "stdafx.h"
/*******************************************
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		zero = wrong
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		1 = wrong
			-- skip --
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		3 = wrong
			-- skip --
			-- skip --
			-- skip --
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		-1	ReadVarLenFormat
		zero + 1 = 1 = wrong
			-- skip --
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		-2	ReadVarLenFormat
		3	Three + 2 = 5 = wrong:
			-- skip --
			-- skip --
			-- skip --
			-- skip --
			-- skip --
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		2 = Ok
			-8 = wrong number of bemoles
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		-1	ReadVarLenFormat
		1	One + 1 = 2 = Ok
			zero = natural key signature, Ok
			2 = wrong major/minor
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		-2	ReadVarLenFormat
		zero + 2 = 2 = Ok
			8 = wrong number of dieses
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		2 = Ok
			-7 = seven bemoles
			zero = major key
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		2 = Ok
			zero = natural key signature, Ok
			1 = minor key
-1	FF	MetaEvent
	89 = 0x59 = KeySignature
		2 = Ok
			7 = seven dieses
*******************************************/

# include "..\..\Model\MidiParserLib\MetaEvent_KeySign.h"
# include "MidiParser_EventCommon.h"

using std::runtime_error;
using testing::FLAGS_gtest_break_on_failure;

FIXTURE(MetaEvent_KeySign, 59);

TEST_F(Test_MetaEvent_KeySign, Read_impl)
{
	FLAGS_gtest_break_on_failure = false;
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong key signature chunk length, 0 bytes skipped");
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong key signature chunk length, 1 bytes skipped");
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong key signature chunk length, 3 bytes skipped");
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong key signature chunk length, 1 bytes skipped");
	EXPECT_NONFATAL_FAILURE(CHECK_WHAT, "Wrong key signature chunk length, 5 bytes skipped");

	FLAGS_gtest_break_on_failure = true;
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "WRONG KEY SIGNATURE, SHOULD BE 0...7 BEMOLES OR DIESES";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "WRONG KEY SIGNATURE, SHOULD BE EITHER MAJOR OR MINOR";
	ASSERT_THROW(CHECK_WHAT, runtime_error) << "WRONG KEY SIGNATURE, SHOULD BE 0...7 BEMOLES OR DIESES";

	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "7 bemoles, major key";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "natural minor key = Lya-Minor";
	ASSERT_NO_FATAL_FAILURE(CHECK_WHAT) << "7 dieses, major key";
}