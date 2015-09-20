# include "stdafx.h"
# include "..\PowerShell_CLR.h"
# include "PowerShell_CLR_pimpl.h"

using namespace std;
using System::Exception;
using namespace ManagedCLR;

# define EXPORT_DECL EXPORT __declspec(dllexport)

EXPORT_DECL PowerShell_CLR::PowerShell_CLR() :
	pimpl_(new PowerShell_CLR_pimpl)
{}

EXPORT_DECL PowerShell_CLR::~PowerShell_CLR()
{
	delete pimpl_;
}

EXPORT_DECL void __cdecl PowerShell_CLR::AppendScript(const char* script)
{
	pimpl_->AppendScript_pimpl(script);
}

EXPORT_DECL vector<string> __cdecl PowerShell_CLR::Execute()
{
	try
	{
		return pimpl_->PowerShell_Invoke();
	}
	catch (Exception^ clrException)
	{
		string errMsg;
		try
		{
			errMsg = pimpl_->StringConvert(clrException->Message);
		}
		catch (const std::exception&)
		{
			throw std::runtime_error("SYSTEM EXCEPTION IN CLASS PowerShell_CLR, METHOD GetOutput()");
		}
		throw std::runtime_error(errMsg.c_str());
	}
}