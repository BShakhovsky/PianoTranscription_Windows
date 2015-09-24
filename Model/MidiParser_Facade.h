# pragma once

namespace Model
{
	namespace MidiParser
	{
		class MidiParser_Facade : private boost::noncopyable
		{
			class MidiTimeCalculator* midiFile_;

			MidiParser_Facade() = delete;
		public:
			explicit MidiParser_Facade(const char* fileName);	// may throw std::runtime_error
			~MidiParser_Facade() = default;	// midiFile_ is deleted immediately in constructor
		private:
			void Execute(const char* fileName) const;	// must be called just one, therefore is called from constructor

			void Step1(const char* fileName) const;
			void Step2() const;
			void Step3() const;
		};
	}
}