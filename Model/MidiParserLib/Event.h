# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			class EventChunk;
		}

		class Event abstract : private boost::noncopyable
		{
			typedef std::shared_ptr<class IFileParser> FileParser_;
			typedef std::shared_ptr<MidiStruct::EventChunk> EventChunk_;

			static FileParser_ fileParser_;
			EventChunk_ eventChunk_;
		public:
			static Event& GetInstance(FileParser_ = nullptr);
			virtual ~Event();
			EventChunk_ Read();
		protected:
			Event();
			Event& Init(char statusByte);
			void SkipEvent() const;

			FileParser_ GetInputFile() const
			{
				return fileParser_;
			}
			EventChunk_ GetChunk() const
			{
				return eventChunk_;
			}
		private:
			virtual void Read_impl() = 0
			{
				PURE_VIRTUAL;
			}
		};
	}
}
# include "MetaEvent.h"
# include "SystemEvent.h"
# include "MidiEvent.h"