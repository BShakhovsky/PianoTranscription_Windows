# pragma once
namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct Bytes;
		}
		class FileParser : private boost::noncopyable
		{
			std::ifstream inputFile_;
			int bytesRemained_;
		public:
			explicit FileParser(const char *fileName) :
				inputFile_(fileName, std::ifstream::binary),
				bytesRemained_(NULL)
			{}
			~FileParser() = default;

			int PeekByte()
			{
				return inputFile_.peek();
			}
			char ReadByte()
			{
				char result('\0');
				inputFile_.get(result);
				--bytesRemained_;
				CheckBytesRemained();
				return result;
			}
			void ReadData(char* data, std::streamsize count)
			{
				inputFile_.read(data, count);
				bytesRemained_ -= static_cast<int>(count);
			}
			void SkipData(std::streamoff offset)
			{
				inputFile_.seekg(offset, std::ifstream::cur);
				bytesRemained_ -= static_cast<int>(offset);
				CheckBytesRemained();
			}

			unsigned ReadInverse(unsigned nBytes, bool checkBytesRemained = false);
			unsigned ReadVarLenFormat();	// may throw std::length_error

			int GetBytesRemained() const
			{
				return bytesRemained_;
			}
			void SetBytesRemained(int value)
			{
				assert("LOGICAL ERROR: BYTES TO READ FROM MIDI FILE SHOULD BE A POSITIVE NUMBER" && value >= 0);
				bytesRemained_ = value;
			}
			int ReduceBytesRemained(int value)
			{
				return (bytesRemained_ -= value);
				CheckBytesRemained();
			}
		private:
			bool CheckBytesRemained() const
			{
				assert("LOGICAL ERROR IN COUNTING BYTES REMAINED TO READ FROM MIDI FILE" && bytesRemained_ >= 0);
				return true;
			}
			const uint32_t UNUSED_ = NULL;	// 4 padding bytes
		};
		uint32_t ReadWord(std::shared_ptr<FileParser> fileParser);	// Word = 4 bytes!!!
		uint16_t ReadDWord(std::shared_ptr<FileParser> fileParser);	// DWord = 2 bytes!!!
	}
}