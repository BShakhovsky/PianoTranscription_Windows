# include "stdafx.h"
# include "..\MidiParser_Facade.h"
# include "MidiTimeCalculator.h"
# include "..\..\Interface\View\ViewFactory.h"

using namespace std;
using namespace Model::MidiParser;
using Interface::View::GetMidiLog;

MidiParser_Facade::MidiParser_Facade(const char* fileName) :
	midiFile_(make_unique<MidiTimeCalculator>()),
	times_(),
	notes_()
{
	Execute(fileName);
	midiFile_.reset(nullptr);
}

MidiParser_Facade::~MidiParser_Facade() {}

void MidiParser_Facade::Execute(const char* fileName)
{
	const auto logWindow(GetMidiLog());	// may throw std::runtime_error
	logWindow->Maximize();

	Step1(fileName);
	system("Pause");
	Step2();

	times_ = midiFile_->GetTimes();
	notes_ = midiFile_->GetNotes();

	// Console logWindow is closed automatically
}

void MidiParser_Facade::Step1(const char* fileName) const
{
	cout << R"%(
	==============================
	STEP 1 of 2: Reading MIDI data
	==============================
	)%" << endl;

	midiFile_->LoadMidiData(fileName);

	cout << "\nEnd of STEP 1 of 2: MIDI file " << fileName << " closed" << endl;
}

void MidiParser_Facade::Step2() const
{
	cout << R"%(
	=============================================================================
	STEP 2 of 2: Calculating MIDI notes time points (minute:seconds:milliseconds)
	=============================================================================
	)%" << endl;

	midiFile_->CalcDeltaTimes();
}