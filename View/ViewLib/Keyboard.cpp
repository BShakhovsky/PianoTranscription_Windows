# include "stdafx.h"
# include "..\Keyboard.h"
# include "..\KeyboardConsole.h"
# include "..\KeyboardWin32.h"

using std::logic_error;
using boost::serialization::singleton;
using namespace View;

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
const Keyboard& Keyboard::GetInstance()
{
	switch (Keyboard::factory_)
	{
	case 'C': return singleton<KeyboardConsole>::get_const_instance();	break;
	case 'W': return singleton<KeyboardWin32>::get_const_instance();	break;
	default: throw logic_error("WRONG FACTORY");
	}
}
# pragma warning(pop)