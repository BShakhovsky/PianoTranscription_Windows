# pragma once

# include "targetver.h"

# include "..\..\..\SharedHeader.h"

# pragma warning(push)
# pragma warning(disable:4820)	// padding bytes
#	include <mmeapi.h>	// otherwise <dsound.h> does not compile
#	include <dsound.h>
# pragma warning(pop)
# pragma comment(lib, "dsound.lib")