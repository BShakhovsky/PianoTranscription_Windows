# pragma once

# pragma warning(disable:4710)	// not inlined functions

# include <conio.h>	// _kbhit() during pause

# include <cassert>

# pragma warning(disable:4625 4626)	// boost::noncopyable

# pragma warning(push)
#	pragma warning(disable:4365)	// signed / unsigned mismatch
#	pragma warning(disable:4472)	// private / public managed enums
#	pragma warning(disable:4548)	// expression with no side-effect
#	pragma warning(disable:4571)	// catch semantics changed since Visual C++ 7.1
#	pragma warning(disable:4619)	// there is no warning 4351
#	pragma warning(disable:4623 5026 5027)	// non-copyable classes
#	pragma warning(disable:4668)	// #define ... 0
#	pragma warning(disable:4711)	// automatic inline expansion
#	pragma warning(disable:4820)	// padding bytes
#		define WIN32_LEAN_AND_MEAN
#		include <Windows.h>	// warnings 4820 and 4668

#		include <chrono>	// warning 4820
#		include <iomanip>	// std::setfill, std::setw
#		include <fstream>	// warning 4472
#		include <iostream>	// --//--
#		include <memory>	// --//-- and 4548
#		include <vector>	// --//--

#		include <boost\noncopyable.hpp>
#		include <boost\lexical_cast.hpp>
#		include <boost\format.hpp>
#		include <boost\serialization\singleton.hpp>
# pragma warning(pop)

# pragma warning(disable:4514)	// unreferenced inline functions are removed