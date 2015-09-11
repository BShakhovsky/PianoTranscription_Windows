# pragma once

namespace View
{
	class KeyboardConsole_handle : private boost::noncopyable
	{
		const HANDLE hStdOut_;
	public:
		KeyboardConsole_handle() :
			hStdOut_(GetStdHandle(STD_OUTPUT_HANDLE))
		{}
		~KeyboardConsole_handle()
		{
			CloseHandle(hStdOut_);
		}

		const HANDLE& Get() const
		{
			return hStdOut_;
		}
	};
}