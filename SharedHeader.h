# pragma once

# pragma warning(disable:4710)	// not inlined functions

# include <cassert>

# pragma warning(disable:4625 4626)	// boost::noncopyable

# pragma warning(push)
#	pragma warning(disable:4619)	// there is no warning 4351
#	pragma warning(disable:5026 5027)	// lexical_cast non-copyable classes
#	pragma warning(disable:4668)	// #define ... 0
#	pragma warning(disable:4711)	// automatic inline expansion
#	pragma warning(disable:4820)	// padding bytes
#		include <iostream>
#		include <memory>
#		include <vector>
# define WIN32_LEAN_AND_MEAN
#		include <Windows.h>
# define _USE_MATH_DEFINES
#		include <boost\lexical_cast.hpp>
#		include <boost\noncopyable.hpp>
# pragma warning(pop)

# pragma warning(disable:4514)	// unreferenced inline functions are removed

# ifdef _DEBUG
#	define BORIS_ASSERT(BORIS_MSG) { std::cerr << "Assertion failed: " << BORIS_MSG << std::endl; assert(false); }
# elif defined NDEBUG
#	define BORIS_ASSERT(BORIS_MSG)
# else
#	"WRONG SOLUTION CONFIGURATION";
# endif
# define PURE_VIRTUAL BORIS_ASSERT("PURE VIRTUAL FUNCTION " __FUNCTION__ " MUST NEVER BE CALLED");