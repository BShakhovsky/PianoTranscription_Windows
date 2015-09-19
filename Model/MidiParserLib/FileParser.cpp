# include "stdafx.h"
# include "FileParser.h"
# include "FileCounter.h"
# include "MidiStruct.h"

using namespace std;
using namespace Model;
using namespace MidiParser;
using MidiStruct::Bytes;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
FileParser::FileParser(const char *fileName) :
	IFileParser(),
	inputFile_(fileName, std::ifstream::binary),
	bytesRemained_(make_shared<FileCounter>())
{}
# pragma warning(pop)

FileParser::~FileParser() {}

int FileParser::GetBytesRemained_impl() const
{
	return bytesRemained_->Get();
}
void FileParser::SetBytesRemained_impl(const int value) const
{
	bytesRemained_->Set(value);
}

char FileParser::ReadByte_impl()
{
	char result('\0');
	inputFile_.get(result);
	bytesRemained_->Reduce(1);
	return result;
}
void FileParser::ReadData_impl(char* data, const std::streamsize count)
{
	inputFile_.read(data, count);
	bytesRemained_->Reduce(static_cast<int>(count), false);
}
void FileParser::SkipData_impl(const std::streamoff offset)
{
	inputFile_.seekg(offset, std::ifstream::cur);
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
		result <<= Bytes::byteSize;
		result |= static_cast<unsigned char>(anotherByte);
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