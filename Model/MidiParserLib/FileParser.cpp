# include "stdafx.h"
# include "FileParser.h"
# include "MidiStruct.h"

using std::shared_ptr;
using namespace Model;
using namespace MidiParser;
using MidiStruct::Bytes;

unsigned FileParser::ReadInverse(unsigned nBytes, bool checkBytesRemained)
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
	bytesRemained_ -= nBytes;
	if (checkBytesRemained) CheckBytesRemained();
	return result;
}

unsigned FileParser::ReadVarLenFormat()
{
	unsigned result(NULL);
	char anotherByte('\0');
	auto totalBytes(NULL);
	do
	{
		anotherByte = ReadByte();
		if (++totalBytes > Bytes::varLengthSize)
			throw std::length_error("UNEXPECTED VARIABLE LENGTH > FOUR BYTES");
		result <<= Bytes::byteSize;
		result |= static_cast<unsigned char>(anotherByte);
	} while (anotherByte < 0);	// ends when the most significant bit is unset

	return result;
}

uint32_t MidiParser::ReadWord(shared_ptr<FileParser> fileParser)
{
	return fileParser->ReadInverse(sizeof uint32_t);
}

uint16_t MidiParser::ReadDWord(shared_ptr<FileParser> fileParser)
{
	return static_cast<uint16_t>(fileParser->ReadInverse(sizeof uint16_t));
}