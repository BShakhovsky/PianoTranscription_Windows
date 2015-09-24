# pragma once
# include "IMidiParser.h"

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct ChunkType;
		}

		class MidiParser : public IMidiParser
		{
			std::shared_ptr<class IFileParser> inputFile_;

			MidiParser() = delete;
		public:
			explicit MidiParser(const char* fileName);
			virtual ~MidiParser() override final;
		private:
			virtual const MidiStruct::ChunkIntro ReadChunkIntro_impl() const override final;
			virtual const MidiStruct::HeaderData ReadHeaderData_impl() const override final;

			virtual void SkipTrackEvents_impl(uint32_t length) const;
			virtual std::vector<MidiStruct::TrackEvent> ReadTrackEvents_impl(uint32_t length) const override final;
				// may throw std::length_error

			const MidiStruct::ChunkType ReadChunkType() const;
		};
	}
}