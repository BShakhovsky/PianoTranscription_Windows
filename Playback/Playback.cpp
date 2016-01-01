# include "stdafx.h"
# include "Playback.h"
# include "MidiParser_Facade.h"
# include "..\Interface\View\ViewFactory.h"
# include "..\Interface\Sound\Sound_Lib\DirectHarmonics.h"

void Playback::Execute(const HWND hWnd, const char* fileName)
{
	using std::vector;
	using namespace Interface;
	using View::GetKeyboard;
	using Sound::DirectHarmonics;

	MidiParser_Facade data(fileName);

	std::vector<int16_t> notes;
	std::vector<unsigned> times;
	const auto allNotes(data.GetNotes());
	for (const auto& note : allNotes)
		if (!note.empty())
		{
			notes = note;
			times = data.GetMilliSeconds().at(static_cast<size_t>(&note - &*allNotes.cbegin()));
			break;
		}
	if (!notes.empty() && !times.empty())
	{
		auto time(times.cbegin());
		auto lastTime(*time);
		const auto threshold(7);

		const auto keyboard(GetKeyboard());
		DirectHarmonics sound(hWnd);
		const auto timeStart(times.front());

		for (auto note(notes.cbegin()); note != notes.cend(); ++note, ++time)
		{
			if (*time - lastTime > threshold)
			{
				sound.Play();
				keyboard->Update();
//				Sleep(*time - lastTime);
				sound.Clear();
				keyboard->ReleaseAllKeys();

				lastTime = *time;

				system("Pause");
			}
			keyboard->PressKey(*note);
			sound.AddNote(*note);
		}
	}
}