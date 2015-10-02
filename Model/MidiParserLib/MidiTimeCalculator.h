# pragma once
namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct TrackEvent;
		}

		class MidiTimeCalculator : private boost::noncopyable
		{
			uint16_t
				tempoDivision_,
				UNUSED_;	// two padding bytes
			unsigned long microSeconds_;
			std::map<unsigned long, uint32_t> tempoSettings_;

			std::vector<std::vector<MidiStruct::TrackEvent>> tracks_;
			typedef std::vector<std::vector<unsigned>> Times_;
			typedef std::vector<std::vector<int16_t>> Notes_;
			Times_ milliSeconds_;
			Notes_ notes_;

			size_t currentTrack_, currentEvent_, currentTempo_;
		public:
			MidiTimeCalculator();
			~MidiTimeCalculator();

			void LoadMidiData(const char* fileName);
			void CalcDeltaTimes();

			Times_ GetTimes() const
			{
				return milliSeconds_;
			}
			Notes_ GetNotes() const
			{
				return notes_;
			}
		private:
			bool EndOfTracks();
			void PrintTime() const;

			uint32_t GetTempo() const;
			MidiStruct::TrackEvent GetEvent() const;
		};
	}
}