# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct HeaderChunk;
			struct TrackChunk;
		}

		class MidiChunksReader : private boost::noncopyable
		{
			typedef std::shared_ptr<MidiStruct::HeaderChunk> HeaderChunk_;
			typedef std::vector<MidiStruct::TrackChunk> TrackChunk_;
			HeaderChunk_ header_;
			TrackChunk_ tracks_;
		public:
			explicit MidiChunksReader(const char *fileName);
			~MidiChunksReader();

			HeaderChunk_ GetHeader() const
			{
				return header_;
			}
			const TrackChunk_& GetTracks() const
			{
				return tracks_;
			}

			void ReadHeaderChunk();	// may throw std::runtime_error, std::length_error and std::logic_error
			void ReadTrackChunks();	// skips alien chunks (non "MTrk"), \
									// pImpl_::ReadEvent() may throw std::runtime_error
		private:
			const std::unique_ptr<class MidiParser> pImpl_;
			MidiChunksReader() = delete;
		};
	}
}