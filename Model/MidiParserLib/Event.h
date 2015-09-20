# pragma once
namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct EventChunk;
		}
		class Event abstract : private boost::noncopyable
		{
			typedef std::shared_ptr<class IFileParser> FileParser_;
			typedef std::shared_ptr<MidiStruct::EventChunk> EventChunk_;
			static FileParser_ fileParser_;
			EventChunk_ eventChunk_;
		public:
			static std::unique_ptr<Event> GetInstance(FileParser_ = nullptr);
			virtual ~Event() = 0;
			EventChunk_ Read();
		protected:
			explicit Event(char statusByte);

			static FileParser_ GetInputFile()
			{
				return fileParser_;
			}
			EventChunk_ GetChunk() const
			{
				return eventChunk_;
			}
			
			void SkipEvent() const;
		private:
			Event() = delete;
			virtual void Read_impl() = 0
			{
				PURE_VIRTUAL;
			}

# define EVENT_DECL(NAME) namespace Model { namespace MidiParser { class NAME ## Event : public Event {	\
	public:		virtual ~ ## NAME ## Event () override = default;										\
	private:	NAME ## Event () = delete;																\
				explicit NAME ## Event (char statusByte) : Event(statusByte) {}							\
				static std::unique_ptr<NAME ## Event> GetInstance(char statusByte)						\
					{ return std::unique_ptr<NAME ## Event>(new NAME ## Event (statusByte)); }			\
				virtual void Read_impl() override final;												\
				friend class Event;
		};
	}
}

# include "MetaEvent.h"
# include "SystemEvent.h"
# include "MidiEvent.h"