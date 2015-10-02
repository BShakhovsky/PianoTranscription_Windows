# include "stdafx.h"
# include "..\ViewFactory.h"
# include "Factory_Console.h"
# include "Factory_Win32.h"

using std::logic_error;
using boost::serialization::singleton;
using Interface::View::ViewFactory;

const ViewFactory& ViewFactory::GetInstance()
{
	switch (factory_)
	{
	case FACTORY::CONSOL: return singleton<Factory_Console>::get_const_instance();	break;
	case FACTORY::WIND32: return singleton<Factory_Win32>::get_const_instance();	break;
	case FACTORY::WRONG: default: throw logic_error("WRONG FACTORY");
	}
}