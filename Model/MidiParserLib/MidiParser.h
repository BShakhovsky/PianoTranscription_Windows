# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct ChunkType;
			struct ChunkIntro;
			class EventChunk;
		}

		class MidiParser : private boost::noncopyable
		{
			typedef std::shared_ptr<class FileParser> FileParser_;
			FileParser_ fileParser_;

			MidiParser() = delete;
		public:
			explicit MidiParser(const char *fileName);
			~MidiParser() = default;

			FileParser_ GetInputFile() const
			{
				return fileParser_;
			}

			std::shared_ptr<MidiStruct::EventChunk> ReadEvent() const;
			const MidiStruct::ChunkIntro ReadChunkIntro() const;
		private:
			const MidiStruct::ChunkType ReadChunkType() const;
		};
	}
}