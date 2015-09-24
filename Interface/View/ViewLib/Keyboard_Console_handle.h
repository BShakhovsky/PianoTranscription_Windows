# pragma once

namespace Interface
{
	namespace View
	{
		class Keyboard_Console_handle : private boost::noncopyable
		{
			const HANDLE hStdOut_;
		public:
			Keyboard_Console_handle() :
				hStdOut_(GetStdHandle(STD_OUTPUT_HANDLE))
			{}
			~Keyboard_Console_handle()
			{
				CloseHandle(hStdOut_);
			}

			const HANDLE& Get() const
			{
				return hStdOut_;
			}
		};
	}
}