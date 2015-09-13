# include "stdafx.h"
# include "MetaEvent_Tempo.h"
# include "FileParser.h"
# include "MidiStruct.h"

using namespace std;
using boost::serialization::singleton;
using namespace Model::MidiParser;
using MidiStruct::TrackEvent;

MetaEvent_Tempo& MetaEvent_Tempo::GetInstance()
{
	return singleton<MetaEvent_Tempo>::get_mutable_instance();
}

void MetaEvent_Tempo::Read_impl()
{
	if (3 != GetInputFile()->PeekByte()) cout << "WARNING: Wrong tempo chunk length" << endl;
	GetChunk()->length = GetInputFile()->ReadVarLenFormat();
	GetChunk()->metaData = GetInputFile()->ReadInverse(GetChunk()->length, true);
	cout << "Tempo setting " << (TrackEvent::microSec * TrackEvent::minute / GetChunk()->metaData) << " BPM" << endl;
}