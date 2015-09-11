# include "stdafx.h"
# include "..\..\Model\MidiTracksCompiler.h"
# include "..\..\CodeMetrics\CodeMetrics.h"

using namespace std;
using Model::MidiParser::MidiTracksCompiler;
using Unmanaged_StatLib::CodeMetrics;

int main()
{
# ifdef _DEBUG
	cout << "Solution total number of non-blank lines of code: " << CodeMetrics().SolutionLines("../..", true)
		<< "\n\tUnit tests: " << CodeMetrics().SolutionLines("../gTests")
		<< "\n\tModel: " << CodeMetrics().SolutionLines("../../Model") << '\n' << endl;
# endif
	try
	{
		MidiTracksCompiler midiFile("Midi File.mid");
		while (midiFile.LoadNextTrack())
			if (_kbhit())
			{
				auto ignored(_getch());
				++ignored;
				midiFile.UnPause();
			}
		system("Pause");
		midiFile.ShowKeyboard();
	}
	catch (const std::exception& e)
	{
		cout << e.what() << endl;
		system("Pause");
		throw;
	}

	return NULL;
}