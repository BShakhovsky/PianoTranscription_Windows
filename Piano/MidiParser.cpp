#include "stdafx.h"
#include "MidiParser.h"
#include "MidiError.h"
#include "Piano.h"

using namespace std;
using namespace boost;

MidiParser::MidiParser(LPCTSTR fileName)
{
	FileInputStream inputFile(
		File::getCurrentWorkingDirectory().getChildFile(String(fileName)).getFullPathName());
	if (inputFile.failedToOpen())
		throw MidiError((wstring(TEXT("Could not open file:\r\n")) + fileName).c_str());
	if (!midi_.readFrom(inputFile))
		throw MidiError((wstring(TEXT("Not a MIDI-file:\r\n")) + fileName).c_str());

	Parse();
}

void MidiParser::ParseKeySignatureMetaEvent()
{
	log_.append(TEXT("Key signature: "));

	const auto nSharpsOrFlats(message_.getKeySignatureNumberOfSharpsOrFlats());
	log_.append(nSharpsOrFlats ? lexical_cast<wstring>(abs(nSharpsOrFlats))
		+ (nSharpsOrFlats > 0 ? TEXT(" sharps (") : TEXT(" flats ("))
		: TEXT("probably, natural "));

	if (nSharpsOrFlats >= 1)
	{
		log_.append(TEXT("F"));
		if (nSharpsOrFlats >= 2)
		{
			log_.append(TEXT(", C"));
			if (nSharpsOrFlats >= 3)
			{
				log_.append(TEXT(", G"));
				if (nSharpsOrFlats >= 4)
				{
					log_.append(TEXT(", D"));
					if (nSharpsOrFlats >= 5)
					{
						log_.append(TEXT(", A"));
						if (nSharpsOrFlats >= 6)
						{
							log_.append(TEXT(", E"));
							if (nSharpsOrFlats >= 7)
							{
								assert("Wrong key signature" && nSharpsOrFlats == 7);
								log_.append(TEXT(", H"));
							}
						}
					}
				}
			}
		}
	}
	else if (nSharpsOrFlats <= -1)
	{
		log_.append(TEXT("H"));
		if (nSharpsOrFlats <= -2)
		{
			log_.append(TEXT(", E"));
			if (nSharpsOrFlats <= -3)
			{
				log_.append(TEXT(", A"));
				if (nSharpsOrFlats <= -4)
				{
					log_.append(TEXT(", D"));
					if (nSharpsOrFlats <= -5)
					{
						log_.append(TEXT(", G"));
						if (nSharpsOrFlats <= -6)
						{
							log_.append(TEXT(", C"));
							if (nSharpsOrFlats <= -7)
							{
								assert("Wrong key signature" && nSharpsOrFlats == -7);
								log_.append(TEXT(", F"));
							}
						}
					}
				}
			}
		}
	}

	switch (nSharpsOrFlats)
	{
	case -7: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> Cb-Major tone\r\n") : TEXT(") -> Ab-Minor tone\r\n"));	break;
	case -6: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> Gb-Major tone\r\n") : TEXT(") -> Eb-Minor tone\r\n"));	break;
	case -5: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> Db-Major tone\r\n") : TEXT(") -> Bb-Minor tone\r\n"));	break;
	case -4: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> Ab-Major tone\r\n") : TEXT(") -> F-Minor tone\r\n"));	break;
	case -3: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> Eb-Major tone\r\n") : TEXT(") -> C-Minor tone\r\n"));	break;
	case -2: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> Bb-Major tone\r\n") : TEXT(") -> G-Minor tone\r\n"));	break;
	case -1: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> F-Major tone\r\n") : TEXT(") -> D-Minor tone\r\n"));	break;

	case 0: log_.append(message_.isKeySignatureMajorKey()
		? TEXT("C-Major tone\r\n") : TEXT("A-Minor tone\r\n"));				break;

	case 1: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> G-Major tone\r\n") : TEXT(") -> E-Minor tone\r\n"));	break;
	case 2: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> D-Major tone\r\n") : TEXT(") -> H-Minor tone\r\n"));	break;
	case 3: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> A-Major tone\r\n") : TEXT(") -> F#-Minor tone\r\n"));	break;
	case 4: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> E-Major tone\r\n") : TEXT(") -> C#-Minor tone\r\n"));	break;
	case 5: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> H-Major tone\r\n") : TEXT(") -> G#-Minor tone\r\n"));	break;
	case 6: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> F#-Major tone\r\n") : TEXT(") -> D#-Minor tone\r\n"));	break;
	case 7: log_.append(message_.isKeySignatureMajorKey()
		? TEXT(") -> C#-Major tone\r\n") : TEXT(") -> A#-Minor tone\r\n"));	break;

	default: assert(!"Wrong key signature");
	}
}

