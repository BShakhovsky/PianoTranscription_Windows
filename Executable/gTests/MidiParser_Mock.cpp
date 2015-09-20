# include "stdafx.h"
# include "MidiParser_Mock.h"
# include "..\..\Model\MidiParserLib\MidiStruct.h"

using namespace std;
using namespace Model::MidiParser::MidiStruct;
using gTest::MidiParser_Mock;


int MidiParser_Mock::counter_ = -1;


const ChunkIntro SetChunkIntro(const char c0, const char c1, const char c2, const char c3)
{
	ChunkIntro result;

	result.length = NULL;	// initialized, otherwise = 0xCC'CC'CC'CC

	result.type.type[0] = c0;
	result.type.type[1] = c1;
	result.type.type[2] = c2;
	result.type.type[3] = c3;
	
	return result;
}

# define RAND_MIN(TYPE, MIN_VAL) static_cast<TYPE>(rand() % (TYPE ## _MAX - (MIN_VAL)) + (MIN_VAL))


const ChunkIntro MidiParser_Mock::ReadChunkIntro_impl() const
{
	auto result(SetChunkIntro('W', 'r', 'n', 'g'));

	switch (++counter_)
	{
	case 0: srand(static_cast<unsigned>(time(NULL)));											break;
	case 1: result = SetChunkIntro('M', 'T', 'h', 'd');											break;
	case 2: result = SetChunkIntro('M', 'T', 'h', 'd');	result.length = 5;						break;
	case 3: result = SetChunkIntro('M', 'T', 'h', 'd');	result.length = RAND_MIN(UINT32, 7);	break;
	case 4: case 5: case 6: case 7: case 8: case 9: case 10:
			result = SetChunkIntro('M', 'T', 'h', 'd');	result.length = 6;						break;

	case 11:																					break;
	case 12: result = SetChunkIntro('M', 'T', 'r', 'k');										break;

	default: assert(!"SHOULD NEVER GET HERE");
	}

	return result;
}

const HeaderData MidiParser_Mock::ReadHeaderData_impl() const
{
	HeaderData result = { NULL, NULL, NULL };

	switch (counter_)
	{
	case 4:																						break;
	case 5:	result.tracks = RAND_MIN(UINT16, 2);												break;
	case 6: result.tracks = 1;																	break;
	
	case 7: result.format = 1;																	break;
	case 8: result.format = RAND_MIN(UINT16, 3);												break;
	case 9: result.format = 2;																	break;

	case 10: result.format = 2; result.division = RAND_MIN(UINT16, 1);							break;
	default: assert(!"SHOULD NEVER GET HERE");
	}

	return result;
}


vector<TrackEvent> MidiParser_Mock::ReadTrackEvents_impl(const uint32_t) const
{
	return vector<TrackEvent>();
}