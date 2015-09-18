# include "stdafx.h"
/******************************************
-1	FF
	-1	WRONG META TYPE
-1	FF
	0	Text event
-1	FF
	1	Text event
-1	FF
	2	Text event
-1	FF
	3	Text event
-1	FF
	4	Text event
-1	FF
	5	Text event
-1	FF
	6	Text event
-1	FF
	7	Text event
-1	FF
	8	WRONG META TYPE
-1	FF
	15	WRONG META TYPE
-1	FF
	25	WRONG META TYPE
-1	FF
	31	WRONG META TYPE
-1	FF
	32	0x20	Skip event
-1	FF
	33	0x21	Skip event
-1	FF
	34	WRONG META TYPE
-1	FF
	46	WRONG META TYPE
-1	FF
	47	0x2F	End of Track
-1	FF
	48	WRONG META TYPE
-1	FF
	80	WRONG META TYPE
-1	FF
	81	0x51	Tempo event
-1	FF
	82	WRONG META TYPE
-1	FF
	83	WRONG META TYPE
-1	FF
	84	0x54	Skip event
-1	FF
	85	WRONG META TYPE
-1	FF
	87	WRONG META TYPE
-1	FF
	88	0x58	Skip event
-1	FF
	89	0x59	Key Signature event
-1	FF
	90	WRONG META TYPE
-1	FF
	126	WRONG META TYPE
-1	FF
	127	0x7F	Skip event
-1	FF
	128	WRONG META TYPE
******************************************/

# include "..\..\Model\MidiParserLib\MetaEvent.h"
# include "EventCommon.h"

using std::runtime_error;
using boost::serialization::singleton;
using namespace Model::MidiParser;

FIXTURE(MetaEvent, 66);

# define CHECK_THROW(META_TYPE) ASSERT_THROW(Event::GetInstance(), runtime_error)	<< (META_TYPE) << " = WRONG META TYPE";

TEST_F(Test_MetaEvent, GetInstance)
{
	CHECK_THROW(-1);
	CHECK_TYPE(0, MetaEvent_Text);	CHECK_TYPE(1, MetaEvent_Text);		CHECK_TYPE(2, MetaEvent_Text);
	CHECK_TYPE(3, MetaEvent_Text);	CHECK_TYPE(4, MetaEvent_Text);		CHECK_TYPE(5, MetaEvent_Text);
	CHECK_TYPE(6, MetaEvent_Text);	CHECK_TYPE(7, MetaEvent_Text);

	CHECK_THROW(8);
	CHECK_THROW(15);
	CHECK_THROW(25);
	CHECK_THROW(31);				CHECK_TYPE(32, MetaEvent_Skip);		CHECK_TYPE(33, MetaEvent_Skip);
	CHECK_THROW(34);
	
	CHECK_THROW(46);				CHECK_TYPE(47, MetaEvent_EndTrack);
	CHECK_THROW(48);

	CHECK_THROW(80);				CHECK_TYPE(81, MetaEvent_Tempo);
	CHECK_THROW(82);
	CHECK_THROW(83);				CHECK_TYPE(84, MetaEvent_Skip);
	CHECK_THROW(85);

	CHECK_THROW(87);				CHECK_TYPE(88, MetaEvent_Skip);
									CHECK_TYPE(89, MetaEvent_KeySign);	CHECK_THROW(90);
	
	CHECK_THROW(126);				CHECK_TYPE(127, MetaEvent_Skip);
	CHECK_THROW(128);
}