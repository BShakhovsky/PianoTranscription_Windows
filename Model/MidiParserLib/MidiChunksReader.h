# pragma once

# ifdef FRIEND_TEST
	class Test_MidiChunksReader;
# endif
namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct ChunkIntro;
			struct HeaderData;
			struct HeaderChunk;
			struct TrackChunk;
		}

		class MidiChunksReader : private boost::noncopyable
		{
			MidiChunksReader() = delete;
		public:
			explicit MidiChunksReader(const char *fileName);			// for use in production
			~MidiChunksReader();

			const MidiStruct::HeaderChunk ReadHeaderChunk() const;
				// may throw std::runtime_error, std::length_error and std::logic_error
			const MidiStruct::TrackChunk ReadTrackChunk() const;
				// skips alien chunks (non "MTrk"), pImpl_::ReadEvent() may throw std::runtime_error
		private:
			void CheckHeaderIntro(MidiStruct::ChunkIntro) const;
			void PrintHeaderData(MidiStruct::HeaderData) const;

			const std::unique_ptr<class IMidiParser> pImpl_;
# ifdef FRIEND_TEST
			explicit MidiChunksReader(std::unique_ptr<IMidiParser>);	// for unit tests
			friend Test_MidiChunksReader;
# endif
		};
	}
}