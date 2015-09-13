# pragma once
# include "Event.h"

namespace Model
{
	namespace MidiParser
	{
		class MidiEvent : public Event
		{
			char runStatus_;
			char UNUSED[3];	// 3 padding bytes
		public:
			static MidiEvent& GetInstance();
			virtual ~MidiEvent() override = default;
		protected:
			MidiEvent() :
				runStatus_('\0')
			{}
		private:
			virtual void Read_impl() override final;

			void PrintMidiEvent() const;
		};
	}
}