# include "stdafx.h"
# include "CurrentFileName.h"
# include "..\..\Model\MidiParserLib\FileParser.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using std::length_error;
using namespace testing;
using namespace Model::MidiParser;
using MidiStruct::Bytes;

class Test_FileParser : public Test
{
public:
	FileParser file;
	const char* assertMsg = "LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE";
	const int32_t UNUSED = NULL;	// four padding bytes

	Test_FileParser() :
		file(CURRENT_FILE_NAME)
	{}
	virtual void SetUp() override final
	{
		FLAGS_gtest_break_on_failure = true;
		ASSERT_EQ(NULL, file.GetBytesRemained()) << "checking constructor";
	}
	virtual void TearDown() override final {}
};

TEST_F(Test_FileParser, ReadByte)
{
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.ReadByte(), assertMsg);
# endif
	file.SetBytesRemained(2);
	ASSERT_EQ(2, file.GetBytesRemained());
	ASSERT_EQ('#', file.ReadByte());

	ASSERT_EQ(1, file.GetBytesRemained());
	ASSERT_EQ(' ', file.ReadByte());
	ASSERT_EQ(NULL, file.GetBytesRemained());

	ASSERT_DEBUG_DEATH(file.ReadByte(), assertMsg);
}

TEST_F(Test_FileParser, SkipData)
{
	// Moving forward
	file.SetBytesRemained(3);
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.SkipData(4), assertMsg);
# endif
	file.SkipData(2);
	ASSERT_EQ(1, file.GetBytesRemained());
	ASSERT_EQ('i', file.PeekByte());
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.SkipData(2), assertMsg);
# endif

	// Moving backwards
	file.SkipData(-1);
	ASSERT_EQ(2, file.GetBytesRemained());
	ASSERT_EQ(' ', file.PeekByte());
	file.SkipData(-1);
	ASSERT_EQ(3, file.GetBytesRemained());
	ASSERT_EQ('#', file.PeekByte());

	// Moving backwards before the file beginning
	file.SetBytesRemained(20);
	file.SkipData(-1);
	ASSERT_EQ(EOF, file.PeekByte());
	ASSERT_EQ(NULL, file.ReadByte());
	ASSERT_EQ(NULL, file.ReadByte());
	file.SkipData(2);
	ASSERT_EQ(EOF, file.PeekByte());
	ASSERT_EQ(NULL, file.ReadByte());
	ASSERT_EQ(NULL, file.ReadByte());
	file.SkipData(-3);
	ASSERT_EQ(EOF, file.PeekByte());
	ASSERT_EQ(NULL, file.ReadByte());
	ASSERT_EQ(NULL, file.ReadByte());
	file.SkipData(10);
	ASSERT_EQ(EOF, file.PeekByte());
	ASSERT_EQ(NULL, file.ReadByte());
	ASSERT_EQ(NULL, file.ReadByte());
}

TEST_F(Test_FileParser, ReadInverse)
{
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.ReadInverse(sizeof int32_t), assertMsg);
# endif
	file.SetBytesRemained(1);
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.ReadInverse(sizeof int32_t), assertMsg);
# endif
	ASSERT_NO_FATAL_FAILURE(file.ReadInverse(sizeof int32_t, false));
	file.SetBytesRemained(sizeof int32_t);
	ASSERT_NO_FATAL_FAILURE(file.ReadInverse(sizeof int32_t));

	file.SkipData(NULL - static_cast<signed>(sizeof int32_t) * 2);

# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.ReadInverse(sizeof(int32_t) + 1, false), "NUMBER OF BYTES TO READ > SIZEOF INT");
	const auto result = file.ReadInverse(sizeof(int32_t), false);
# elif defined NDEBUG
	srand(static_cast<unsigned>(time(NULL)));
	const auto result = file.ReadInverse(sizeof(int32_t) + rand());
# else
		"WRONG SOLUTION CONFIGURATION";
