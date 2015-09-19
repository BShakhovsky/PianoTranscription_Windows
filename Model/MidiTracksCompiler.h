# pragma once
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
			std::unique_ptr<MidiStruct::HeaderChunk> header_;
			std::vector<MidiStruct::TrackChunk> tracks_;

			static const char project_ =
# if defined _CONSOLE || defined _LIB
				'C';
# elif defined _WINDOWS
				'W';
# else
#	"WRONG PROJECT TYPE";
# endif
			class MidiChunksReader* ptrMidi_;
			unsigned long totalMicroSeconds_;
			std::vector<std::pair<unsigned long, unsigned>> tempoSettings_;
		public:
			explicit MidiTracksCompiler(const char *fileName);
			~MidiTracksCompiler();

			bool LoadNextTrack();
			void UnPause()
			{
				isPaused_ = false;
			}
			void ShowKeyboard() const;
		private:
			bool isPaused_;
			char UNUSED_[3];	// 3 padding bytes
			std::vector<MidiStruct::TrackChunk>::const_iterator itCurrentTrack_;
			std::vector<MidiStruct::TrackEvent>::const_iterator itCurrentEvent_;
			std::vector<std::pair<unsigned long, unsigned>>::const_iterator itCurrentTempo_;
			void PrintTime() const;
			MidiTracksCompiler() = delete;
		};
	}
}