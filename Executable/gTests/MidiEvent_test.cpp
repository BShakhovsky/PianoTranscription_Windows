# include "stdafx.h"
/****************************************************************
-16		F0	System event
-11		F5	System event
-1		FF	Meta event

0		00	running status
15		0F	running status
30		1E	running status
45		2D	running status
60		3C	running status
75		4B	running status
90		5A	running status
105		69	running status
120		78	running status

-121	87	Note off (key is released)
				15		Note
				31		Velocity
-106	96	Note on (key is pressed)
				60		Note
				0		Velocity
55 > 0 ==> running status = note
				1		New velocity
-91		A5	Key after-touch
				-5		Note
				150		Velocity
-76		B4	Control change
				-150	Controller number
				-21		New value
-61		C3	New program (patch)
				46		Number
				-130	Not used
-46		D2	Channel after-touch
				150		Number
				0		Not used
-31		E1	Pitch wheel change (2'000H is normal or no change)
				0		Bottom(least significant) 7 bits of value
				5		Top (most significant) 7 bits of value
1 > 0 ==> running status = new bottom value
				4		New top value
****************************************************************/

# include "..\..\Model\MidiParserLib\MidiEvent.h"
# include "..\..\Model\MidiParserLib\FileParser.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "FileParser_Mock.h"
# include "CurrentFileName.h"
# include "IEvent_Fixture.h"

using namespace std;
using namespace Model::MidiParser;
using MidiStruct::EventChunk;
using gTest::IEvent_Fixture;

class MidiEvent_F : public IEvent_Fixture
{
public:
	MidiEvent_F() :
		IEvent_Fixture(CURRENT_FILE_NAME)
	{}
	virtual ~MidiEvent_F() override = default;

	virtual void SetUp() override final
	{
		IEvent_Fixture::SetUp();
	}
	virtual void TearDown() override final {}
};

# define CHECK_THROW(MESSG) {	ASSERT_THROW(Event::GetInstance().Read(), runtime_error) << (MESSG);	}
# define CHECK_RESULT(STATUS, NOTE, VELOC, MESSG) {	result = Event::GetInstance().Read();				\
													ASSERT_EQ((STATUS),	result->status)		<< (MESSG);	\
													ASSERT_EQ((NOTE),	result->note)		<< (MESSG);	\
													ASSERT_EQ((VELOC),	result->velocity)	<< (MESSG);	}
TEST_F(MidiEvent_F, Read_impl)
{
	Event::GetInstance();		// 3rd line (F0) = system event
	Event::GetInstance();		// 4th line (F5) = system event
	file_->SkipData(1);			// 5th line (FF) = meta event

	CHECK_THROW("Empty line");

	CHECK_THROW("00 running status");
	CHECK_THROW("0F running status");
	CHECK_THROW("1E running status");
	CHECK_THROW("2D running status");
	CHECK_THROW("3C running status");
	CHECK_THROW("4B running status");
	CHECK_THROW("5A running status");
	CHECK_THROW("69 running status");
	CHECK_THROW("78 running status");

	CHECK_THROW("Empty line");

	shared_ptr<EventChunk> result;
	CHECK_RESULT('\x87', 15, 31, "note off");
	CHECK_RESULT('\x96', 60, 0, "note on");

	CHECK_RESULT('\x96', 55, 1, "55 > 0 = running status = note");
	
# pragma warning(push)
# pragma warning(disable:4309)	// truncations of constant value:
	CHECK_RESULT('\xA5', 		-5,			static_cast<char>(150),		"A5 key after - touch");
	CHECK_RESULT('\xB4', static_cast<char>(-150),	-21,				"B4 control change");
	CHECK_RESULT('\xC3', 		46,			static_cast<char>(-130),	"C3 new program(patch) number");
	CHECK_RESULT('\xD2', static_cast<char>(150),	0,					"D2 channel after - touch number");
	CHECK_RESULT('\xE1', 		0,					5,					"E1 pitch wheel change");
# pragma warning(pop)

	CHECK_RESULT('\xE1', 1, 4, "1 > 0 = running status = new bottom value");

	CHECK_RESULT('\xE1', NULL, NULL, "/************ = zero running status = new bottom value");
	CHECK_RESULT('\xE1', NULL, NULL, "# include ... = zero running status = new bottom value");
	CHECK_RESULT('\xE1', NULL, NULL, "# include ... = zero running status = new bottom value");
}