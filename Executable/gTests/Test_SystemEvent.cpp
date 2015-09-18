# include "stdafx.h"
/***********************************
-1	FF	META-EVENT
-2	FE	Real-time event
			0	zero lines to skip
-3	FD	WRONG STATUS BYTE
-4	FC	Real-time event
			1	one line to skip:
					-- skip --
-5	FB	Real-time event
			7	seven lines to skip:
***********************************/

# include "..\..\Model\MidiParserLib\SystemEvent.h"
# include "..\..\Model\MidiParserLib\FileParser.h"
# include "EventCommon.h"

/********************************************
-6	FA	Real-time event
			-1	ReadVarLenFormat
			5	one + five = 6 lines to skip:
********************************************/

using std::length_error;
using Model::MidiParser::Event;

/*************************************************
-7	F9	WRONG STATUS BYTE
-8	F8	Real-time event
			0	no lines to skip:
-9	F7	Exclusive event
			-2	ReadVarLenFormat
			-1	ReadVarLenFormat
			-2	ReadVarLenFormat
			0	Two + 1 + 2 + 0 = 5 lines to skip:
*************************************************/

FIXTURE(SystemEvent, 100);

/***********************************************
-10	F6	Common event
			0	no lines to skip
-11	F5	WRONG STATUS BYTE
-12	F4	WRONG STATUS BYTE
-13	F3	Common event
			-9	ReadVarLenFormat
			-1	ReadVarLenFormat
			3	Nine + 1 + 3 = 13 lines to skip:
***********************************************/

# define CHECK_OK(MESSG)		ASSERT_NO_FATAL_FAILURE(Event::GetInstance(file_).Read())	<< (MESSG);
# ifdef _DEBUG
#	define CHECK_WRONG(MESSG) {	ASSERT_DEBUG_DEATH(CHECK_WHAT, assertStatus_)				<< (MESSG);	\
								Event::GetInstance();													}
# elif defined NDEBUG
#	define CHECK_WRONG(MESSG)	Event::GetInstance();
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif

/******************************************************
-14	F2	Common event
			-16 ReadVarLenFormat
			-4	ReadVarLenFormat
			-2	ReadVarLenFormat
			2	Sixteen + 4 + 2 + 2 = 22 lines to skip:
******************************************************/

TEST_F(Test_SystemEvent, Read_impl)
{
	file_->SkipData(1);	// 3rd line (FF) = meta event
	CHECK_OK("FE = real-time event");
	CHECK_WRONG("FD");
	CHECK_OK("FC = real-time event");
	CHECK_OK("FB = real-time event");
	CHECK_OK("FA = real-time event");
	CHECK_WRONG("F9");
	CHECK_OK("F8 = real-time event");
	CHECK_OK("F7 = exclusive event");
	CHECK_OK("F6 = common event");
	CHECK_WRONG("F5");
	CHECK_WRONG("F4");
	CHECK_OK("F3 = common event");
	CHECK_OK("F2 = common event");
	CHECK_OK("F1 = common event");
	CHECK_OK("F0 = exclusive event");
	ASSERT_THROW(Event::GetInstance().Read(), length_error) << "VarLenFormat exceeds four bytes";
}

/**************************************************************
-15	F1	Common event
			0 no lines to skip
-16	F0	Exclusive event
			1 line to skip

-16		Check if VarLenFormat exceeds four bytes:
			-1
			-1
			-1
			-1
			0	stop at 5th byte

==============================================================
			SUMMARY OF MAIN CHECKS BEING DONE:
--------------------------------------------------------------
-16(F0) -9 (F7)							Exclusive event,	Ok
-15(F1)	-14(F2) -13(F3) -10(F6)			Common event,		Ok
-8 (F8) -6 (FA) -5 (FB)	-4 (FC)	-2 (FE)	Real-time event,	Ok
									WRONG STATUS BYTE	Not Ok
**************************************************************/