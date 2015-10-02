# pragma once

# include "targetver.h"

# include "..\..\SharedHeader.h"

#	include <chrono>
#	include <iomanip>	// std::setfill, std::setw
#	include <fstream>
#	include <map>

# include "..\..\Executable\gTests\stdafx.h"
# ifdef ADD_FAILURE
#	define WARNING(MESSG) { ADD_FAILURE()				<< MESSG;				system("Pause"); }
# else
#	define WARNING(MESSG) { std::cout << "\nWARNING: "	<< MESSG << std::endl;	system("Pause"); }
# endif