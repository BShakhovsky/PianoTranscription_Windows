# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_Tempo : public MetaEvent
		{
		public:
			virtual ~MetaEvent_Tempo() override = default;
		protected:
			MetaEvent_Tempo() = default;
		private:
			virtual void Read_impl() override final;

			static MetaEvent_Tempo& GetInstance();
			friend class MetaEvent;
		};
	}
}