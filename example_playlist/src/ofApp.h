#pragma once

#include "ofMain.h"
#include "ofxYouTubeVideoUtils.h"

class ofApp : public ofBaseApp, public YouTubeDownloadEventListener{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
    
    void onYouTubeVideoDownloadComplete(YouTubeDownloadEventData& e);
    void onYouTubeDownloadError(YouTubeDownloadEventData& e);
    void onYouTubeAllVideosDownloadComplete();
    
    ofxYouTubeVideoUtils youTubeUtils;
    
    vector<YouTubeVideoURL> youTubeVideoURLs;
    vector<string> videoIDs;
    vector<string> videoPaths;
    vector<ofImage> images;
	
    stringstream info;
    ofVideoPlayer videoPlayer;
    bool doLoadNextMovie;
    int videoCounter;
    bool isFirstRun;
};
