# pragma once
# include <gtest\gtest.h>

namespace Model
{
	namespace MidiParser
	{
		class IFileParser;
	}
}

namespace gTest
{
	class IEvent_Fixture abstract : public testing::Test
	{
	public:
		const char* assertMsg_ = "WRONG STATUS BYTE";
		std::shared_ptr<Model::MidiParser::IFileParser> file_;
	public:
		virtual ~IEvent_Fixture() override;

		virtual void SetUp() override = 0;
		virtual void TearDown() override = 0
		{
			PURE_VIRTUAL;
		}
	protected:
		explicit IEvent_Fixture(const char* fileName);
	private:
		IEvent_Fixture() = delete;
	};
}