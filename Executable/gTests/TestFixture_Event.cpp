# include "stdafx.h"
# include "TestFixture_Event.h"
# include "..\..\Model\MidiParserLib\Event.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"
# include "FileParser_Mock.h"

using std::make_shared;
using testing::FLAGS_gtest_break_on_failure;
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
	FLAGS_gtest_break_on_failure = true;

# ifdef _DEBUG
	file_->SetBytesRemained(bytesRemained_);
# endif
	ASSERT_NO_FATAL_FAILURE(Event::GetInstance(file_))	<< R"%(	1st line ("# include "stdafx.h"\n") = 0x00	)%";
	ASSERT_NO_FATAL_FAILURE(Event::GetInstance())		<< R"%(	2nd line ("/*******************\n") = 0x00	)%";
}

void TestFixture_Event::TearDown()
{
	ASSERT_DEBUG_DEATH(Event::GetInstance(), "LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE")
												<< file_->GetBytesRemained() << " bytes left";
}