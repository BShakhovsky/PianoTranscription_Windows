# include "stdafx.h"
# include "MidiTracksCollector.h"
# include "MidiChunksReader.h"
# include "MidiStruct.h"

using namespace std;
using namespace Model::MidiParser;
using namespace MidiStruct;

MidiTracksCollector::MidiTracksCollector(const char* fileName) :
	midiFile_(make_unique<MidiChunksReader>(fileName)),
	headerData_(nullptr),
	tracks_()
{}

MidiTracksCollector::~MidiTracksCollector() {}

void MidiTracksCollector::ReadMidiFile()
{
	if (!tracks_.empty()) { BORIS_ASSERT(__FUNCTION__ " MUST NOT BE CALLED TWICE"); }
	else
	headerData_ = make_shared<HeaderData>(midiFile_->ReadHeaderChunk().data);
	ReadTracks();
}

void MidiTracksCollector::ReadTracks()
{
	tracks_.reserve(headerData_->tracks);
	for (uint16_t i(0); i < headerData_->tracks; ++i)
	{
		const auto result(midiFile_->ReadTrackChunk().trackEvent);
		if (!result.empty()) tracks_.push_back(result);	// may throw std::runtime_error
		cout << "\n\nEnd of track " << i + 1 << " of " << headerData_->tracks << endl;
	}
}