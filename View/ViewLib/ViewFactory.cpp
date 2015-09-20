# include "stdafx.h"
# include "..\ViewFactory.h"
# include "Factory_Console.h"
# include "Factory_Win32.h"

using std::logic_error;
using boost::serialization::singleton;
using View::ViewFactory;

const ViewFactory& ViewFactory::GetInstance()
{
	switch (factory_)
	{
	case 'C': return singleton<Factory_Console>::get_const_instance();	break;
	case 'W': return singleton<Factory_Win32>::get_const_instance();	break;
	default: throw logic_error("WRONG FACTORY");
	}
}