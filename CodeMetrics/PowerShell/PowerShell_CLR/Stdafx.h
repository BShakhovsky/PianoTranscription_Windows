# pragma once

# pragma warning(disable:4710)		// not inlined functions

# pragma warning(push)
# pragma warning(disable:4472)	// PowerShell_CLR private / public managed enums
#	include "..\..\..\SharedHeader.h"
# pragma warning(pop)

# pragma warning(push)
# pragma warning(disable:4365)	// signed / unsigned mismatch
#	include <vcclr.h>
# pragma warning(pop)

# pragma warning(disable:4339)		// undefined type
# pragma warning(disable:4514)		// unreferenced inline functions are removed
# pragma warning(disable:4625 4626)	// boost::noncopyable