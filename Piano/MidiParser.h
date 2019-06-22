#pragma once

class MidiParser
{
	MidiParser() = delete;
public:
	explicit MidiParser(LPCTSTR fileName);
#pragma warning(push)
#pragma warning(disable:4514) // Unreferenced inline function has been removed
	const std::vector<std::wstring>& GetTrackNames() const
	{
		return trackNames_;
	}
	const wchar_t* GetLog() const
	{
		return log_.c_str();
	}
#pragma warning(pop)
private:
	void Parse();
	void ParseKeySignatureMetaEvent();
	bool ParseNoteOn(size_t trackNo);	// returns true if it is for percussion-track

	juce::MidiFile midi_;
#ifdef _DEBUG
	const BYTE padding1_[4] = { '\0' };
#endif
	juce::MidiMessage message_;
	unsigned lastTime_, milliSeconds_;

	std::vector<std::wstring> trackNames_;
	std::wstring log_;
	const BYTE padding2_[4] = { '\0' };

	MidiParser(const MidiParser&) = delete;
	const MidiParser& operator=(const MidiParser&) = delete;
};