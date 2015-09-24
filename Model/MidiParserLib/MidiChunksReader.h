# pragma once

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
			explicit MidiChunksReader(const char *fileName);				// for use in production
			explicit MidiChunksReader(std::unique_ptr<class IMidiParser>);	// for unit tests
			~MidiChunksReader();

			static uint32_t SMPTE_TicksPerSec(uint32_t division, bool toPrint = false);

			const MidiStruct::HeaderChunk ReadHeaderChunk() const;
				// may throw std::runtime_error, std::length_error and std::logic_error
			const MidiStruct::TrackChunk ReadTrackChunk() const;
				// skips alien chunks (non "MTrk"), pImpl_::ReadEvent() may throw std::runtime_error
		private:
			const std::unique_ptr<IMidiParser> pImpl_;
		};
	}
}