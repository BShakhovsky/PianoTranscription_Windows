# include "stdafx.h"
# include "MetaEvent_Tempo.h"
# include "FileParser.h"
# include "MidiStruct.h"

using std::cout;
using Model::MidiParser::MidiStruct::TrackEvent;

META_IMPL(Tempo)
{
	if (3 != GetInputFile()->PeekByte()) WARNING("Wrong tempo chunk length");

	if (GetChunk()->metaData = GetInputFile()->ReadInverse(GetInputFile()->ReadVarLenFormat(), true))	// = not ==
		cout << "\nTempo setting "
			<< (TrackEvent::microSec * TrackEvent::minute / GetChunk()->metaData)	// not covered by unit test
			<< " BPM";
	else assert(!"WRONG TEMPO: DIVISION BY ZERO");
}