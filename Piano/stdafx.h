#pragma once
#include "targetver.h"

#pragma warning(disable:4514 4711)
#pragma warning(push, 3)
#	include <Windows.h>
#	include <WindowsX.h>
#pragma warning(pop)
#include <CommCtrl.h>

#pragma warning(push)
#pragma warning(disable:4710)
#	include <iostream>
#	include <map>
#	include <numeric>
#	include <set>
#pragma warning(pop)

#pragma warning(disable:4625 4626 5026 5027)
#pragma warning(push, 3)
#	include <boost\noncopyable.hpp>
#pragma warning(pop)

#pragma warning(disable:4710)
#pragma warning(push, 2)
#	include <boost\lexical_cast.hpp>
#	include <boost\format.hpp>
#pragma warning(pop)

#pragma warning(push, 3)
#pragma warning(disable:6011 6297 6387 28182)
#	include <Juce\JuceHeader.h>
#pragma warning(pop)
#pragma comment(lib, "Juce.lib")