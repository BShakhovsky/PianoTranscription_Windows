# pragma once
# include "IFileParser.h"

namespace Model
{
	namespace MidiParser
	{
		class FileParser : public IFileParser
		{
			const int32_t UNUSED = NULL;	// four padding bytes
			std::ifstream inputFile_;
			std::shared_ptr<class FileCounter> bytesRemained_;
		public:
			explicit FileParser(const char *fileName);
			virtual ~FileParser() override final = default;
		private:
			virtual void CloseFile_impl() override final;

			virtual int GetBytesRemained_impl() const override final;
			virtual void SetBytesRemained_impl(int value) const override final;

			virtual int PeekByte_impl() override final;
			virtual char ReadByte_impl() override final;
			virtual void ReadData_impl(char* data, std::streamsize count) override final;
			virtual void SkipData_impl(std::streamoff offset) override final;
			
			virtual unsigned ReadInverse_impl(unsigned nBytes, bool toCheck) override final;
			virtual unsigned ReadVarLenFormat_impl() override final;	// may throw std::length_error
		};

		uint32_t ReadWord(std::shared_ptr<IFileParser> fileParser);		// Word = 4 bytes!!!
		uint16_t ReadDWord(std::shared_ptr<IFileParser> fileParser);	// DWord = 2 bytes!!!
	}
}