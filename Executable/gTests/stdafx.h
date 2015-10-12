# pragma once

# include "targetver.h"

# include "..\..\SharedHeader.h"

# include <fstream>

# pragma warning(disable:5026 5027)	// non-copyable classes

# pragma warning(push)
#	pragma warning(disable:4626)	// non-copyable classes
#	pragma warning(disable:4668)	// #define ... 0
#	pragma warning(disable:4711)	// automatic inline expansion
#	pragma warning(disable:4820)	// padding bytes
#	pragma warning(disable:4826)	// sign-extended conversion
#		include <gtest\gtest.h>
#		include <gtest\gtest-spi.h>	// for checking execution of ADD_FAILURE() macro in client's code
#	pragma warning(disable:4191)	// unsafe reinterpret_cast<>()
#	pragma warning(disable:4619)	// there is no warning 4651
#		include <boost\function.hpp>
# pragma warning(pop)

# pragma warning(disable:4571)		// catch semantics changed since Visual C++ 7.1