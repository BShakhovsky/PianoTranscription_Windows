# pragma once

namespace View
{
	class Keyboard abstract : private boost::noncopyable
	{
	public:
		void Update() const
		{
			Update_impl();
		}

		virtual ~Keyboard() = 0
		{
			PURE_VIRTUAL;
		}
	protected:
		Keyboard() = default;
	private:
		virtual void Update_impl() const = 0
		{
			PURE_VIRTUAL;
		}
	};
}