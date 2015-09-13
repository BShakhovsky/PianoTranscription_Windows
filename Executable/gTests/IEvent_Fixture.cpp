# include "stdafx.h"
# include "IEvent_Fixture.h"
# include "..\..\Model\MidiParserLib\Event.h"
# include "FileParser_Mock.h"

using std::make_shared;
using namespace Model::MidiParser;
using gTest::IEvent_Fixture;

IEvent_Fixture::IEvent_Fixture(const char* fileName) :
	file_(make_shared<FileParser_Mock>(fileName))
{}

IEvent_Fixture::~IEvent_Fixture() {}

void IEvent_Fixture::SetUp()
{
	Event::GetInstance(file_);	// 1st line	("# include "stdafx.h"\n") = 0x00
	Event::GetInstance();		// 2nd line	("/*******************\n") = 0x00
}