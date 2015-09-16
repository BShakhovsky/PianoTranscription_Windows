# include "stdafx.h"
/*******************************************
-1	FF	MetaEvent
		47	2F	EndOfTrack
					zero byte = Ok
-1	FF	MetaEvent
		47	2F	EndOfTrack
				1	corrupted non-zero byte
-1	FF	MetaEvent
		47	2F	EndOfTrack
				-1	corrupted non-zero byte
-1	FF	MetaEvent
		47	2F	EndOfTrack
				15	corrupted non-zero byte
-1	FF	MetaEvent
		47	2F	EndOfTrack
				-15	corrupted non-zero byte
-1	FF	MetaEvent
		47	2F	EndOfTrack
				150	corrupted non-zero byte
-1	FF	MetaEvent
		47	2F	EndOfTrack
				-150 corrupted non-zero byte
-1	FF	MetaEvent
		47	2F	EndOfTrack
				3840 = zero byte = Ok
-1	FF	MetaEvent
		47	2F	EndOfTrack
				-256 = zero byte = Ok
*******************************************/

# include "..\..\Model\MidiParserLib\MetaEvent_EndTrack.h"
# include "EventCommon.h"

FIXTURE(MetaEvent_EndTrack, 4);

# define CHECK_LOGIC(MESSG){						ASSERT_DEBUG_DEATH		(CHECK_WHAT, assertMsg)	<< (MESSG);			}
# define CHECK_OK(MESSG){file_->SetBytesRemained(3);ASSERT_NO_FATAL_FAILURE	(CHECK_WHAT)			<< (MESSG);			}
# define CHECK_WRONG	{file_->SetBytesRemained(3);EXPECT_FATAL_FAILURE	(CHECK_WHAT,"Corrupted MIDI's end of track");}

TEST_F(Test_MetaEvent_EndTrack, Read_impl)
{
# ifdef _DEBUG
	auto assertMsg("SOMETHING IS WRONG IN PROGRAM ARITHMETICS");
	CHECK_LOGIC("bytes remained = 0");
	
	file_->SetBytesRemained(4);
	CHECK_LOGIC("bytes remained = 2");

	srand(static_cast<unsigned>(time(nullptr)));
	file_->SetBytesRemained(rand() % (RAND_MAX - 5) + 5);
	CHECK_LOGIC("bytes remained = rand > 2");
# endif
	
	CHECK_OK("bytes remained = 1");

	CHECK_WRONG;	//  1	= corrupted non-zero byte
	CHECK_WRONG;	// -1	= corrupted non-zero byte
	CHECK_WRONG;	//  15	= corrupted non-zero byte
	CHECK_WRONG;	// -15	= corrupted non-zero byte
	CHECK_WRONG;	//  150	= corrupted non-zero byte
	CHECK_WRONG;	// -150	= corrupted non-zero byte

	CHECK_OK("3'840 = zero byte = Ok");
	CHECK_OK("-256 = zero byte = Ok");
}