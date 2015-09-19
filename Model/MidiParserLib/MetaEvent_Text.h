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
		private:
			explicit MetaEvent_Text(char statusByte, char metaType) :
				MetaEvent(statusByte, metaType)
			{}
			void ReadText(const char* eventMsg) const;	// may throw std::length_error
			META_DECL(Text);
		};
	}
}