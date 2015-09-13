# include "stdafx.h"
# include "MetaEvent_KeySign.h"
# include "FileParser.h"

using namespace std;
using boost::serialization::singleton;
using Model::MidiParser::MetaEvent_KeySign;

MetaEvent_KeySign& MetaEvent_KeySign::GetInstance()
{
	return singleton<MetaEvent_KeySign>::get_mutable_instance();
}

void MetaEvent_KeySign::Read_impl()
{
	cout << "Key signature : ";
	if (const auto length = GetInputFile()->ReadVarLenFormat() != 2)
	{
		cout << "WARNING: Wrong key signature chunk length";
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