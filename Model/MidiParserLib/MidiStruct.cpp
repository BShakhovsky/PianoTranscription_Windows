#include "stdafx.h"
#include "MidiStruct.h"

using namespace std;
using namespace chrono;
using namespace Model::MidiParser::MidiStruct;

const int
Bytes::varLengthSize = sizeof(DWORD),
Bytes::byteSize = numeric_limits<BYTE>::digits;

const ChunkType
ChunkIntro::HEADER{ 'M', 'T', 'h', 'd' },
ChunkIntro::TRACK{ 'M', 'T', 'r', 'k' };

const long long
TrackEvent::microSec = micro::den / micro::num,
TrackEvent::minute = duration_cast<seconds>(minutes(1)).count();