# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_KeySign : public MetaEvent
		{
			const std::unique_ptr<class MetaEvent_KeySign_printer> pimpl_;
		public:
			virtual ~MetaEvent_KeySign() override;
		private:
			explicit MetaEvent_KeySign(char statusByte, char metaType);
			META_DECL(KeySign);
		};
	}
}