#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "TerminalListener.h"
#include "ofxYouTubeVideoUtils.h"


class ofApp : public ofBaseApp, public YouTubeDownloadEventListener{

	public:

		void setup();
		void update();
		void draw();
	
		void keyPressed(int key);

        //YouTubeDownloadEventListener
        void onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e);
        void onYouTubeDownloadEventError(YouTubeDownloadEventData& e);
		
		vector<string> videoIDs;
		int videoCounter;

        ofxYouTubeVideoUtils youTubeUtils;
        vector<YouTubeVideoURL> youTubeVideoURLs;
    
        stringstream info;
    
};

