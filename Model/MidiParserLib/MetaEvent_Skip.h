# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_Skip : public MetaEvent
		{
		public:
			virtual ~MetaEvent_Skip() override = default;
		protected:
			MetaEvent_Skip() = default;
		private:
			virtual void Read_impl() override final;

			static MetaEvent_Skip& GetInstance();
			friend class MetaEvent;
		};
	}
}