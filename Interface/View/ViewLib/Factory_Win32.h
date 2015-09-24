# pragma once
# include "..\ViewFactory.h"

namespace Interface
{
	namespace View
	{
		class Factory_Win32 : public ViewFactory
		{
		public:
			virtual ~Factory_Win32() override = default;
		protected:
			Factory_Win32() = default;
		private:
			virtual std::unique_ptr<MidiLog> GetMidiLog_impl() const override final;
			virtual std::unique_ptr<Keyboard> GetKeyboard_impl() const override final;
		};
	}
}