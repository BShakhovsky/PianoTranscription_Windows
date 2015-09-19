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
		private:
			explicit MetaEvent_EndTrack(char statusByte, char metaType) :
				MetaEvent(statusByte, metaType)
			{}
			META_DECL(EndTrack);
		};
	}
}