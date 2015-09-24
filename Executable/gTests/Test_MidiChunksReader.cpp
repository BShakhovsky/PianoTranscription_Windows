# include "stdafx.h"
# include "..\..\Model\MidiParserLib\MidiChunksReader.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "MidiParser_Mock.h"
# include "CurrentFileName.h"

using namespace std;
using boost::lexical_cast;
using namespace testing;
using namespace Model::MidiParser;
using namespace MidiStruct;
using gTest::MidiParser_Mock;

class Test_MidiChunksReader : public Test
{
public:
	MidiChunksReader midi_;

	Test_MidiChunksReader() :
		midi_(make_unique<MidiParser_Mock>())
	{}
	virtual ~Test_MidiChunksReader() override = default;

	virtual void SetUp() override final
	{
		FLAGS_gtest_break_on_failure = true;
		ASSERT_THROW(MidiChunksReader(CURRENT_FILE_NAME).ReadHeaderChunk(), runtime_error) << "CORRUPTED MIDI FILE HEADER";
		FLAGS_gtest_break_on_failure = false;
		union
		{
			const char ch[5] = "dulc";
			uint32_t num;
		} length;
		EXPECT_NONFATAL_FAILURE(MidiChunksReader(CURRENT_FILE_NAME).ReadTrackChunk(), "Corrupted MIDI Track Header, " + lexical_cast<string>(length.num) + " bytes skipped");
		FLAGS_gtest_break_on_failure = true;
	}
	virtual void TearDown() override final {}
};

# ifdef _DEBUG
#	define CHECK_DEATH(MESSG) {	ASSERT_DEBUG_DEATH(midi_.ReadHeaderChunk(),												\
		R"(DELTA TIMES ARE IN "SMPTE" COMPATIBLE UNITS.+ONLY "PPQN" CASE IS CURRENTLY CONSIDERED IN THIS SOFTWARE)"	)	\
								<< (MESSG); MidiParser_Mock::IncreaseCounter();											}
# elif defined NDEBUG
#	define CHECK_DEATH(MESSG)	ASSERT_DEBUG_DEATH(midi_.ReadHeaderChunk(),												\
								""																					)	\
								<< (MESSG)																				;
# else
#	"WRONG SOLUTION CONFIGURATION"
# endif

TEST_F(Test_MidiChunksReader, ReadHeaderChunk)
{
	ASSERT_THROW(midi_.ReadHeaderChunk(), runtime_error) << R"(	header != "MThd"	)";
	ASSERT_THROW(midi_.ReadHeaderChunk(), length_error) << "length = 0";
	ASSERT_THROW(midi_.ReadHeaderChunk(), length_error) << "length < sizeof HeaderData";
	ASSERT_THROW(midi_.ReadHeaderChunk(), length_error) << "length > sizeof HeaderData";

	ASSERT_THROW(midi_.ReadHeaderChunk(), logic_error) << "tracks = 0";
	ASSERT_THROW(midi_.ReadHeaderChunk(), logic_error) << "tracks > 1";

	ASSERT_THROW(midi_.ReadHeaderChunk(), logic_error) << "format = 0, division = 0";
	ASSERT_THROW(midi_.ReadHeaderChunk(), logic_error) << "format = 1, division = 0";
	try
	{
		midi_.ReadHeaderChunk();
	}
	catch (const runtime_error& e)
	{
		ASSERT_STREQ("CORRUPTED MIDI FILE FORMAT", e.what()) << "format > 2";
	}
	ASSERT_THROW(midi_.ReadHeaderChunk(), logic_error) << "format = 2, division = 0";

	HeaderChunk result = { NULL, NULL, NULL };
	ASSERT_NO_FATAL_FAILURE(result = midi_.ReadHeaderChunk());
	ASSERT_EQ(MidiStruct::ChunkIntro::HEADER, result.intro.type);
	ASSERT_EQ(6, result.intro.length);

	ASSERT_EQ(2, result.data.format);
	ASSERT_EQ(NULL, result.data.tracks);
	ASSERT_LT(NULL, result.data.division);
}

TEST_F(Test_MidiChunksReader, ReadTrackChunk)
{
	FLAGS_gtest_break_on_failure = false;
	EXPECT_NONFATAL_FAILURE(midi_.ReadTrackChunk(), "Corrupted MIDI Track Header, 0 bytes skipped");
	FLAGS_gtest_break_on_failure = true;

	TrackChunk result = {};
	ASSERT_NO_FATAL_FAILURE(result = midi_.ReadTrackChunk());
	ASSERT_EQ(MidiStruct::ChunkIntro::TRACK, result.intro.type);
	ASSERT_EQ(NULL, result.intro.length);
	ASSERT_EQ(NULL, result.trackEvent.size());

# ifdef _DEBUG
	FLAGS_gtest_break_on_failure = false;
	ASSERT_DEBUG_DEATH(midi_.ReadTrackChunk(), "");	// "SHOULD NEVER GET HERE"
# endif
}