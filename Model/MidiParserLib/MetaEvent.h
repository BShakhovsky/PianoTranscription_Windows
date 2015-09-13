# pragma once
# include "Event.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent abstract : public Event
		{
		public:
			virtual ~MetaEvent() override = default;
		protected:
			MetaEvent() = default;
			MetaEvent& MetaInit(char metaTypeByte);
		private:
			virtual void Read_impl() override = 0	// may throw std::runtime_error
			{
				PURE_VIRTUAL;
			}

			static MetaEvent& GetInstance();
			friend class Event;
		};
	}
}

# include "MetaEvent_Text.h"
# include "MetaEvent_Skip.h"
# include "MetaEvent_EndTrack.h"
# include "MetaEvent_Tempo.h"
# include "MetaEvent_KeySign.h"