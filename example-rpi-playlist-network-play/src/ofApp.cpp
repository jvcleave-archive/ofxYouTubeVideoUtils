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
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        if(videoInfo.isAvailable)
        {
            //youTubeUtils.downloadAllImages(videoInfo);
            
           //format 22 is the best for the RPi (Audio/Video, 720P, MP4) but may not be available
            YouTubeVideoURL videoURL;
            if(videoInfo.isFormatAvailable(22))
            {
                videoURL = videoInfo.getURL(22);
                youTubeVideoURLs.push_back(videoURL);
            }else
            {
                //22 not available - look for 18 (Audio/Video, 360P, MP4)
                if(videoInfo.isFormatAvailable(18))
                {
                    videoURL = videoInfo.getURL(18);
                    youTubeVideoURLs.push_back(videoURL);
                }
            
            }
            
            //In case 18 or 22 are not available
            if (videoURL.url.empty()) 
            {
                
                YouTubeFormat lastDitchFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_AUDIO_VIDEO, YouTubeFormat::CONTAINER_MP4);
                videoURL = videoInfo.getURL(lastDitchFormat);
                
            }
            if(!videoURL.url.empty())
            {
                youTubeVideoURLs.push_back(videoURL);
            }
            
            //if you want multiple versions for the same video you can do this
#if 0
            //getURLs can take a vector of YouTubeFormat or ints (itags)
            vector<int> selectedFormats; 
            selectedFormats.push_back(18);
            selectedFormats.push_back(22);
            
            vector<YouTubeVideoURL> urlsForPreferredFormats = videoInfo.getURLs(selectedFormats);
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), urlsForPreferredFormats.begin(), urlsForPreferredFormats.end());
#endif
            
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
    info << omxPlayer.getInfo() << "\n";
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
