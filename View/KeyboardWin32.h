# pragma once
# include "Keyboard.h"

namespace View
{
	class KeyboardWin32 : public Keyboard
	{
	public:
		virtual ~KeyboardWin32() override = default;
	protected:
		KeyboardWin32() = default;
	private:
		virtual void Init_impl() const override final
		{
			assert(!"<KeyboardWin32.h> HAS NOT BEEN IMPLEMENTED YET");
		}
		virtual void Update_impl() const override final
		{
			assert(!"<KeyboardWin32.h> HAS NOT BEEN IMPLEMENTED YET");
		}
	};
}