# include "stdafx.h"
# include "MetaEvent_KeySign_printer.h"
# include "IFileParser.h"

using namespace std;
using Model::MidiParser::MetaEvent_KeySign_printer;

MetaEvent_KeySign_printer::MetaEvent_KeySign_printer() :
	inputFile_(nullptr),
	sf(NULL)
{}

MetaEvent_KeySign_printer::~MetaEvent_KeySign_printer() {}

void MetaEvent_KeySign_printer::KeySignInit(FileParser_ newFile)
{
	inputFile_ = newFile;
}



void MetaEvent_KeySign_printer::PrintKeySignature()
{
	if (!inputFile_) throw runtime_error("INPUT FILE HAS NOT BEEN SET YET");
	sf = inputFile_->ReadByte();

	cout << "\nKey signature: ";
	PrintKeySF();
	PrintKeyMI();
}

void MetaEvent_KeySign_printer::PrintKeySF() const
{
	switch (sf)
	{
	case-7:case-6:case-5:case-4:case-3:case-2:case-1:	cout << sf << " bemoles, ";			break;
	case 0:												cout << "natural key signature, ";	break;
	case 1:case 2:case 3:case 4:case 5:case 6:case 7:	cout << sf << " dieses, ";			break;
	default: throw runtime_error("WRONG KEY SIGNATURE, SHOULD BE 0...7 BEMOLES OR DIESES");
	}
}

void MetaEvent_KeySign_printer::PrintKeyMI() const
{
	if (!inputFile_) assert(!"INPUT FILE HAS NOT BEEN SET YET");
	switch (inputFile_->ReadByte())	// mi
	{
	case 0:										cout << (sf ? "major key" : "Do-Major");	break;
	case 1:										cout << (sf ? "minor key" : "Lya-Minor");	break;
	default: throw runtime_error("WRONG KEY SIGNATURE, SHOULD BE EITHER MAJOR OR MINOR");
	}
}