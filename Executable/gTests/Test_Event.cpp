# include "stdafx.h"
/*************************
-1	FF	Meta event
	zero = meta-text event
-2	FE	System event
-3	FD	System event
-4	FC	System event
-5	FB	System event
-6	FA	System event
-7	F9	System event
-8	F8	System event
-9	F7	System event
-10	F6	System event
-11	F5	System event
-12	F4	System event
-13	F3	System event
-14	F2	System event
-15	F1	System event
-16	F0	System event
-17	EF	Midi event
0		Midi event
1		Midi event
239	EF	Midi event
*************************/

# include "..\..\Model\MidiParserLib\Event.h"
# include "EventCommon.h"

FIXTURE(Event, 23);

TEST_F(Test_Event, GetInstance)
{
	CHECK_TYPE(-1, MetaEvent_Text);
	
	CHECK_TYPE(-2, SystemEvent);	CHECK_TYPE(-3, SystemEvent);	CHECK_TYPE(-4, SystemEvent);
	CHECK_TYPE(-5, SystemEvent);	CHECK_TYPE(-6, SystemEvent);	CHECK_TYPE(-7, SystemEvent);
	CHECK_TYPE(-8, SystemEvent);	CHECK_TYPE(-9, SystemEvent);	CHECK_TYPE(-10, SystemEvent);
	CHECK_TYPE(-11, SystemEvent);	CHECK_TYPE(-12, SystemEvent);	CHECK_TYPE(-13, SystemEvent);
	CHECK_TYPE(-14, SystemEvent);	CHECK_TYPE(-15, SystemEvent);	CHECK_TYPE(-16, SystemEvent);

	CHECK_TYPE(-17, MidiEvent);		CHECK_TYPE(0, MidiEvent);		CHECK_TYPE(1, MidiEvent);	CHECK_TYPE(239, MidiEvent);
}