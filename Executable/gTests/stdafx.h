# pragma once

# include "targetver.h"

# include "..\..\SharedHeader.h"


# pragma warning(disable:5026 5027)	// non-copyable classes

# pragma warning(push)
#	pragma warning(disable:4626)	// non-copyable classes
#	pragma warning(disable:4668)	// #define ... 0
#	pragma warning(disable:4711)	// automatic inline expansion
#	pragma warning(disable:4820)	// padding bytes
#	pragma warning(disable:4826)	// sign-extended conversion
#		include <gtest\gtest.h>
# pragma warning(pop)

# pragma warning(disable:4571)		// catch semantics changed since Visual C++ 7.1