# pragma once

namespace Unmanaged_StatLib
{
	class CodeMetrics : private boost::noncopyable
	{
		class CodeMetrics_pimpl* pimpl_;
	public:
		int SolutionLines(const char* directory, bool openPSwindow = false) const;

		CodeMetrics();
		~CodeMetrics();
	};
}