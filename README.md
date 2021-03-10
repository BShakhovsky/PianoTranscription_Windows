# Update (2021 March)

There is also version for Android 8.0 (API level 26) or higher, for it click on the following screenshot:
[![](https://raw.GitHubUserContent.com/BShakhovsky/BShakhovsky.github.io/master/Android.png 'Android 8.0')](https://GitHub.com/BShakhovsky/PianoTranscription_Android/blob/master/README.md)

or get it on Google Play:

[![](https://play.google.com/intl/en_us/badges/static/images/badges/en_badge_web_generic.png 'Get it on Google Play')](https://play.google.com/store/apps/details?id=ru.BShakhovsky.Piano_Transcription)

# Description

3D-piano, with an option of advising on which fingers to use to press each piano key, accuracy of finger numbers is around 75%.

[![](Keyboard.png 'Screenshot')](https://GitHub.com/BShakhovsky/PianoTranscription_Windows/releases/download/2019-06-22/PianoTranscription3D.zip)

It is not possible to play on this piano using mouse nor keyboard, it only plays audio files (and it can also extract audio-stream of some video formats).  Not only MIDI-files are supported, but also MP3, WAV, etc.  Polyphonic piano transcription feature will generate MIDI from audio/video.

[![](Spectrogram.png 'Screenshot')](https://GitHub.com/BShakhovsky/PianoTranscription_Windows/releases/download/2019-06-22/PianoTranscription3D.zip)

No instrument information is extracted, and all transcribed notes get combined into one part.  So, "Fingering" option would not be applicable in this case, as it requires to have 2 separate MIDI-tracks for left and right hands.  The accuracy depends on the complexity of the song, and is obviously higher for solo piano pieces.  Currently, accuracy for piano pieces is around 75%.

# How to Install

[Download the latest zip-archive](https://GitHub.com/BShakhovsky/PianoTranscription_Windows/releases/download/2019-06-22/PianoTranscription3D.zip)

No installation is required, just open file "Piano Transcription 3D.exe".

# System Requirements

Windows 7 or later.

# Supported File Types

Not only *.mid (MIDI) or *.kar (Karaoke) files are now supported, but also most common audio formats.  The program will automatically transcribe audio into midi.

Want to transcribe some piano piece from YouTube?  You can google for websites that will convert video from YouTube to MP3.  You then can open downloaded MP3 in my program.

# About Midi/Karaoke Files

You can find plenty of them over the internet.  Those *.mid or *.kar files usually consist of several tracks, including percussion.  You probably would not want to play percussion-tracks on piano, because their "MIDI-notes" do not overload correctly on piano-notes.  So, in the most cases, you may choose any kind of tracks, but percussions (like "Drums", "Rhythms", "Hit", "Blow", "Strike", "Clash", etc.) will be disabled.

# Troubleshooting

## 1. Missing DLL's

If you encounter errors about missing DLL's like below:

[![](X3DAudio1_7.png 'Missing DLL')](https://www.microsoft.com/en-us/download/details.aspx?id=35)

[![](XAPOFX1_5.png 'Missing DLL')](https://www.microsoft.com/en-us/download/details.aspx?id=35)

or error about missing audio device like below:

[![](AudioDevice.png 'Missing audio device')](https://www.microsoft.com/en-us/download/details.aspx?id=35)

or the following strange error:

[![](msvcrt_W_Getmonths.png 'Outdated msvcrt.dll')](https://www.microsoft.com/en-us/download/details.aspx?id=35)

then try to __*reinstall the latest possible DirectX (even if you already have it):*__

[![](https://upload.wikimedia.org/wikipedia/commons/thumb/3/3f/Microsoft-DirectX-11-Logo-wordmark.svg/500px-Microsoft-DirectX-11-Logo-wordmark.svg.png 'DirectX 11')](https://www.microsoft.com/en-us/download/details.aspx?id=35)

https://www.microsoft.com/en-us/download/details.aspx?id=35

If there is still some error, then sorry, you are screwed :disappointed:

## 2. Polyphonic audio to MIDI conversion: not all media file formats supported

For video-files it may work, or it may not :disappointed:  Not all audio/video formats have been tested, e.g. for some reason it does not work correctly for MP4 videos and returns strange results.  So, if something is wrong, try to convert the file to MP3, and then feed this MP3 into my program.

# How to Use

1. Drag-and-drop any MIDI- or Karaoke-file onto the application.

2. Or alternatively, most common audio formats are now supported (such as MP3, WAV, etc.).  The program will automatically transcribe audio and save as a MIDI-file.  However, no instrument information is extracted, and all transcribed notes get combined into one part.  The accuracy depends on the complexity of the song, and is obviously higher for solo piano pieces.  Accuracy for piano pieces is around 75%.  [Also see Troubleshooting](#2-polyphonic-audio-to-midi-conversion-not-all-media-file-formats-supported)

3. Select appropriate track for left hand, and another track for right hand (not applicable for MIDI generated from audio/video, because there will be just one track).  Finger numbers for left hand will be drawn with blue color, for right hand - with red.  Accuracy of finger numbers is around 75%.  If you are not interested in finger numbers, you can skip this step.

4. Select any additional tracks in "Remaining Tracks" list, if you want.  Finger numbers for those additional tracks will not be calculated or drawn.  Percussion-tracks (like "Drums", "Rhythms", "Hit", "Blow", "Strike", "Clash", etc.) will be disabled.

5. If you want to go forward or backwards chord-by-chord, you can use scroll-bar left or right button.  Or if you want just to play the song in real time, press "Play" button.  For smoother performance, try not to move mouse over the program while playing in 3D-mode.

6. By default, each note is being played with different volume.  If you want all notes to be played with the same maximal loudness, check "Normalize volume" box.

7. Use left mouse button to rotate, middle (or press mouse wheel) to move, scroll mouse wheel to zoom, double-click on mouse wheel to fit the piano inside the window.  Right click --> choose context menu to restore the default 3D-piano position.

8. Enjoy :smile:
