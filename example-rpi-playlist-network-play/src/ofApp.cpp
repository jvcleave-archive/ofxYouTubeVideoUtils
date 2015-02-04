#include "ofApp.h"


void ofApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
    ofLogVerbose(__func__) << " RECEIVED";
    doLoadNextMovie = true;
}


//--------------------------------------------------------------
void ofApp::setup()
{
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_SILENT);
    
    
    consoleListener.setup(this);
    doLoadNextMovie = false;
    
    videoCounter = 0;
    string playlistID = "PLW1HSnzWuArWiwWjkkUN7-8CjoRzBuCYi";
    
    videoIDs = youTubeUtils.getVideoIDsFromPlaylist(playlistID);
    
    
    //format 18 is good for the RPi
    vector<int> selectedFormats; //getPreferredFormats takes a vector
    selectedFormats.push_back(18);
    
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        if(videoInfo.isAvailable)
        {
            
            //youTubeUtils.downloadAllImages(videoInfo);
            vector<YouTubeVideoURL> urlsForPreferredFormats = videoInfo.getPreferredFormats(selectedFormats);
            ofLogVerbose(__func__) << "urlsForPreferredFormats SIZE: " << urlsForPreferredFormats.size();
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), urlsForPreferredFormats.begin(), urlsForPreferredFormats.end());
            
        }
    }
    
    if(youTubeVideoURLs.empty())
    {
        ofLogVerbose() << "NO MOVIES AVAILABLE";
    }else
    {
        ofLogVerbose() << "TRYING ITAG: " << youTubeVideoURLs[videoCounter].itag << "\n\n\n\n";
        
        settings.videoPath = youTubeVideoURLs[videoCounter].url;
        ofLogVerbose() << "settings.videoPath: " << settings.videoPath;
        settings.enableAudio = true;
        settings.useHDMIForAudio = true;	//default true
        settings.enableLooping = false;		//default true
        settings.enableTexture = true;		//default true
        settings.listener = this;			//this app extends ofxOMXPlayerListener so it will receive events ;
        omxPlayer.setup(settings);
    }

}

void ofApp::loadNextMovie()
{
    if(videoCounter+1<youTubeVideoURLs.size())
    {
        videoCounter++;
    }else
    {
        videoCounter = 0;
    }
    omxPlayer.loadMovie(youTubeVideoURLs[videoCounter].url);

    doLoadNextMovie = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if (doLoadNextMovie)
    {
        ofLogVerbose(__func__) << "doing reload";
        
        if(omxPlayer.isTextureEnabled())
        {
            //clear the texture if you want
            //omxPlayer.getTextureReference().clear();
        }
        //with the texture based player this must be done here - especially if the videos are different resolutions
        loadNextMovie();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    stringstream info;
    info <<  "APP FPS: " << ofGetFrameRate() << "\n";
    info << youTubeVideoURLs[videoCounter].metadata.toString() << "\n";
    
    
    ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
    ofLogVerbose(__func__) << "key: " << key;
    switch (key)
    {
        case 'n':
        {
            doLoadNextMovie = true;
            break;
        }
        case 'e':
        {
            break;
        }
        case 'x':
        {
            break;
        }
        case 'p':
        {
            ofLogVerbose() << "pause: " << !omxPlayer.isPaused();
            omxPlayer.setPaused(!omxPlayer.isPaused());
            break;
        }
        case 's':
        {
            
            break;
        }
    }

}

void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
    keyPressed((int)e.character);
}
