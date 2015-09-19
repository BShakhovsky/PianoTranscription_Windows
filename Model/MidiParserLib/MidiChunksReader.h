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
			MidiChunksReader() = delete;
		public:
			explicit MidiChunksReader(const char *fileName);
			~MidiChunksReader();

			const MidiStruct::HeaderChunk ReadHeaderChunk();
				// may throw std::runtime_error, std::length_error and std::logic_error
			const MidiStruct::TrackChunk ReadTrackChunk();
				// skips alien chunks (non "MTrk"), pImpl_::ReadEvent() may throw std::runtime_error
		private:
			const std::unique_ptr<class MidiParser> pImpl_;
		};
	}
}