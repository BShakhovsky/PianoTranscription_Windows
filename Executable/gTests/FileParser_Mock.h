# pragma once
# include "..\..\Model\MidiParserLib\IFileParser.h"
namespace Model
{
	namespace MidiParser
	{
		class FileCounter;
	}
}
namespace gTest
{
	class FileParser_Mock : public Model::MidiParser::IFileParser
	{
		const int32_t UNUSED = NULL;	// four padding bytes
		std::ifstream inputFile_;
		std::shared_ptr<Model::MidiParser::FileCounter> bytesRemained_;
	public:
		explicit FileParser_Mock(const char* fileName);
		virtual ~FileParser_Mock() override final;
	private:
		FileParser_Mock() = delete;

		virtual void CloseFile_impl() override final;

		virtual int GetBytesRemained_impl() const override final;
		virtual void SetBytesRemained_impl(int value) const override final;

		std::unique_ptr<std::string> ReadLine();
		int ReadNumber();
		virtual int PeekByte_impl() override final;
		virtual char ReadByte_impl() override final;
		virtual void ReadData_impl(char* data, std::streamsize count) override final;
		virtual void SkipData_impl(std::streamoff offset) override final;

		virtual unsigned ReadInverse_impl(unsigned nBytes, bool toCheck) override final;
		virtual unsigned ReadVarLenFormat_impl() override final;
	};
	uint32_t ReadWord(std::shared_ptr<FileParser_Mock>);	// Word = 4 bytes!!!
	uint16_t ReadDWord(std::shared_ptr<FileParser_Mock>);	// DWord = 2 bytes!!!
}