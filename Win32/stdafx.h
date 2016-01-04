#pragma once

#include "targetver.h"

#pragma warning(disable:4514)
#pragma warning(push, 3)
#	include <Windows.h>
#	include <WindowsX.h>
#	include <DSound.h>
#pragma warning(pop)

#pragma warning(disable:4625 4626)
#pragma warning(push, 3)
#pragma warning(disable:5026 5027)
#	include <boost\noncopyable.hpp>
#pragma warning(pop)

#pragma warning(disable:4710)
#pragma warning(push, 2)
#pragma warning(disable:4711)
#	include <boost\lexical_cast.hpp>
#	include <boost\format.hpp>
#pragma warning(pop)

#include <cassert>
#include <memory>
#include <vector>