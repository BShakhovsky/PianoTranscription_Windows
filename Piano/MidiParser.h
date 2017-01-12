#pragma once

class MidiParser : boost::noncopyable
{
	MidiParser() = delete;
public:
	explicit MidiParser(LPCTSTR fileName);
	const std::vector<std::wstring>& GetTrackNames() const
	{
		return trackNames_;
	}
	const wchar_t* GetLog() const
	{
		return log_.c_str();
	}
private:
	void Parse();
	void ParseKeySignatureMetaEvent();
	bool ParseNoteOn(size_t trackNo);	// returns true if it is for percussion-track

	MidiFile midi_;
#ifdef _DEBUG
	const BYTE padding1_[4] = { '\0' };
#endif
	MidiMessage message_;
	unsigned lastTime_, milliSeconds_;

	std::vector<std::wstring> trackNames_;
	std::wstring log_;
	const BYTE padding2_[4] = { '\0' };
};