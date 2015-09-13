# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_EndTrack : public MetaEvent
		{
		public:
			virtual ~MetaEvent_EndTrack() override = default;
		protected:
			MetaEvent_EndTrack() = default;
		private:
			virtual void Read_impl() override final;

			static MetaEvent_EndTrack& GetInstance();
			friend class MetaEvent;
		};
	}
}