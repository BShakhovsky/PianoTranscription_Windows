# include "stdafx.h"
/*********************************
-1	FF	META-EVENT
-2	FE	Real-time event
			0	zero lines to skip
-3	FD	WRONG STATUS BYTE
-4	FC	Real-time event
			1	one line to skip:
					-- skip --
-5	FB	Real-time event
			6	six lines to skip:
*********************************/
# include "..\..\Model\MidiParserLib\SystemEvent.h"
# include "..\..\Model\MidiParserLib\FileParser.h"
# include "FileParser_Mock.h"
# include "CurrentFileName.h"
/********************************************
-6	FA	Real-time event
			-1	ReadVarLenFormat
			4	one + four = 5 lines to skip:
********************************************/
using namespace std;
using testing::Test;
using namespace Model::MidiParser;
/*******************************************************
-7	F9	WRONG STATUS BYTE
-8	F8	Real-time event
			0	no lines to skip:
-9	F7	Exclusive event
			-15	ReadVarLenFormat
			-23	ReadVarLenFormat
			-2	ReadVarLenFormat
			0	Fifteen + 23 + 2 + 0 = 40 lines to skip:
*******************************************************/
class SystemEvent_F : public Test
{
public:
	const char* assertMsg = "WRONG STATUS BYTE";
	SystemEvent& sysEvent;
	shared_ptr<IFileParser> file;

	SystemEvent_F() :
		sysEvent(SystemEvent::GetInstance()),
		file(make_shared<FileParser>(CURRENT_FILE_NAME))
	{}
	virtual void SetUp() override final
	{
# ifdef _DEBUG
		ASSERT_DEBUG_DEATH(sysEvent.Read(), assertMsg) << "status = zero = not initialized yet";
# endif
	}
	virtual void TearDown() override final {}

	template<class T>
	void SetFile()
	{
		file = make_shared<T>(CURRENT_FILE_NAME);
	}

	const type_info& Midi() const
	{
		return typeid(MidiEvent::GetInstance());
	}
	const type_info& Meta() const
	{
		return typeid(MetaEvent::GetInstance());
	}
	const type_info& Syst() const
	{
		return typeid(SystemEvent::GetInstance());
	}
};
/************************************************
-10	F6	Common event
			-1	ReadVarLenFormat
			-10	ReadVarLenFormat
			4	One + ten + 4 = 15 lines to skip:
************************************************/
TEST_F(SystemEvent_F, Read_impl_FileParser)
{
	SetFile<FileParser>();
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(Event::GetInstance(file), "LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE");
# endif
	file->SetBytesRemained(1);

	Event::GetInstance(file);
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(sysEvent.Read(), assertMsg) << "status = '#' = 35 = 0x23";
# endif
}
/***********************************************
-11	F5	WRONG STATUS BYTE
-12	F4	WRONG STATUS BYTE
-13	F3	Common event
			-5	ReadVarLenFormat
			7	Five + seven = 12 lines to skip:
***********************************************/
# define CHECK_TYPE(TYPE, MESSG) ASSERT_EQ(TYPE(), typeid(Event::GetInstance())) << (MESSG);
# ifdef _DEBUG
#	define CHECK_DEATH(MESSG) ASSERT_DEBUG_DEATH(sysEvent.Read(), assertStatus) << (MESSG);
# elif defined NDEBUG
#	define CHECK_DEATH(MESSG)
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif
# define CHECK_OK(MESSG) { CHECK_TYPE(Syst, (MESSG)); ASSERT_NO_FATAL_FAILURE(sysEvent.Read()) << (MESSG); }
# define CHECK_WRONG(MESSG) { CHECK_TYPE(Syst, (MESSG)); CHECK_DEATH(MESSG); }
/*****************************
-14	F2	Common event
			29	lines to skip:
*****************************/
TEST_F(SystemEvent_F, Read_impl_Mock)
{
	SetFile<FileParser_Mock>();
# ifdef _DEBUG
	auto assertStatus("WRONG STATUS BYTE");
# endif
	Event::GetInstance(file);	// read first line of this file ("# include "stdafx.h"\n")
	CHECK_TYPE(Midi, "second line (/**********) = 0x00");
	CHECK_TYPE(Meta, "third line (FF)");
	CHECK_DEATH("FF = meta event");
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
	ASSERT_THROW(Event::GetInstance().Read(), length_error);	// if VarLenFormat exceeds four bytes
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