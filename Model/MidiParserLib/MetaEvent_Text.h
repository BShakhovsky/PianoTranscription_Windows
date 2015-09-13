# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_Text : public MetaEvent
		{
		public:
			virtual ~MetaEvent_Text() = default;
		protected:
			MetaEvent_Text() = default;
		private:
			virtual void Read_impl() override final;	// may throw std::runtime_error

			void ReadText(const char *eventMsg) const;	// may throw std::length_error

			static MetaEvent_Text& GetInstance();
			friend class MetaEvent;
		};
	}
}