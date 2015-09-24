# include "stdafx.h"
# include "FileParser.h"
# include "FileCounter.h"
# include "MidiStruct.h"

using namespace std;
using namespace Model;
using namespace MidiParser;
using MidiStruct::Bytes;

FileParser::FileParser(const char *fileName) : IFileParser(),
	inputFile_(fileName, ifstream::binary),
	bytesRemained_(make_shared<FileCounter>())
{
	if (inputFile_.fail()) throw runtime_error(string("CANNOT OPEN INPUT FILE ") + fileName);
}

void FileParser::CloseFile_impl()
{
	inputFile_.close();
	if (inputFile_.fail()) throw runtime_error("CANNOT CLOSE INPUT FILE");
}

int FileParser::GetBytesRemained_impl() const
{
	return bytesRemained_->Get();
}
void FileParser::SetBytesRemained_impl(const int value) const
{
	bytesRemained_->Set(value);
}

# define CHECK_FLAGS {	if (inputFile_.eof()) throw length_error("END OF INPUT FILE IS REACHED");	\
					else if (inputFile_.fail()) throw runtime_error(__FUNCTION__);					}

int FileParser::PeekByte_impl()
{
	const auto result(inputFile_.peek());
	CHECK_FLAGS;
	return result;
}
char FileParser::ReadByte_impl()
{
	char result('\0');
	inputFile_.get(result);
	CHECK_FLAGS;
	bytesRemained_->Reduce(1);
	return result;
}
void FileParser::ReadData_impl(char* data, const std::streamsize count)
{
	inputFile_.read(data, count);
	CHECK_FLAGS;
	bytesRemained_->Reduce(static_cast<int>(count), false);
}
void FileParser::SkipData_impl(const std::streamoff offset)
{
	inputFile_.clear();	// remove EOF flag if set
	inputFile_.seekg(offset, std::ifstream::cur);
	CHECK_FLAGS;
	bytesRemained_->Reduce(static_cast<int>(offset));
}

unsigned FileParser::ReadInverse_impl(unsigned nBytes, const bool toCheck)
// Inverse from little-endian to big-endian format
{
	if (nBytes > sizeof(int32_t))
	{
		assert(!"NUMBER OF BYTES TO READ > SIZEOF INT");
		nBytes = sizeof(int32_t);
	}
	unsigned result(NULL);
	for (unsigned i(NULL); i < nBytes; ++i)
	{
		result <<= MidiStruct::Bytes::byteSize;
		result |= inputFile_.get();
		CHECK_FLAGS;
	}
	bytesRemained_->Reduce(static_cast<signed>(nBytes), toCheck);
	return result;
}
unsigned FileParser::ReadVarLenFormat_impl()
{
	unsigned result(NULL);
	char anotherByte('\0');
	auto totalBytes(NULL);
	do
	{
		anotherByte = ReadByte();
		if (++totalBytes > Bytes::varLengthSize)
			throw length_error("UNEXPECTED VARIABLE LENGTH > FOUR BYTES");
		result <<= Bytes::byteSize - 1;
		result |= static_cast<unsigned char>(anotherByte & 0x7F);
	} while (anotherByte < 0);	// ends when the most significant bit is unset

	return result;
}

uint32_t MidiParser::ReadWord(shared_ptr<IFileParser> fileParser)
{
	return fileParser->ReadInverse(sizeof uint32_t, false);
}
uint16_t MidiParser::ReadDWord(shared_ptr<IFileParser> fileParser)
{
	return static_cast<uint16_t>(fileParser->ReadInverse(sizeof uint16_t, false));
}