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
				return 0 == memcmp(&lhs, &rhs, sizeof ChunkType);
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

			struct EventChunk
			{
				char status,				// meta or sysex or midi event
					metaType, UNUSED[2];	// two padding bytes
				uint32_t metaData;			// tempo setting
				int16_t note, velocity;		// for midi events
			};
			struct TrackEvent
			{
				static const int microSec, milliSec, minute;
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