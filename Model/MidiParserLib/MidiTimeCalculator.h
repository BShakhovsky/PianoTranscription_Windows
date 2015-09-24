# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct TrackEvent;
			struct TrackChunk;
		}

		class MidiTimeCalculator : private boost::noncopyable
		{
			uint16_t
				tempoDivision_,
				UNUSED_;	// two padding bytes

			typedef std::vector<MidiStruct::TrackEvent> Events_;
			typedef std::vector<Events_> Tracks_;
			Tracks_ tracks_;

			unsigned long totalMicroSeconds_;
			typedef std::vector<std::pair<unsigned long, unsigned>> Tempos_;
			Tempos_ tempoSettings_;
		public:
			MidiTimeCalculator();
			~MidiTimeCalculator();

			void LoadMidiData(const char* fileName);
			void CollectTempos();
			void CalcDeltaTimes();
		private:
			Tracks_::const_iterator itCurrentTrack_;
			Events_::const_iterator itCurrentEvent_;
			Tempos_::const_iterator itCurrentTempo_;

			void PrintTime() const;
			bool EndOfTracks();
		};
	}
}