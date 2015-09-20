# pragma once
namespace View
{
	class MidiLog;
}
namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct HeaderChunk;
			struct TrackEvent;
			struct TrackChunk;
		}
		class MidiTracksCompiler : private boost::noncopyable
		{
			const std::unique_ptr<View::MidiLog> logWindow_;
			const std::unique_ptr<class MidiChunksReader> ptrMidi_;

			std::unique_ptr<MidiStruct::HeaderChunk> header_;
			std::vector<MidiStruct::TrackChunk> tracks_;

			unsigned long totalMicroSeconds_;
			std::vector<std::pair<unsigned long, unsigned>> tempoSettings_;
		public:
			explicit MidiTracksCompiler(const char *fileName);
			~MidiTracksCompiler();

			void Init();
			bool LoadNextTrack();
		private:
			std::vector<MidiStruct::TrackChunk>::const_iterator itCurrentTrack_;
			std::vector<MidiStruct::TrackEvent>::const_iterator itCurrentEvent_;
			std::vector<std::pair<unsigned long, unsigned>>::const_iterator itCurrentTempo_;

			void PrintTime() const;

			MidiTracksCompiler() = delete;
		};
	}
}