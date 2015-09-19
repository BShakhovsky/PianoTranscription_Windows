# pragma once
# include "Event.h"

EVENT_DECL(Midi)
	static char runStatus_;
	void PrintMidiEvent() const;
}; } }