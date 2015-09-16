# include "stdafx.h"
# include "FileParser_Mock.h"
# include "..\..\Model\MidiParserLib\FileCounter.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using namespace std;
using namespace Model;
using namespace MidiParser;
using MidiStruct::Bytes;

FileParser_Mock::FileParser_Mock(const char* fileName) :
	IFileParser(),
	inputFile_(fileName, std::ifstream::in),
	bytesRemained_(make_shared<FileCounter>())
{}
FileParser_Mock::~FileParser_Mock() {}


int FileParser_Mock::GetBytesRemained_impl() const
{
	return bytesRemained_->Get();
}
void FileParser_Mock::SetBytesRemained_impl(const int value) const
{
	bytesRemained_->Set(value);
}


int FileParser_Mock::ReadNumber()
{
	string str;
	getline(inputFile_, str);

	auto result(NULL);
	istringstream(str) >> result;
	return result;
}

int FileParser_Mock::PeekByte_impl()
{
	const auto pos(inputFile_.tellg());
	const auto result(ReadNumber());
	inputFile_.seekg(pos);
	return result;
}
char FileParser_Mock::ReadByte_impl()
{
	bytesRemained_->Reduce(1);
	return static_cast<char>(ReadNumber());
}
void FileParser_Mock::ReadData_impl(char*, std::streamsize)
{
	BORIS_ASSERT("VIRTUAL FUNCTION " __FUNCTION__ " HAS NOT BEEN OVERRIDEN");
}
void FileParser_Mock::SkipData_impl(std::streamoff offset)
{
	assert("CURRENT MOCK OBJECT CAN HANDLE ONLY FORWARD MOVES" && offset >= 0);
	for (auto i(NULL); i < offset; ++i) ReadByte();
}

unsigned FileParser_Mock::ReadInverse_impl(unsigned nBytes, const bool toCheck)
{
	if (nBytes > sizeof(int32_t))
	{
		assert(!"NUMBER OF BYTES TO READ > SIZEOF INT");
		nBytes = sizeof(int32_t);
	}
	unsigned result(NULL);
	for (unsigned i(NULL); i < nBytes; ++i) result += ReadNumber();
	bytesRemained_->Reduce(static_cast<signed>(nBytes), toCheck);
	return result;
}
unsigned FileParser_Mock::ReadVarLenFormat_impl()
{
	auto result(NULL),
		totalBytes(NULL);
	auto anotherByte('\0');

	for (; (anotherByte = ReadByte_impl()) < 0; result -= anotherByte)	// ends with the positive byte
		if (++totalBytes >= Bytes::varLengthSize) throw length_error("UNEXPECTED VARIABLE LENGTH > FOUR BYTES");
	return static_cast<unsigned>(result + anotherByte);
}


uint32_t MidiParser::ReadWord(std::shared_ptr<FileParser_Mock>)	// Word = 4 bytes!!!
{
	BORIS_ASSERT("TEMPLATE " __FUNCTION__ " HAS NOT BEEN SPECIALIZED");
	return NULL;
}
uint16_t MidiParser::ReadDWord(std::shared_ptr<FileParser_Mock>)	// DWord = 2 bytes!!!
{
	BORIS_ASSERT("TEMPLATE " __FUNCTION__ " HAS NOT BEEN SPECIALIZED");
	return NULL;
}