# include "stdafx.h"
# include "..\..\Model\MidiParserLib\MidiParser.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "..\..\Model\MidiParserLib\FileParser.h"
# include "CurrentFileName.h"

using std::runtime_error;
using namespace testing;
using namespace Model::MidiParser;

class Test_MidiParser : public Test
{
public:
	MidiParser midi_;
	Test_MidiParser() :
		midi_(CURRENT_FILE_NAME)
	{}
	virtual ~Test_MidiParser() override = default;

	virtual void SetUp() override final
	{
		FLAGS_gtest_break_on_failure = true;
	}
	virtual void TearDown() override final {}
};

TEST_F(Test_MidiParser, ReadChunkIntro)
{
	const auto result(midi_.ReadChunkIntro());
	ASSERT_EQ('#', result.type.type[0]);
	ASSERT_EQ(' ', result.type.type[1]);
	ASSERT_EQ('i', result.type.type[2]);
	ASSERT_EQ('n', result.type.type[3]) << "type = first four bytes";

	union
	{
		const char ch[5] = "dulc";
		int num;
	} length;
	ASSERT_EQ(length.num, result.length) << "length = next four bytes";
}

TEST_F(Test_MidiParser, ReadHeaderData)
{
	const auto result(midi_.ReadHeaderData());

	union
	{
		char ch[2];
		int16_t num;
	} format, tracks, division;
	format.ch[1] = '#';
	format.ch[0] = ' ';
	tracks.ch[1] = 'i';
	tracks.ch[0] = 'n';
	division.ch[1] = 'c';
	division.ch[0] = 'l';
	ASSERT_EQ(format.num, result.format) << "format = first two bytes";
	ASSERT_EQ(tracks.num, result.tracks) << "tracks = next two bytes";
	ASSERT_EQ(division.num, result.division) << "division = last two bytes";
}

# define CHECK_INIT(NUM_BYTES, RES_SIZE) const auto result(midi_.ReadTrackEvents(NUM_BYTES));	\
						ASSERT_EQ((RES_SIZE),	result.size());									
# define CHECK_RESULT(INDEX, DELTA_TIME, STATUS, NOTE, VELOCITY, MESSG) {						\
						ASSERT_EQ((DELTA_TIME),	result.at(INDEX).deltaTime);					\
						ASSERT_EQ((STATUS),		result.at(INDEX).eventChunk.status) << (MESSG);	\
						ASSERT_EQ((NOTE),		result.at(INDEX).eventChunk.note);				\
						ASSERT_EQ((VELOCITY),	result.at(INDEX).eventChunk.velocity);			}

# define CATCH			catch (const runtime_error& e) { ASSERT_STREQ(throwMsg, e.what());		}
# define CATCH_AND_DIE	catch (const runtime_error& e) { ASSERT_STREQ(throwMsg, e.what());		\
	assert(!"LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE");					}

TEST_F(Test_MidiParser, ReadTrackEvents)
{
	ASSERT_NO_FATAL_FAILURE(midi_.ReadTrackEvents(NULL)) << "never going inside the while loop";
	const auto throwMsg("MIDI RUNNING STATUS IS NOT CORRECT");
# ifdef _DEBUG
	const auto assertMsg("LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE");
	ASSERT_DEBUG_DEATH(midi_.ReadTrackEvents(static_cast<uint32_t>(-1)),
													"BYTES TO READ FROM MIDI FILE SHOULD BE A POSITIVE NUMBER");
	ASSERT_DEBUG_DEATH(midi_.ReadTrackEvents(1), assertMsg);
	ASSERT_DEBUG_DEATH(try
	{
		midi_.ReadTrackEvents(2);
	}
	CATCH_AND_DIE, assertMsg);
# endif
	try
	{
		CHECK_INIT(3, 1);
		CHECK_RESULT(0, '#', '\xE1', ' ', 'i', "running status is still being kept after MidiEvent test");
	}
	CATCH
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(try
	{
		midi_.ReadTrackEvents(4);
		midi_.ReadTrackEvents(5);
	}
	CATCH_AND_DIE, assertMsg);
# endif
	try
	{
		CHECK_INIT(6, 2);
		CHECK_RESULT(0, 'n', '\xE1', 'c', 'l', "running status is still being kept after MidiEvent test");
		CHECK_RESULT(1, 'u', '\xE1', 'd', 'e', "running status is still being kept after MidiEvent test");
	}
	CATCH
}