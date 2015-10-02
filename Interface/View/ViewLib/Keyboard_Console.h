# pragma once
# include "..\Include\Keyboard.h"

namespace Interface
{
	namespace View
	{
		class Keyboard_Console : public Keyboard
		{
			std::unique_ptr<class Keyboard_Console_handle> consoleHandle_;
		public:
			virtual ~Keyboard_Console() override;
		private:
			virtual void PressKey_impl(int16_t note) const override final;
			virtual void ReleaseKey_impl(int16_t note) const override final;
			virtual void ReleaseAllKeys_impl() const override final;
			virtual void Update_impl() const override final;

			Keyboard_Console();
			friend class Factory_Console;
		};
	}
}