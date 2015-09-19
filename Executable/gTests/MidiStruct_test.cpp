# include "stdafx.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using Model::MidiParser::MidiStruct::ChunkType;

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