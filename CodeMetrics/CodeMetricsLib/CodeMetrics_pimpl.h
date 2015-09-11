# pragma once

namespace ManagedCLR
{
	class PowerShell_CLR;
}

namespace Unmanaged_StatLib
{
	class CodeMetrics_pimpl : private boost::noncopyable
	{
		std::unique_ptr<ManagedCLR::PowerShell_CLR> dll_;
	public:
		int SolutionLines_impl(const char *command) const;
		CodeMetrics_pimpl();
		~CodeMetrics_pimpl();
	};
}