bool MidiParser::ParseNoteOn(const size_t trackNo)
{
	// if it is midi-file with all instruments in a single track, do not add percussions:
	if (midi_.getNumTracks() == 1 && message_.getChannel() == 10) return false;

	// if there are several tracks, we add percussions, but just once
	// (then check again and remove them later)
	if (milliSeconds_ - lastTime_ < Piano::timerTick && !Piano::notes.at(trackNo).empty())
	{
		Piano::notes.at(trackNo).back().insert(make_pair(
			static_cast<int16_t>(message_.getNoteNumber()), message_.getFloatVelocity()));
		Piano::milliSeconds.at(trackNo).back().second = milliSeconds_;
	}
	else
	{
		Piano::notes.at(trackNo).push_back({ make_pair(
			static_cast<int16_t>(message_.getNoteNumber()), message_.getFloatVelocity()) });
		Piano::milliSeconds.at(trackNo).emplace_back(make_pair(milliSeconds_, milliSeconds_));
	}
	lastTime_ = milliSeconds_;

	// do not check earlier, otherwise empty data-tracks will be also shown in track-list
	if (message_.getChannel() == 10 && midi_.getNumTracks() > 1)
	{
		Piano::percussions.at(trackNo) = true;
		return true;
	}
	return false;
}

void MidiParser::Parse()
{
	midi_.convertTimestampTicksToSeconds();

	const auto nTracks(static_cast<size_t>(midi_.getNumTracks()));
	Piano::notes.assign(nTracks, vector<map<int16_t, float>>());
	Piano::milliSeconds.assign(nTracks, vector<pair<unsigned, unsigned>>());
	Piano::percussions.assign(nTracks, false);
	trackNames_.assign(nTracks, TEXT(""));

	for (size_t i(0); i < nTracks; ++i)
	{
		lastTime_ = 0;
		const auto track(midi_.getTrack(static_cast<int>(i)));

		for (auto j(0); j < track->getNumEvents(); ++j)
		{
			message_ = track->getEventPointer(j)->message;
			milliSeconds_ = static_cast<unsigned>(message_.getTimeStamp() * 1'000);

			if (message_.isTextMetaEvent())
				if (message_.isTrackNameEvent()) trackNames_.at(i)
					= message_.getTextFromTextMetaEvent().toWideCharPointer();
				else log_.append(wstring(TEXT("\t"))
					+ message_.getTextFromTextMetaEvent().toWideCharPointer() + TEXT("\r\n"));
			else if (message_.isTempoMetaEvent())
				log_.append((wformat{ TEXT("Time %02d:%02d:%02d Tempo %d BPM\r\n") }
					% (milliSeconds_ / 1'000 / 60) % (milliSeconds_ / 1'000 % 60) % (milliSeconds_ % 1'000 / 10)
					% static_cast<int>(60 / message_.getTempoSecondsPerQuarterNote() + 0.5)).str());
			else if (message_.isKeySignatureMetaEvent()) ParseKeySignatureMetaEvent();
			else if (message_.isNoteOn()) if (ParseNoteOn(i)) continue;
				// do not check earlier, otherwise empty data-tracks will be also shown in track-list
		}
	}
}
