# include "stdafx.h"
# include "MetaEvent_Text.h"
# include "FileParser.h"
# include "MidiStruct.h"

using namespace std;
using boost::serialization::singleton;
using Model::MidiParser::MetaEvent_Text;

MetaEvent_Text& MetaEvent_Text::GetInstance()
{
	return singleton<MetaEvent_Text>::get_mutable_instance();
}

void MetaEvent_Text::Read_impl()
{
	switch (GetChunk()->metaType)
	{
	case 0x00: if (2 != GetInputFile()->PeekByte()) cout << "WARNING: Wrong sequence chunk length" << endl;
				ReadText("Sequence number");		break;
	case 0x01:	ReadText("Text event");				break;
	case 0x02:	ReadText("Copyright notice");		break;
	case 0x03:	ReadText("Sequence or track name");	break;
	case 0x04:	ReadText("Instrument name");		break;
	case 0x05:	ReadText("Lyric text");				break;
	case 0x06:	ReadText("Marker text");			break;
	case 0x07:	ReadText("Cue point");				break;
	}
}

void MetaEvent_Text::ReadText(const char *eventMsg) const
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