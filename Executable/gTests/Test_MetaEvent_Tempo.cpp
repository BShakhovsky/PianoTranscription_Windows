# include "stdafx.h"
/*********************************************************
-1	FF	MetaEvent
		81	0x51	TempoEvent
					zero byte = Not Ok
-1	FF	MetaEvent
		81	0x51	TempoEvent
					1 = Not Ok
						5 = metaData
-1	FF	MetaEvent
		81	0x51	TempoEvent
					2 = Not Ok
						1	ReadInverse
						3	Three + 1 = 4 = metaData
-1	FF	MetaEvent
		81	0x51	TempoEvent
					3 = Ok
						4	ReadInverse
						5	ReadInverse
						6	Six + 5 + 4 = 15 = metaData
-1	FF	MetaEvent
		81	0x51	TempoEvent
					4 = Not Ok
						0	ReadInverse
						0	ReadInverse
						6	ReadInverse
						0	0 + 6 + 0 + 0 = 6 = metaData
-1	FF	MetaEvent
		81	0x51	TempoEvent
					-4 = Not Ok
					Zero + 4 = 4 lines to read:
						1	ReadInverse
						2	ReadInverse
						3	ReadInverse
						4	1 + 2 + 3 + 4 = 10 = metaData
-1	FF	MetaEvent
		81	0x51	TempoEvent
					-1 = Not Ok
					-2	ReadVarLenFormat
					2	Two + 2 + 1 = 5 lines to read:
						2	ReadInverse
						3	ReadInverse
						4	ReadInverse
						5	2 + 3 + 4 + 5 = 14 = metaData
*********************************************************/
# include "..\..\Model\MidiParserLib\MetaEvent_Tempo.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "FileParser_Mock.h"
# include "CurrentFileName.h"
# include "TestFixture_Event.h"

using std::shared_ptr;
using namespace Model::MidiParser;
using MidiStruct::EventChunk;
using gTest::TestFixture_Event;

class Test_MetaEvent_Tempo : public TestFixture_Event
{
public:
	Test_MetaEvent_Tempo() :
		TestFixture_Event(CURRENT_FILE_NAME, 44)
	{}
	virtual ~Test_MetaEvent_Tempo() override = default;

	virtual void SetUp() override final
	{
		TestFixture_Event::SetUp();
	}
	virtual void TearDown() override final
	{
		TestFixture_Event::TearDown();
	}
};

# ifdef _DEBUG
#	define CHECK_DEATH(MESSG, SKIP_BYTES){ ASSERT_DEBUG_DEATH(CHECK_WHAT, (MESSG));	file_->SkipData(SKIP_BYTES);	}
# elif defined NDEBUG
#	define CHECK_DEATH(MESSG, SKIP_BYTES)											file_->SkipData(SKIP_BYTES)
# else
#	"WRONG SOLUTION CONFIGURATION"
# endif
# define CHECK_RESULT(LEN, DATA){ ASSERT_EQ((LEN), result_->length);	ASSERT_EQ((DATA), result_->metaData);		}
# define CHECK_WRONG(LEN, DATA)	{ EXPECT_NONFATAL_FAILURE(result_ = CHECK_WHAT, "Wrong tempo chunk length");		\
																			CHECK_RESULT((LEN), (DATA));			}
TEST_F(Test_MetaEvent_Tempo, Read_impl)
{
	CHECK_DEATH("WRONG TEMPO: DIVISION BY ZERO", 3);
	
	CHECK_WRONG(1, 5);
	CHECK_WRONG(2, 4);
	
	ASSERT_NO_FATAL_FAILURE(result_ = CHECK_WHAT) << "Tempo chunk length = 3 = Ok";
	CHECK_RESULT(3, 15);
	
	CHECK_WRONG(4, 6);
	CHECK_WRONG(4, 10);
# ifdef _DEBUG
	CHECK_DEATH("NUMBER OF BYTES TO READ > SIZEOF INT", 9);
# elif defined NDEBUG
	CHECK_WRONG(5, 14);
# else
	"WRONG SOLUTION CONFIGURATION";
# endif
}