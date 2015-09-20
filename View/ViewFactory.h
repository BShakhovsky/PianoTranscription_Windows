# pragma once

namespace View
{
	class MidiLog;
	class Keyboard;

	class ViewFactory abstract : private boost::noncopyable
	{
		static const char factory_ =
# if defined _CONSOLE || _LIB
			'C';
# elif defined _WINDOWS
			'W';
# else
			"WRONG PROJECT";
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

		virtual ~ViewFactory() = 0
		{
			PURE_VIRTUAL;
		}
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

# include "Include\MidiLog.h"
# include "Include\Keyboard.h"