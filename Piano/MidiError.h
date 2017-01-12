#pragma once
#include "Piano3D\RusError.h"

class MidiError : public RusError
{
public:
	MidiError() = default;
	explicit MidiError(const char* msg) : RusError(msg) {}
	explicit MidiError(const wchar_t* rusMsg) : RusError(rusMsg) {}
};