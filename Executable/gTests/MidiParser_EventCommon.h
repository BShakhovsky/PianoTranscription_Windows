# pragma once

# include "MidiParser_TestFixture_Event.h"
# include "MidiParser_FileParser_Mock.h"
# include "CurrentFileName.h"

# define FIXTURE(CLASS_NAME, BYTES_REMAINED)														\
	class Test_ ## CLASS_NAME : public gTest::TestFixture_Event										\
	{																								\
	public:																							\
		Test_ ## CLASS_NAME () : gTest::TestFixture_Event(CURRENT_FILE_NAME, (BYTES_REMAINED)) {}	\
		Test_ ## CLASS_NAME (const Test_ ## CLASS_NAME &) = default;								\
		Test_ ## CLASS_NAME & operator = (const Test_ ## CLASS_NAME &) = default;					\
		virtual ~Test_ ## CLASS_NAME () override = default;											\
																									\
		virtual void SetUp() override final															\
		{																							\
			gTest::TestFixture_Event::SetUp();														\
		}																							\
		virtual void TearDown() override final														\
		{																							\
			gTest::TestFixture_Event::TearDown();													\
		}																							\
	};

# define CHECK_WHAT Model::MidiParser::Event::GetInstance(file_)->Read()

# define CHECK_TYPE(META_TYPE, EVENT_TYPE) ASSERT_EQ(typeid(Model::MidiParser::EVENT_TYPE),	\
	typeid(*Model::MidiParser::Event::GetInstance(file_))) << "meta type = " << (META_TYPE)	;