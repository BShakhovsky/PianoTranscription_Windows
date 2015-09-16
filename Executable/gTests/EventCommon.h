# pragma once

# include "TestFixture_Event.h"
# include "FileParser_Mock.h"
# include "CurrentFileName.h"

# define FIXTURE(CLASS_NAME, BYTES_REMAINED)								\
	class Test_ ## CLASS_NAME : public gTest::TestFixture_Event				\
	{																		\
	public:																	\
		Test_ ## CLASS_NAME () :											\
			gTest::TestFixture_Event(CURRENT_FILE_NAME, (BYTES_REMAINED))	\
		{}																	\
		virtual ~Test_ ## CLASS_NAME () override = default;					\
																			\
		virtual void SetUp() override final									\
		{																	\
			gTest::TestFixture_Event::SetUp();								\
		}																	\
		virtual void TearDown() override final								\
		{																	\
			gTest::TestFixture_Event::TearDown();							\
		}																	\
	};

# define CHECK_WHAT Model::MidiParser::Event::GetInstance().Read()