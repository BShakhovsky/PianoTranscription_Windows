# pragma once
# include <gtest\gtest.h>

namespace Model
{
	namespace MidiParser
	{
		class Event;
		class IFileParser;
	}
}

namespace gTest
{
	class IEvent_F : public testing::Test
	{
		IEvent_F() = delete;
		typedef Model::MidiParser::Event& Event_;
		typedef Model::MidiParser::IFileParser FileParser_;
	public:
		const char* assertMsg_;
		Event_ event_;
		std::shared_ptr<FileParser_> file_;
	public:
		explicit IEvent_F(const char* assertMsg, Event_);
		virtual ~IEvent_F();

		virtual void SetUp() override = 0;
		virtual void TearDown() override = 0;

		const type_info& Midi() const;
		const type_info& Meta() const;
		const type_info& Syst() const;
	protected:
		template<class T>
		void SetFile(const char* fileName)
		{
			file_ = std::make_shared<T>(fileName);
		}
	};
}

# ifdef _DEBUG
#	define INIT_DEBUG			{ ASSERT_DEBUG_DEATH(Event::GetInstance(file_),							\
									"LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE");	}
# elif defined NDEBUG
#	define INIT_DEBUG
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif
# define INIT					{	SetFile<FileParser>(CURRENT_FILE_NAME);								\
									INIT_DEBUG;															\
									SetFile<FileParser_Mock>(CURRENT_FILE_NAME);						\
			Event::GetInstance(file_);	/* read first line of this file ("# include "stdafx.h"\n") */	\
			CHECK_TYPE(Midi, "second line (/**********) = 0x00");										}

# define CHECK_TYPE(TYPE, MESSG){ ASSERT_EQ(TYPE(), typeid(Event::GetInstance())) << (MESSG); }