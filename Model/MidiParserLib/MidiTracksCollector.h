# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct HeaderData;
			struct TrackEvent;
		}

		class MidiTracksCollector : private boost::noncopyable
		{
			typedef std::shared_ptr<MidiStruct::HeaderData> HeaderData_;
			typedef std::vector<std::vector<MidiStruct::TrackEvent>> Tracks_;

			HeaderData_ headerData_;
			Tracks_ tracks_;

			MidiTracksCollector() = delete;
		public:
			explicit MidiTracksCollector(const char* fileName);
			~MidiTracksCollector();

			HeaderData_ GetHeaderData() const
			{
				return headerData_;
			}
			Tracks_ GetTracks() const
			{
				return tracks_;
			}

			void ReadMidiFile();
		private:
			void ReadTracks();

			const std::unique_ptr<class MidiChunksReader> midiFile_;
		};
	}
}