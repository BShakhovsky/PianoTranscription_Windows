# include "stdafx.h"
# include "MetaEvent_KeySign.h"
# include "MetaEvent_KeySign_pimpl.h"
# include "FileParser.h"

using namespace std;
using boost::serialization::singleton;
using namespace Model::MidiParser;

MetaEvent_KeySign::MetaEvent_KeySign() :
	MetaEvent(),
	pimpl_(make_unique<MetaEvent_KeySign_pimpl>())
{}

MetaEvent_KeySign::~MetaEvent_KeySign() {}

MetaEvent_KeySign& MetaEvent_KeySign::GetInstance()
{
	return singleton<MetaEvent_KeySign>::get_mutable_instance();
}

void MetaEvent_KeySign::Read_impl()
{
	const auto length = GetInputFile()->ReadVarLenFormat();
	if (2 != length)
	{
		GetInputFile()->SkipData(length);
		FAIL() << "Wrong key signature chunk length, " << length << " bytes skipped";
	}
	else
	{
		pimpl_->KeySignInit(GetInputFile());
		pimpl_->PrintKeySignature();	// not covered by unit tests
	}
}