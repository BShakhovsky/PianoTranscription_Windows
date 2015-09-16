# include "stdafx.h"
# include "MetaEvent_Tempo.h"
# include "FileParser.h"
# include "MidiStruct.h"

using std::endl;
using boost::serialization::singleton;
using namespace Model::MidiParser;
using MidiStruct::TrackEvent;

MetaEvent_Tempo& MetaEvent_Tempo::GetInstance()
{
	return singleton<MetaEvent_Tempo>::get_mutable_instance();
}

void MetaEvent_Tempo::Read_impl()
{
	if (3 != GetInputFile()->PeekByte()) ADD_FAILURE() << "Wrong tempo chunk length";

	GetChunk()->length = GetInputFile()->ReadVarLenFormat();
	GetChunk()->metaData = GetInputFile()->ReadInverse(GetChunk()->length, true);

	if (GetChunk()->metaData) SUCCESS << "Tempo setting "
		<< (TrackEvent::microSec * TrackEvent::minute / GetChunk()->metaData)	// not covered by unit test
		<< " BPM" << endl;
	else assert(!"WRONG TEMPO: DIVISION BY ZERO");
}