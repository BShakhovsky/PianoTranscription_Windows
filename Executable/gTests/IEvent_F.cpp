# include "stdafx.h"
# include "IEvent_F.h"
# include "..\..\Model\MidiParserLib\Event.h"
# include "..\..\Model\MidiParserLib\IFileParser.h"

using testing::Test;
using namespace Model::MidiParser;
using gTest::IEvent_F;

IEvent_F::IEvent_F(const char* assertMsg, Event_ newEvent) :
	assertMsg_(assertMsg),
	event_(newEvent),
	file_()
{}

IEvent_F::~IEvent_F() {}

void IEvent_F::SetUp()
{
	PURE_VIRTUAL;
}

void IEvent_F::TearDown()
{
	PURE_VIRTUAL;
}


const type_info& IEvent_F::Midi() const
{
	return typeid(MidiEvent::GetInstance());
}
const type_info& IEvent_F::Meta() const
{
	return typeid(MetaEvent::GetInstance());
}
const type_info& IEvent_F::Syst() const
{
	return typeid(SystemEvent::GetInstance());
}