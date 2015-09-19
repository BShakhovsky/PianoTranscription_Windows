# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct ChunkType;
			struct ChunkIntro;
			struct HeaderData;
			struct TrackEvent;
		}

		class MidiParser : private boost::noncopyable
		{
			std::shared_ptr<class IFileParser> inputFile_;

			const MidiStruct::ChunkType ReadChunkType() const;
		public:
			const MidiStruct::ChunkIntro ReadChunkIntro() const;
			const MidiStruct::HeaderData ReadHeaderData() const;

			void SkipTrackEvents(uint32_t length) const;
			std::vector<MidiStruct::TrackEvent> ReadTrackEvents(uint32_t length) const;	// may throw std::length_error

			explicit MidiParser(const char* fileName);
			~MidiParser() = default;
		private:
			MidiParser() = delete;
		};
	}
}