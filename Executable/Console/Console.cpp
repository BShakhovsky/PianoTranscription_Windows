# include "stdafx.h"
# include "..\..\Model\MidiPlayback_Lib\MidiPlayback.h"

using namespace std;
using Model::MidiPlayback::MidiPlayback;

int main()
{
	MidiPlayback::Execute("../../../Test.mid");
}