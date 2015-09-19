# pragma once

# include "targetver.h"

# include "..\..\SharedHeader.h"

# include "..\..\Executable\gTests\stdafx.h"
#	ifdef ADD_FAILURE
#		define WARNING(MESSG) { ADD_FAILURE()				<< MESSG;				system("Pause"); }
#	else
#		define WARNING(MESSG) {std::cout << "\nWARNING: "	<< MESSG << std::endl;	system("Pause"); }
#	endif