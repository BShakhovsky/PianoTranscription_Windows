# pragma once
# include "Keyboard.h"

namespace View
{
	class KeyboardConsole : public Keyboard
	{
		std::unique_ptr<class KeyboardConsole_handle> consoleHandle_;
	public:
		virtual ~KeyboardConsole() override;
	protected:
		KeyboardConsole();
	private:
		virtual void Init_impl() const override final;
		virtual void Update_impl() const override final;
	};
}