# pragma once
# include "Event.h"

namespace Model
{
	namespace MidiParser
	{
		class SystemEvent : public Event
		{
		public:
			static SystemEvent& GetInstance();
			virtual ~SystemEvent() override = default;
		protected:
			SystemEvent() = default;
		private:
			virtual void Read_impl() override final;
		};
	}
}