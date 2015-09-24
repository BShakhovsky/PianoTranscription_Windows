# include "stdafx.h"
# include "..\MidiParser_Facade.h"
# include "MidiTimeCalculator.h"
# include "..\..\Interface\View\ViewFactory.h"

using namespace std;
using namespace Model::MidiParser;
using Interface::View::GetMidiLog;

MidiParser_Facade::MidiParser_Facade(const char* fileName) :
	midiFile_(new MidiTimeCalculator)
{
	Execute(fileName);
	delete midiFile_;
}

void MidiParser_Facade::Execute(const char* fileName) const
{
	const auto logWindow(GetMidiLog());	// may throw std::runtime_error
	logWindow->Maximize();

	Step1(fileName);
	system("Pause");
	Step2();
	system("Pause");
	Step3();

	// Console logWindow is closed automatically
}

void MidiParser_Facade::Step1(const char* fileName) const
{
	cout << R"%(
	==============================
	STEP 1 of 3: Reading MIDI data
	==============================
	)%" << endl;

	midiFile_->LoadMidiData(fileName);

	cout << "\nEnd of STEP 1 of 3: MIDI file " << fileName << " closed" << endl;
}

void MidiParser_Facade::Step2() const
{
	cout << R"%(
	=================================================
	STEP 2 of 3: Collecting tempos (beats per minute)
	=================================================
	)%" << endl;

	midiFile_->CollectTempos();
}

void MidiParser_Facade::Step3() const
{
	cout << R"%(
	=============================================================================
	STEP 3 of 3: Calculating MIDI notes time points (minute:seconds:milliseconds)
	=============================================================================
	)%" << endl;

	midiFile_->CalcDeltaTimes();
}