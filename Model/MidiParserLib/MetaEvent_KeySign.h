# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_KeySign : public MetaEvent
		{
		public:
			virtual ~MetaEvent_KeySign() override = default;
		protected:
			MetaEvent_KeySign() = default;
		private:
			virtual void Read_impl() override final;	// may throw std::runtime_error

			static MetaEvent_KeySign& GetInstance();
			friend class MetaEvent;
		};
	}
}