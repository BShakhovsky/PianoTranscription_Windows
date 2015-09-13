# include "stdafx.h"
# include "Event.h"
# include "MidiStruct.h"
# include "FileParser.h"

using namespace std;
using boost::serialization::singleton;
using namespace Model::MidiParser;
using MidiStruct::EventChunk;

Event::FileParser_ Event::fileParser_ = nullptr;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
Event::Event() :
	eventChunk_(make_shared<EventChunk>())
{}
# pragma warning(pop)

Event& Event::Init(const char statusByte)
{
	eventChunk_->status = statusByte;
	return *this;
}

Event& Event::GetInstance(FileParser_ file)
{
	if (file) fileParser_ = file;
	else if (!fileParser_) throw std::runtime_error("INPUT FILE HAS NOT BEEN SET YET");

	const auto status = fileParser_->ReadByte();
	if ((status & 0x0'F0) == 0xF0)	// 0xF0 is negative ==> 0x0F0 is positive
		if (status == -1) /* 0xFF */	return MetaEvent::GetInstance().Init(status);
		else							return SystemEvent::GetInstance().Init(status);
	else								return MidiEvent::GetInstance().Init(status);
}

Event::~Event() {}

Event::EventChunk_ Event::Read()
{
	Read_impl();
	return eventChunk_;
}

void Event::SkipEvent() const
{
	eventChunk_->length = fileParser_->ReadVarLenFormat();
	fileParser_->SkipData(eventChunk_->length);
}