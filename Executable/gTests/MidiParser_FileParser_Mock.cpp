# include "stdafx.h"
# include "MidiParser_FileParser_Mock.h"
# include "..\..\Model\MidiParserLib\FileCounter.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using namespace std;
using namespace Model::MidiParser;
using MidiStruct::Bytes;
using gTest::FileParser_Mock;

FileParser_Mock::FileParser_Mock(const char* fileName) :
	IFileParser(),
	inputFile_(fileName, std::ifstream::in),
	bytesRemained_(make_shared<FileCounter>())
{}
FileParser_Mock::~FileParser_Mock() {}

void FileParser_Mock::CloseFile_impl()
{
	BORIS_ASSERT("VIRTUAL " __FUNCTION__ " HAS NOT BEEN OVERRIDEN");
}

int FileParser_Mock::GetBytesRemained_impl() const
{
	return bytesRemained_->Get();
}
void FileParser_Mock::SetBytesRemained_impl(const int value) const
{
	bytesRemained_->Set(value);
}

unique_ptr<string> FileParser_Mock::ReadLine()
{
	auto str(make_unique<string>(""));
	getline(inputFile_, *str);
	return str;
}
int FileParser_Mock::ReadNumber()
{
	auto result(NULL);
	istringstream(*ReadLine()) >> result;
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
void FileParser_Mock::ReadData_impl(char* data, std::streamsize count)
{
	bytesRemained_->Reduce(static_cast<int>(count), false);
	for (; count--; ++data)
	{
		auto result('\0');
		istringstream(*ReadLine()) >> result;
		*data = result ? result : ' ';
	}
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

uint32_t gTest::ReadWord(std::shared_ptr<FileParser_Mock>)	// Word = 4 bytes!!!
{
	BORIS_ASSERT("TEMPLATE " __FUNCTION__ " HAS NOT BEEN SPECIALIZED");
	return NULL;
}
uint16_t gTest::ReadDWord(std::shared_ptr<FileParser_Mock>)	// DWord = 2 bytes!!!
{
	BORIS_ASSERT("TEMPLATE " __FUNCTION__ " HAS NOT BEEN SPECIALIZED");
	return NULL;
}