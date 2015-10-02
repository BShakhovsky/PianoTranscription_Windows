# pragma once

namespace Model
{
	namespace MidiParser
	{
		class MidiParser_Facade : private boost::noncopyable
		{
			std::unique_ptr<class MidiTimeCalculator> midiFile_;

			typedef std::vector<std::vector<unsigned>> Times_;
			typedef std::vector<std::vector<int16_t>> Notes_;
			Times_ times_;
			Notes_ notes_;

			MidiParser_Facade() = delete;
		public:
			explicit MidiParser_Facade(const char* fileName);	// may throw std::runtime_error
			~MidiParser_Facade();								// midiFile_ is deleted immediately in constructor

			Times_ GetTimes() const
			{
				return times_;
			}
			Notes_ GetNotes() const
			{
				return notes_;
			}
		private:
			void Execute(const char* fileName);					// must be called just once, \
																// therefore is called from constructor
			void Step1(const char* fileName) const;
			void Step2() const;
		};
	}
}