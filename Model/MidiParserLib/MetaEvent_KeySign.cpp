# include "stdafx.h"
# include "MetaEvent_KeySign.h"
# include "MetaEvent_KeySign_printer.h"
# include "IFileParser.h"

using std::make_unique;
using namespace Model::MidiParser;

MetaEvent_KeySign::MetaEvent_KeySign(const char statusByte, const char metaType) :
	MetaEvent(statusByte, metaType),
	pimpl_(make_unique<MetaEvent_KeySign_printer>())
{}

MetaEvent_KeySign::~MetaEvent_KeySign() {}

void MetaEvent_KeySign::Read_impl()
{
	const auto length = GetInputFile()->ReadVarLenFormat();
	if (2 != length)
	{
		GetInputFile()->SkipData(length);
		WARNING("Wrong key signature chunk length, " << length << " bytes skipped");
	}
	else
	{
		pimpl_->KeySignInit(GetInputFile());
		pimpl_->PrintKeySignature();	// not covered by unit tests
	}
}