# endif
	/* ***************************************************************************************
	 * "# include "
	 * https://upload.wikimedia.org/wikipedia/commons/e/e0/ASCII_Code_Chart-Quick_ref_card.png
	 * '#' =  35 = 0x'2'3' = ' *010 ' 0011 '
	 * ' ' =  32 = 0x'2'0' = ' *010 ' 0000 '
	 * 'i' = 105 = 0x'6'9' = ' *110 ' 1001 '
	 * 'n' = 110 = 0x'6'E' = ' *110 ' 1110 '
	 * 'c' =  99 = 0x'6'3' = ' *110 ' 0011 '
	 * 'l' = 108 = 0x'6'C' = ' *110 ' 1100 '
	 * 'u' = 117 = 0x'7'5' = ' *111 ' 0101 '
	 * 'd' = 100 = 0x'6'4' = ' *110 ' 0100 '
	 * 'e' = 101 = 0x'6'5' = ' *110 ' 0101 '
	 *
	 * "# in"	= 0010'0011 ' 0010'0000 ' 0110'1001 ' 0110'1110	= 0x'23'20'69'6E	= 589'326'702
	 * "clu"	= 0110'0011 ' 0110'1100 ' 0111'0101				= 0x'63'6C'75		= 6'515'829
	 * "de"		= 0110'0100 ' 0110'0101							= 0x'64'65			= 25'701
	 * **************************************************************************************/
	union
	{
		char ch[4];
		int num;
	} four, three, two;
	four.ch[3] = '#';
	four.ch[2] = ' ';
	four.ch[1] = 'i';
	four.ch[0] = 'n';
	three.ch[3] = '\0';
	three.ch[2] = 'c';
	three.ch[1] = 'l';
	three.ch[0] = 'u';
	two.ch[3] = two.ch[2] = '\0';
	two.ch[1] = 'd';
	two.ch[0] = 'e';
	
	ASSERT_EQ(four.num, result)						<< "result = " << 589'326'702;
	ASSERT_EQ(sizeof int32_t, file.GetBytesRemained());
	ASSERT_EQ('c', file.PeekByte());

	ASSERT_EQ(three.num, file.ReadInverse(3))		<< "result = " << 6'515'829;
	ASSERT_EQ(sizeof int32_t - 3, file.GetBytesRemained());
	ASSERT_EQ('d', file.PeekByte());

	ASSERT_EQ(two.num, file.ReadInverse(2, false))	<< "result = " << 25'701;
	ASSERT_EQ(sizeof int32_t - 3 - 2, file.GetBytesRemained());
	ASSERT_EQ(' ', file.PeekByte());

	ASSERT_EQ(' ', file.ReadInverse(1, false))		<< "result = " << 32;
	ASSERT_EQ(sizeof int32_t - 3 - 2 - 1, file.GetBytesRemained());
	ASSERT_EQ('\"', file.PeekByte());

	ASSERT_EQ(NULL, file.ReadInverse(NULL, false))	<< "result is zero";
	ASSERT_EQ(sizeof int32_t - 3 - 2 - 1 - NULL, file.GetBytesRemained());
	ASSERT_EQ('\"', file.PeekByte());
}

TEST_F(Test_FileParser, ReadVarLenFormat)
{
# ifdef _DEBUG
	ASSERT_DEBUG_DEATH(file.ReadVarLenFormat(), assertMsg);
# endif
	file.SetBytesRemained(Bytes::varLengthSize);
	ASSERT_EQ('#', file.ReadVarLenFormat());
	ASSERT_EQ(' ', file.ReadVarLenFormat());
	ASSERT_EQ('i', file.ReadVarLenFormat());
	ASSERT_EQ('n', file.ReadVarLenFormat());
	ASSERT_EQ('c', file.PeekByte());
	ASSERT_DEBUG_DEATH(file.ReadVarLenFormat(), assertMsg);

	FileParser project("gTests.vcxproj");
	project.SetBytesRemained(Bytes::varLengthSize);
	ASSERT_EQ(0xEF, project.PeekByte());	// https://hexed.it/
	ASSERT_EQ(0xEF'BB'BF'3C, project.ReadVarLenFormat());

	project.SkipData(-3);
	ASSERT_EQ(0xBB'BF'3C, project.ReadVarLenFormat());
	project.SkipData(-2);
	ASSERT_EQ(0xBF'3C, project.ReadVarLenFormat());
	project.SkipData(-1);
	ASSERT_EQ(0x3C, project.ReadVarLenFormat());

	FileParser header("Debug/gTests.pch");
	header.SetBytesRemained(200);
	ASSERT_EQ(0x56, header.PeekByte());		// https://hexed.it/
	header.SkipData(167);
	ASSERT_EQ(NULL, header.ReadVarLenFormat());
	ASSERT_THROW(header.ReadVarLenFormat(), length_error) << "UNEXPECTED VARIABLE LENGTH > FOUR BYTES";

	header.SkipData(-4);
	ASSERT_EQ(0xFF'FF'FF'01, header.ReadVarLenFormat());
	ASSERT_EQ(NULL, header.ReadVarLenFormat());

	header.SkipData(-200);	// moving backwards before the file beginning, so that each ReadByte() call returns NULL
	ASSERT_EQ(NULL, header.ReadVarLenFormat());
	ASSERT_EQ(NULL, header.ReadVarLenFormat());
}