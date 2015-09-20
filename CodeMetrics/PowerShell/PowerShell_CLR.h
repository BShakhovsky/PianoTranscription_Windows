# pragma once

namespace ManagedCLR
{
# ifdef __cplusplus
#	define EXPORT extern "C"
# else
#	define EXPORT
# endif
	EXPORT class __declspec(dllexport) PowerShell_CLR : private boost::noncopyable
	{
		class PowerShell_CLR_pimpl* pimpl_;
	public:
		void __cdecl AppendScript(const char* script);
		std::vector<std::string> __cdecl Execute();

		PowerShell_CLR();
		~PowerShell_CLR();
	};
}