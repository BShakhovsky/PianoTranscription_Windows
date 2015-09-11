# include "stdafx.h"
# include "SystemEvent.h"
# include "MidiStruct.h"

using boost::serialization::singleton;
using Model::MidiParser::SystemEvent;

SystemEvent& SystemEvent::GetInstance()
{
	return singleton<SystemEvent>::get_mutable_instance();
}

void SystemEvent::Read_impl()
{
	switch (GetChunk()->status)
	{
	case -16:/* 0xF0 */	case -9: /* 0xF7 */
		SkipMsg("System exclusive event");	break;
	case -15:/* 0xF1 */ case -14:/* 0xF2 */ case -13:/* 0xF3 */ case -10:/* 0xF6 */
		SkipMsg("System common event");		break;
	case -8: /* 0xF8 */ case -6: /* 0xFA */ case -5: /* 0xFB */ case -4: /* 0xFC */ case -2:/* 0xFE */
		SkipMsg("System real-time event");	break;
	default: assert("!WRONG STATUS BYTE");
	}
}