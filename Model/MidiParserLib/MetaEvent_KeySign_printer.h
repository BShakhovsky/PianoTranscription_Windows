# pragma once

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_KeySign_printer
		{
			typedef std::shared_ptr<class IFileParser> FileParser_;
			FileParser_ inputFile_;
			int sf;
		public:
			MetaEvent_KeySign_printer();
			~MetaEvent_KeySign_printer();

			void KeySignInit(FileParser_);

			void PrintKeySignature();
		private:
			void PrintKeySF() const;
			void PrintKeyMI() const;
		};
	}
}