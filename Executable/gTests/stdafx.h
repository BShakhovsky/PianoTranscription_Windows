# pragma once

# include "targetver.h"

# include "..\..\SharedHeader.h"

# pragma warning(disable:5026 5027)	// non-copyable classes

# pragma warning(push, 1)
#		include <gtest\gtest.h>
#		include <boost\function.hpp>
# pragma warning(pop)

# pragma warning(disable:4571)		// catch semantics changed since Visual C++ 7.1