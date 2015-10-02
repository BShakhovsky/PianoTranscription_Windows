# pragma once
namespace Interface
{
	namespace View
	{
		class MidiLog;
		class Keyboard;

		class ViewFactory abstract : private boost::noncopyable
		{
			enum class FACTORY { CONSOL, WIND32, WRONG };
			static const FACTORY factory_ =
# if defined _CONSOLE || _LIB
				FACTORY::CONSOL;
# elif defined _WINDOWS
				FACTORY::WIND32;
# else
				FACTORY::WRONG;
# endif
		public:
			static const ViewFactory& GetInstance();

			std::unique_ptr<MidiLog> GetMidiLog() const
			{
				return GetMidiLog_impl();
			}
			std::unique_ptr<Keyboard> GetKeyboard() const
			{
				return GetKeyboard_impl();
			}

			virtual ~ViewFactory() = 0 {}
		protected:
			ViewFactory() = default;
		private:
			virtual std::unique_ptr<MidiLog> GetMidiLog_impl() const = 0
			{
				PURE_VIRTUAL;
			}
			virtual std::unique_ptr<Keyboard> GetKeyboard_impl() const = 0
			{
				PURE_VIRTUAL;
			}
		};

		inline std::unique_ptr<MidiLog> GetMidiLog()
		{
			return ViewFactory::GetInstance().GetMidiLog();
		}
		inline std::unique_ptr<Keyboard> GetKeyboard()
		{
			return ViewFactory::GetInstance().GetKeyboard();
		}
	}
}
# include "Include\MidiLog.h"
# include "Include\Keyboard.h"