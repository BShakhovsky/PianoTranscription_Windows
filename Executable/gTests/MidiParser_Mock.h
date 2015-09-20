# pragma once
# include "..\..\Model\MidiParserLib\IMidiParser.h"

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct ChunkIntro;
			struct HeaderData;
			struct TrackEvent;
		}
	}
}

namespace gTest
{
	class MidiParser_Mock : public Model::MidiParser::IMidiParser
	{
		static int counter_;
	public:
		static void IncreaseCounter()
		{
			++counter_;
		}

		MidiParser_Mock() = default;
		virtual ~MidiParser_Mock() override final = default;
	private:
		virtual const Model::MidiParser::MidiStruct::ChunkIntro ReadChunkIntro_impl() const override final;
		virtual const Model::MidiParser::MidiStruct::HeaderData ReadHeaderData_impl() const override final;

		virtual void SkipTrackEvents_impl(uint32_t length) const override final
		{
			ADD_FAILURE() << "Corrupted MIDI Track Header, " << length << " bytes skipped";
		}
		virtual std::vector<Model::MidiParser::MidiStruct::TrackEvent> ReadTrackEvents_impl(uint32_t) const override final;
	};
}