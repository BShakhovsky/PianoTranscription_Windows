#pragma once
#include "targetver.h"

#pragma warning(disable:4514)
#pragma warning(push, 3)
#	include <Windows.h>
#	include <WindowsX.h>
#pragma warning(pop)
#include <CommCtrl.h>

#pragma warning(push)
#pragma warning(disable:4710 4711)
#	include <iostream>
#	include <map>
#	include <numeric>
#	include <set>
#pragma warning(pop)

#pragma warning(disable:4625 4626)
#pragma warning(push, 3)
#	include <boost\noncopyable.hpp>
#pragma warning(pop)

#pragma warning(disable:4710)
#pragma warning(push, 2)
#pragma warning(disable:4711)
#	include <boost\lexical_cast.hpp>
#	include <boost\format.hpp>
#pragma warning(pop)

#pragma warning(push, 3)
#pragma warning(disable:4711)
#	include <Juce\JuceHeader.h>
#pragma warning(pop)
#pragma comment(lib, "Juce.lib")