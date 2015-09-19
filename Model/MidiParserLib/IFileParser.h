# pragma once
namespace Model
{
	namespace MidiParser
	{
		class IFileParser abstract : private boost::noncopyable
		{
		public:
			virtual ~IFileParser() = default;

			int GetBytesRemained() const
			{
				return GetBytesRemained_impl();
			}
			void SetBytesRemained(int value) const
			{
				SetBytesRemained_impl(value);
			}

			int PeekByte()
			{
				return PeekByte_impl();
			}
			char ReadByte()
			{
				return ReadByte_impl();
			}
			void ReadData(char* data, std::streamsize count)
			{
				ReadData_impl(data, count);
			}
			void SkipData(std::streamoff offset)
			{
				SkipData_impl(offset);
			}

			unsigned ReadInverse(unsigned nBytes, bool toCheck = true)
			{
				return ReadInverse_impl(nBytes, toCheck);
			}
			unsigned ReadVarLenFormat()
			{
				return ReadVarLenFormat_impl();
			}
		protected:
			IFileParser() = default;
		private:
			virtual int GetBytesRemained_impl() const = 0
			{
				PURE_VIRTUAL;
			}
			virtual void SetBytesRemained_impl(int) const = 0
			{
				PURE_VIRTUAL;
			}

			virtual int PeekByte_impl() = 0
			{
				PURE_VIRTUAL;
			}
			virtual char ReadByte_impl() = 0
			{
				PURE_VIRTUAL;
			}
			virtual void ReadData_impl(char*, std::streamsize) = 0
			{
				PURE_VIRTUAL;
			}
			virtual void SkipData_impl(std::streamoff) = 0
			{
				PURE_VIRTUAL;
			}

			virtual unsigned ReadInverse_impl(unsigned, bool) = 0;
			virtual unsigned ReadVarLenFormat_impl() = 0;
		};
	}
}