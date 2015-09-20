# include "stdafx.h"
# include "CodeMetrics_pimpl.h"
# include "..\PowerShell\PowerShell_CLR.h"

using std::make_unique;
using boost::lexical_cast;
using ManagedCLR::PowerShell_CLR;
using Unmanaged_StatLib::CodeMetrics_pimpl;

CodeMetrics_pimpl::CodeMetrics_pimpl() :
	dll_(make_unique<PowerShell_CLR>())
{}

# pragma warning(push)
# pragma warning(disable:4711)	// automatic inline expansion
CodeMetrics_pimpl::~CodeMetrics_pimpl() {}
# pragma warning(pop)

int CodeMetrics_pimpl::SolutionLines_impl(const char* command) const
{
	dll_->AppendScript(command);
	auto result(dll_->Execute());
	assert("WRONG NUMBER OF OUTPUTS FROM POWER SHELL" && result.size() == 1);
	return lexical_cast<int>(result.at(0).c_str());
}