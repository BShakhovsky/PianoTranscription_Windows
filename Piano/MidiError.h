#pragma once
#include "Piano3D\RusError.h"

class MidiError : public RusError
{
public:
	MidiError() = default;
#pragma warning(push)
#pragma warning(disable:4514) // Unreferenced inline function has been removed
	explicit MidiError(const char* msg) : RusError(msg) {}
	explicit MidiError(const wchar_t* rusMsg) : RusError(rusMsg) {}
#pragma warning(pop)
};