# pragma once

namespace Model
{
	namespace MidiParser
	{
		class MetaEvent_KeySign_pimpl
		{
			typedef std::shared_ptr<class IFileParser> FileParser_;
			FileParser_ inputFile_;
			int sf;
		public:
			MetaEvent_KeySign_pimpl();
			~MetaEvent_KeySign_pimpl();

			void KeySignInit(FileParser_);

			void PrintKeySignature();
		private:
			void PrintKeySF() const;
			void PrintKeyMI() const;
		};
	}
}