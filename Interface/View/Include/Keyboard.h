# pragma once

namespace Interface
{
	namespace View
	{
		class Keyboard abstract : private boost::noncopyable
		{
		public:
			void PressKey(int16_t note) const
			{
				PressKey_impl(note);
			}
			void ReleaseKey(int16_t note) const
			{
				ReleaseKey_impl(note);
			}
			void ReleaseAllKeys() const
			{
				ReleaseAllKeys_impl();
			}
			void Update() const
			{
				Update_impl();
			}

			virtual ~Keyboard() = 0 {}
		protected:
			Keyboard() = default;
		private:
			virtual void PressKey_impl(int16_t) const = 0
			{
				PURE_VIRTUAL;
			}
			virtual void ReleaseKey_impl(int16_t) const = 0
			{
				PURE_VIRTUAL;
			}
			virtual void ReleaseAllKeys_impl() const = 0
			{
				PURE_VIRTUAL;
			}
			virtual void Update_impl() const = 0
			{
				PURE_VIRTUAL;
			}
		};
	}
}