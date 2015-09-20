# pragma once
# include "..\ViewFactory.h"

namespace View
{
	class Factory_Console : public ViewFactory
	{
	public:
		virtual ~Factory_Console() override = default;
	protected:
		Factory_Console() = default;
	private:
		virtual std::unique_ptr<MidiLog> GetMidiLog_impl() const override final;
		virtual std::unique_ptr<Keyboard> GetKeyboard_impl() const override final;
	};
}