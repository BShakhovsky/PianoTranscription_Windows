# pragma once

namespace Model
{
	namespace MidiParser
	{
		namespace MidiStruct
		{
			struct Bytes
			{
				static const int varLengthSize, byteSize;
			};

			struct ChunkType
			{
				char type[sizeof int32_t];
			};
			inline bool operator == (const ChunkType& lhs, const ChunkType& rhs)
			{
				for (auto i(0); i < sizeof lhs.type / sizeof *lhs.type; ++i)
					if (lhs.type[i] != rhs.type[i]) return false;
				return true;
			}
			inline bool operator != (const ChunkType& lhs, const ChunkType& rhs)
			{
				return !(lhs == rhs);
			}
			struct ChunkIntro
			{
				static const ChunkType HEADER, TRACK;
				ChunkType type;
				uint32_t length;	// 0 x 00 00 00 06 for header
			};

			struct HeaderData
			{
				uint16_t
					format,		// 0 or 1 or 2
					tracks,		// 1 for zero format
					division,	// pulses per quater note if positive, SMPTE otherwise
					UNUSED_twoPaddingBytes;
			};
			struct HeaderChunk
			{
				ChunkIntro intro;
				HeaderData data;
			};

			class EventChunk
			{
			public:
				EventChunk()
				{
					Clear();
				}
				EventChunk(const EventChunk&) = default;
				EventChunk& operator = (const EventChunk&) = default;

				char status,			// meta or sysex or midi event
					metaType;
				int16_t UNUSED_twoPaddingBytes;
				uint32_t length;		// for meta and sysex events, and at most four bytes long
				uint32_t metaData;		// tempo setting
				std::string metaText;	// track name, instrument, etc.

				int16_t note,			// midi events: 0x90 = note on, 0x80 = note off
					velocity;

				void Clear()
				{
					status = '\0';
					metaType = '\0';
					length = NULL;
					metaData = NULL;
					metaText.clear();
					note = NULL;
					velocity = NULL;
				}
			};
			struct TrackEvent
			{
				static const long long
					microSec,
					minute;
				uint32_t deltaTime;	// delta-time is at most four bytes long
				EventChunk eventChunk;
			};
			struct TrackChunk
			{
				ChunkIntro intro;
				std::vector<TrackEvent> trackEvent;
			};
		}
	}
}