# include "stdafx.h"
# include "TestFixture_Event.h"
# include "..\..\Model\MidiParserLib\Event.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "FileParser_Mock.h"

using std::make_shared;
using namespace Model::MidiParser;
using gTest::TestFixture_Event;

TestFixture_Event::TestFixture_Event(const char* fileName, const int bytesRemained) :
	file_(make_shared<FileParser_Mock>(fileName)),
	bytesRemained_(bytesRemained),
	result_(nullptr)
{}

TestFixture_Event::~TestFixture_Event() {}

void TestFixture_Event::SetUp()
{
# ifdef _DEBUG
	file_->SetBytesRemained(bytesRemained_);
# endif
	Event::GetInstance(file_);	// 1st line	("# include "stdafx.h"\n") = 0x00
	Event::GetInstance();		// 2nd line	("/*******************\n") = 0x00
}

void TestFixture_Event::TearDown()
{
	ASSERT_DEBUG_DEATH(Event::GetInstance(), "LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE")
												<< file_->GetBytesRemained() << " bytes left";
}