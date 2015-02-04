# ofxYouTubeVideoUtils

WARNING: This addon does write files so any data loss is on you :)

CURRENT STATUS: Work in progress

This is an ofxAddon that allows you to download / playback videos from YouTube in an openFrameworks application


EXAMPLES:

example_playlist (Mac)
Given a playlist ID, the application gets video IDs and downloads them into respective folders in bin/data along with available screen images.
Events are sent to the ofApp when Downloads are complete

example-dev (Mac)
Currently plays a single large resolution YouTube video. Mostly for development testing

example-rpi-playlist-download (RPI)
Given a playlist ID, the application gets video IDs and downloads them into respective folders in bin/data along with available screen images. As videos are downloaded they are played back automatically
Requires ofxOMXPlayer (https://github.com/jvcleave/ofxOMXPlayer) which allows 720P videos to be played back at full rate (typically 30fps from YouTube) - Apps are able to maintain 60fps+

example-rpi-playlist-network-play (RPI)
Given a playlist ID, the application gets video IDs and plays them directly (no downloading first)
Requires ofxOMXPlayer (https://github.com/jvcleave/ofxOMXPlayer) which allows 720P videos to be played back at full rate (typically 30fps from YouTube) - Apps are able to maintain 60fps+


OBSERVED LIMITATIONS/KNOWN ISSUES
Videos must be embeddable
Content restrictions (Vevo, Age restrictions may hinder this)
High Frame rate videos will require openFrameworks 0.90 (unreleased) 

TESTED ON

Mac OS X 10.10
Xcode Version 6.1.1
openFrameworks 0.84

Raspberry Pi
openFrameworks 0.84

