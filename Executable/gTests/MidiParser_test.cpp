# include "stdafx.h"
# include "..\..\Model\MidiParserLib\MidiParser.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "CurrentFileName.h"

using testing::Test;
using Model::MidiParser::MidiParser;

TEST(MidiParser_F, ReadChunkIntro)
{
	MidiParser midiParser(CURRENT_FILE_NAME);
	ASSERT_NE(nullptr, midiParser.GetInputFile()) << "checking constructor";

	const auto result(midiParser.ReadChunkIntro());
	ASSERT_EQ('#', result.type.type[0]);
	ASSERT_EQ(' ', result.type.type[1]);
	ASSERT_EQ('i', result.type.type[2]);
	ASSERT_EQ('n', result.type.type[3]) << "type = first four bytes";

	union
	{
		char ch[4];
		int num;
	} length;
	length.ch[3] = 'c';
	length.ch[2] = 'l';
	length.ch[1] = 'u';
	length.ch[0] = 'd';
	ASSERT_EQ(length.num, result.length) << "length = next four bytes";
}