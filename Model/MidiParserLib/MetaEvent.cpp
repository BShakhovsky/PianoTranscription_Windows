# include "stdafx.h"
# include "MetaEvent.h"
# include "MidiStruct.h"
# include "FileParser.h"

using std::runtime_error;
using Model::MidiParser::MetaEvent;

MetaEvent& MetaEvent::MetaInit(const char metaTypeByte)
{
	GetChunk()->metaType = metaTypeByte;
	return *this;
}

MetaEvent& MetaEvent::GetInstance()
{
	if (!GetInputFile()) throw runtime_error("INPUT FILE HAS NOT BEEN SET YET");
	const auto metaType(GetInputFile()->ReadByte());

	switch (metaType)
	{
	case 0x00: case 0x01: case 0x02: case 0x03: case 0x04: case 0x05: case 0x06: case 0x07:
				return MetaEvent_Text		::GetInstance().MetaInit(metaType);	break;
	case 0x20: case 0x21: case 0x54: case 0x58: case 0x7F:
				return MetaEvent_Skip		::GetInstance().MetaInit(metaType);	break;
	case 0x2F:	return MetaEvent_EndTrack	::GetInstance().MetaInit(metaType);	break;
	case 0x51:	return MetaEvent_Tempo		::GetInstance().MetaInit(metaType);	break;
	case 0x59:	return MetaEvent_KeySign	::GetInstance().MetaInit(metaType);	break;
	default: throw runtime_error("WRONG META TYPE");
	}
}