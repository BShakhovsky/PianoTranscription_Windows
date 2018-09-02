#pragma once
#include "targetver.h"

#pragma warning(push, 3)
#pragma warning(disable:4514 5039)
#	include <Windows.h>
#	include <WindowsX.h>
#	include <CommCtrl.h>
#pragma warning(pop)

#pragma warning(push)
#pragma warning(disable:4365 4514 4571 4625 4626 4710 4711 4774 4820 5026 5027)
#	include <iostream>
#	include <map>
#	include <numeric>
#	include <set>
#pragma warning(pop)

#pragma warning(push, 2)
#pragma warning(disable:4365 4514 4571 4625 4626 4710 4711 4774 4820 5026 5027 26439 26444 26451 26495)
#	include <boost\format.hpp>
#	include <boost\lexical_cast.hpp>
#pragma warning(pop)

#pragma warning(push, 3)
#pragma warning(disable:4711 6011 6297 6387 26439 26451 26495 26498 28182)
#	include <Juce\JuceHeader.h>
#pragma warning(pop)
#pragma comment(lib, "Juce.lib")

#pragma warning(disable:5045) // Compiler will insert Spectre mitigation for memory load