# include "stdafx.h"
# include "Event.h"
# include "MidiStruct.h"
# include "IFileParser.h"

using namespace std;
using namespace Model::MidiParser;
using MidiStruct::EventChunk;

Event::FileParser_ Event::fileParser_ = nullptr;

Event::Event(const char statusByte) :
	eventChunk_(make_shared<EventChunk>())
{
	memset(eventChunk_.get(), NULL, sizeof (*eventChunk_));
	eventChunk_->status = statusByte;
}

Event::~Event()
{
	fileParser_.reset();
}


unique_ptr<Event> Event::GetInstance(FileParser_ file)
{
	if (file) fileParser_ = file;
	else if (!fileParser_) throw std::runtime_error("INPUT FILE HAS NOT BEEN SET YET");

	const auto status(fileParser_->ReadByte());
	if ((status & 0x0'F0) == 0xF0)	// 0xF0 is negative ==> 0x0F0 is positive
		if (status == -1) /* 0xFF */	return MetaEvent::GetInstance(status);
		else							return SystemEvent::GetInstance(status);
	else								return MidiEvent::GetInstance(status);
}

Event::EventChunk_ Event::Read()
{
	if (!fileParser_) assert(!"INPUT FILE HAS NOT BEEN SET YET");
	Read_impl();
	return eventChunk_;
}


void Event::SkipEvent() const
{
	fileParser_->SkipData(fileParser_->ReadVarLenFormat());
}