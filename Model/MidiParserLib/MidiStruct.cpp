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

const int
TrackEvent::microSec = static_cast<int>(micro::den / micro::num),
TrackEvent::milliSec = static_cast<int>(milli::den / milli::num),
TrackEvent::minute = static_cast<int>(duration_cast<seconds>(minutes(1)).count());