# include "stdafx.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using testing::Test;
using namespace Model::MidiParser::MidiStruct;

class MidiStructure_F : public Test
{
public:
	EventChunk eventChunk;
	virtual void SetUp() override final {}
	virtual void TearDown() override final
	{
		ASSERT_EQ('\0', eventChunk.status);
		ASSERT_EQ('\0', eventChunk.metaType);

		ASSERT_EQ(NULL, eventChunk.length);
		ASSERT_EQ(NULL, eventChunk.metaData);

		ASSERT_STREQ("", eventChunk.metaText.c_str());

		ASSERT_EQ(NULL, eventChunk.note);
		ASSERT_EQ(NULL, eventChunk.velocity);
	}
};

TEST_F(MidiStructure_F, EventChunk_Constr) {}

TEST_F(MidiStructure_F, EventChunk_Clear)
{
	srand(static_cast<unsigned>(time(NULL)));
	
	eventChunk.status = rand() % SCHAR_MAX;
	eventChunk.metaType = rand() % SCHAR_MAX;

	eventChunk.length = static_cast<uint32_t>(rand());
	eventChunk.metaData = static_cast<uint32_t>(rand());

	eventChunk.metaText = "String";

	eventChunk.note = rand() % INT16_MAX;
	eventChunk.velocity = rand() % INT16_MAX;

	eventChunk.Clear();
}

TEST(MidiStructure, BoolOperators)
{
	ChunkType
		header{ 'M', 'T', 'h', 'd' },
		track{ 'M', 'T', 'r', 'k' },
		anotherHeader{ 'M', 'T', 'h', 'd' };
	ASSERT_TRUE(header == anotherHeader);
	ASSERT_FALSE(header != anotherHeader);
	ASSERT_FALSE(header == track);
	ASSERT_TRUE(header != track);
}