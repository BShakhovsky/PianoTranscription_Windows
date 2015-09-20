# pragma once

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

		class IMidiParser abstract : private boost::noncopyable
		{
		public:
			const MidiStruct::ChunkIntro ReadChunkIntro() const;
			const MidiStruct::HeaderData ReadHeaderData() const;

			void SkipTrackEvents(uint32_t length) const
			{
				SkipTrackEvents_impl(length);
			}
			std::vector<MidiStruct::TrackEvent> ReadTrackEvents(uint32_t length) const;

			virtual ~IMidiParser() = 0 {}
		protected:
			IMidiParser() = default;
		private:
			virtual const MidiStruct::ChunkIntro ReadChunkIntro_impl() const = 0;
			virtual const MidiStruct::HeaderData ReadHeaderData_impl() const = 0;

			virtual void SkipTrackEvents_impl(uint32_t) const = 0
			{
				PURE_VIRTUAL;
			}
			virtual std::vector<MidiStruct::TrackEvent> ReadTrackEvents_impl(uint32_t) const = 0;
				// may throw std::length_error
		};
	}
}