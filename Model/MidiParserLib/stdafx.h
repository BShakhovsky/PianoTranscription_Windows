# pragma once

# include "targetver.h"

# include "..\..\SharedHeader.h"

# include "..\..\Executable\gTests\stdafx.h"
#	ifndef FAIL
#		define FAIL()			std::cout << "\nWARNING: "
#	endif
#	ifndef ADD_FAILURE
#		define ADD_FAILURE()	std::cout << "\nWARNING: "
#		define SUCCESS			std::cout << '\n'
#	else
#		define SUCCESS			std::cout
#	endif