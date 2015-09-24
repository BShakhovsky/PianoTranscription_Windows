# include "stdafx.h"
# include "MetaEvent_Tempo.h"
# include "IFileParser.h"
# include "MidiStruct.h"

using std::cout;
using namespace Model::MidiParser;

void MetaEvent_Tempo::Read_impl()
{
	if (3 != GetInputFile()->PeekByte()) WARNING("Wrong tempo chunk length");
	GetChunk()->metaData = GetInputFile()->ReadInverse(GetInputFile()->ReadVarLenFormat());
	assert("WRONG TEMPO: DIVISION BY ZERO" && GetChunk()->metaData);
}