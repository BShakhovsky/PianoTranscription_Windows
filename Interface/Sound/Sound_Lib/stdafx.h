# pragma once

# include "targetver.h"

# define _USE_MATH_DEFINES
# define _SCL_SECURE_NO_WARNINGS
# include <algorithm>
# include <cassert>
# include <functional>
# include <iostream>
# include <memory>
# include <vector>

# pragma warning(push)
# pragma warning(disable:4820)	// padding bytes
#	define WIN32_LEAN_AND_MEAN
#	include <Windows.h>
#	include <mmeapi.h>	// otherwise <dsound.h> does not compile
#	include <dsound.h>
# pragma warning(pop)
# pragma comment(lib, "dsound.lib")

# pragma warning(push)
#	include <boost\noncopyable.hpp>
# pragma warning(pop)

# define WARNING(MESSG) { std::cout << "\nWARNING: " << (MESSG) << std::endl; system("Pause"); }

# ifdef _DEBUG
#	define BORIS_ASSERT(BORIS_MSG) { std::cerr << "Assertion failed: " << BORIS_MSG << std::endl; assert(false); }
# elif defined NDEBUG
#	define BORIS_ASSERT(BORIS_MSG)
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif