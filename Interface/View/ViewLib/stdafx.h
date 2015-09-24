# pragma once

# include "targetver.h"

# include "..\..\..\SharedHeader.h"

# pragma warning(push)
# pragma warning(disable:5026 5027)	// non-copyable classes
#	include <boost\serialization\singleton.hpp>
# pragma warning(pop)