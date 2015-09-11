# include "stdafx.h"
# include "MetaEvent.h"
# include "MidiStruct.h"
# include "FileParser.h"

using namespace std;
using namespace boost;
using serialization::singleton;
using namespace Model::MidiParser;
using MidiStruct::TrackEvent;

MetaEvent& MetaEvent::GetInstance()
{
	return singleton<MetaEvent>::get_mutable_instance();
}

void MetaEvent::Read_impl()
{
//	GetChunk()->Clear();
//	GetChunk()->status = -1;	// 0xFF
	GetChunk()->metaType = GetInputFile()->ReadByte();

	switch (GetChunk()->metaType)
	{
	case 0x00: if (2 != GetInputFile()->PeekByte()) cout << "\nWARNING: Wrong sequence chunk length" << endl;
				ReadText("Sequence number");		break;
	case 0x01:	ReadText("Text event");				break;
	case 0x02:	ReadText("Copyright notice");		break;
	case 0x03:	ReadText("Sequence or track name");	break;
	case 0x04:	ReadText("Instrument name");		break;
	case 0x05:	ReadText("Lyric text");				break;
	case 0x06:	ReadText("Marker text");			break;
	case 0x07:	ReadText("Cue point");				break;

	case 0x20: if (1 != GetInputFile()->PeekByte()) cout << "\nWARNING: Wrong MIDI channel chunk length" << endl;
		SkipMsg("MIDI channel prefix assignment");	break;
	case 0x21: if (1 != GetInputFile()->PeekByte()) cout << "\nWARNING: Wrong MIDI port chunk length" << endl;
		SkipMsg("MIDI port");						break;
	case 0x2F: assert("SOMETHING IS WRONG IN PROGRAM ARITHMETICS" && 1 == GetInputFile()->GetBytesRemained());
		if (GetInputFile()->ReadByte()) cout << "\nWARNING: Corrupted MIDI's end of track";
		break;

	case 0x51: if (3 != GetInputFile()->PeekByte()) cout << "\nWARNING: Wrong tempo chunk length" << endl;
		GetChunk()->metaData = GetInputFile()->ReadInverse(GetChunk()->length = GetInputFile()->ReadVarLenFormat(), true);
		GetChunk()->metaText += (format{ "Tempo setting %1% BPM\n" } %
			(TrackEvent::microSec * TrackEvent::minute / GetChunk()->metaData)).str();
		break;

	case 0x54: if (5 != GetInputFile()->PeekByte()) cout << "\nWARNING: Wrong SMPTE offset chunk length" << endl;
		SkipMsg("SMPTE offset");					break;
	case 0x58: if (4 != GetInputFile()->PeekByte()) cout << "\nWARNING: Wrong time signature chunk length" << endl;
		SkipMsg("Time signature");					break;

	case 0x59: ReadKeySignature();					break;

	case 0x7F: SkipMsg("Sequencer specific event");	break;

	default: throw runtime_error("WRONG META TYPE"); break;
	}
}

void MetaEvent::ReadText(const char *eventMsg) const
{
	cout << eventMsg << '\t';

	const auto length = GetInputFile()->ReadVarLenFormat();	// may throw std::length_error
	vector<char> result(length + 4);	// plus quotes, new line and null-byte in the end
	GetInputFile()->ReadData(result.data() + 1, length);

	*(result.data()) = '\"';
	auto resultEnd = result.data() + length;
	*++resultEnd = '\"';
	*++resultEnd = '\n';
	*++resultEnd = '\0';

	cout << result.data();
}

void MetaEvent::ReadKeySignature() const
{
	cout << "Key signature : ";
	if (const auto length = GetInputFile()->ReadVarLenFormat() != 2)
	{
		cout << "\nWARNING: Wrong key signature chunk length";
		GetInputFile()->SkipData(length);
		cout << length << " bytes skipped" << endl;
	}
	else
	{
		const auto sf(GetInputFile()->ReadByte());
		switch (sf)
		{
		case -7: case -6: case -5: case -4: case -3: case -2: case -1:	cout << sf << " bemoles";				break;
		case 0:															cout << sf << " natural key signature";	break;
		case 1: case 2: case 3: case 4: case 5: case 6: case 7:			cout << sf << " dieses";				break;
		default: throw runtime_error("WRONG KEY SIGNATURE, SHOULD BE 0...7 BEMOLES OR DIESES");					break;
		}
		switch (GetInputFile()->ReadByte())	// mi
		{
		case 0:									cout << ", major key"; if (!sf) cout << " (Do-Major)" << endl;	break;
		case 1:									cout << ", minor key"; if (!sf) cout << " (Lya-Minor)" << endl;	break;
		default: throw runtime_error("WRONG KEY SIGNATURE, SHOULD BE EITHER MAJOR OR MINOR");					break;
		}
	}
}