# pragma once

namespace ManagedCLR
{
	private class PowerShell_CLR_pimpl : private boost::noncopyable
	{
		std::vector<std::string> commands_;
	public:
		void AppendScript_pimpl(const char* script)
		{
			commands_.push_back(script);
		}
		std::unique_ptr<std::string> StringConvert(System::String^ str) const;
		std::unique_ptr<std::vector<std::string>> PowerShell_Invoke();

		PowerShell_CLR_pimpl() = default;
		~PowerShell_CLR_pimpl() = default;
	};
}