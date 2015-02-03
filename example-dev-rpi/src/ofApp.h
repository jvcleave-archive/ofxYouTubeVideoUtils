#pragma once

#include "ofMain.h"
#include "ofxOMXPlayer.h"
#include "TerminalListener.h"
#include "ofxYouTubeVideoUtils.h"


class ofApp : public ofBaseApp,  public ofxOMXPlayerListener, public KeyListener, public YouTubeDownloadEventListener{

	public:

		void setup();
		void update();
		void draw();
	
		void keyPressed(int key);

        ofxOMXPlayer omxPlayer;
        
    

        //ofxOMXPlayerListener
        void onVideoEnd(ofxOMXPlayerListenerEventData& e);
        void onVideoLoop(ofxOMXPlayerListenerEventData& e){ /*empty*/ };
    
        //YouTubeDownloadEventListener
        void onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e);
        void onYouTubeDownloadEventError(YouTubeDownloadEventData& e);
    
        vector<string> videoIDs;
        int videoCounter;
    
        //KeyListener
        TerminalListener consoleListener;
        void onCharacterReceived(KeyListenerEventData& e);

		
        ofxYouTubeVideoUtils youTubeUtils;
        //vector<YouTubeVideoURL> youTubeVideoURLs;
    
        stringstream info;
    
        void loadNextMovie();
        bool doLoadNextMovie;
    
};

