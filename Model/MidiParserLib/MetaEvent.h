# pragma once
# include "Event.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent : public Event
		{
		public:
			static MetaEvent& GetInstance();
			virtual ~MetaEvent() override = default;
		protected:
			MetaEvent() = default;
		private:
			virtual void Read_impl() override final;	// may throw std::runtime_error

			void ReadText(const char *eventMsg) const;	// may throw std::length_error
			void ReadKeySignature() const;
		};
	}
}