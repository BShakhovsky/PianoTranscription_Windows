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
	case -16:/*F0*/ case -9: /*F7*/													// Exclusive event,	Ok
	case -15:/*F1*/ case -14:/*F2*/ case -13:/*F3*/ case -10:/*F6*/					// Common event,	Ok
	case -8: /*F8*/ case -6: /*FA*/ case -5: /*FB*/ case -4: /*FC*/ case -2:/*FE*/	// Real-time event,	Ok
		break; default: assert(!"WRONG STATUS BYTE");								// Not Ok
	}
	Skip();
}