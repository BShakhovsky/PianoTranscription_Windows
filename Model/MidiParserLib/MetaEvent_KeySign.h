# pragma once
# include "MetaEvent.h"

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_KeySign : public MetaEvent
		{
			std::unique_ptr<class MetaEvent_KeySign_pimpl> pimpl_;
		public:
			virtual ~MetaEvent_KeySign() override;
		protected:
			MetaEvent_KeySign();
		private:
			virtual void Read_impl() override final;	// may throw std::runtime_error

			static MetaEvent_KeySign& GetInstance();
			friend class MetaEvent;
		};
	}
}