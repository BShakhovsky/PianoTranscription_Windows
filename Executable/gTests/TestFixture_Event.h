# pragma once
# include <gtest\gtest.h>

namespace Model
{
	namespace MidiParser
	{
		class IFileParser;

		namespace MidiStruct
		{
			class EventChunk;
		}
	}
}

namespace gTest
{
	class TestFixture_Event abstract : public testing::Test
	{
	public:
# ifdef _DEBUG
		const char* assertStatus_ = "WRONG STATUS BYTE";
# endif
		std::shared_ptr<Model::MidiParser::IFileParser> file_;
		int bytesRemained_;

		std::shared_ptr<Model::MidiParser::MidiStruct::EventChunk> result_;
	public:
		virtual ~TestFixture_Event() override;

		virtual void SetUp() override = 0;
		virtual void TearDown() override = 0;
	protected:
		explicit TestFixture_Event(const char* fileName, int bytesRemained);
	private:
		TestFixture_Event() = delete;
	};
}

# define CHECK_WHAT Event::GetInstance().Read()