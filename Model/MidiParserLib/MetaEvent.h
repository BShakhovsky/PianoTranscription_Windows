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
			explicit MetaEvent(char statusByte, char metaType);
		private:
			MetaEvent() = delete;
			static std::unique_ptr<MetaEvent> GetInstance(char statusByte);
			virtual void Read_impl() override = 0	// may throw std::runtime_error
			{
				PURE_VIRTUAL;
			}
			friend class Event;

# define META_DECL(EVENT)	MetaEvent_ ## EVENT () = delete;											\
	static std::unique_ptr<MetaEvent_ ## EVENT > GetInstance(char statusByte, char metaType);			\
	virtual void Read_impl() override final;	/* may throw std::runtime_error */						\
	friend class MetaEvent;
		};
	}
}
# include "MetaEvent_Text.h"
# include "MetaEvent_Skip.h"
# include "MetaEvent_EndTrack.h"
# include "MetaEvent_Tempo.h"
# include "MetaEvent_KeySign.h"

# define META_IMPL(EVENT) std::unique_ptr<Model::MidiParser::MetaEvent_ ## EVENT >						\
	Model::MidiParser::MetaEvent_ ## EVENT ::GetInstance(const char statusByte, const char metaType)	\
		{ return std::unique_ptr<MetaEvent_ ## EVENT >(new MetaEvent_ ## EVENT(statusByte, metaType)); }\
	void Model::MidiParser::MetaEvent_ ## EVENT ::Read_impl()