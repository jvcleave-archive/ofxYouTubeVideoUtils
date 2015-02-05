#include "ofApp.h"


void ofApp::onYouTubeVideoDownloadComplete(YouTubeDownloadEventData& e)
{
    downloadInfo    <<  "COMPLETED\n";
    downloadInfo    <<"\n" <<   "url: "         << e.downloadRequest.url;
    downloadInfo    <<"\n" <<   "videoID: "     << e.downloadRequest.videoID;
    downloadInfo    <<"\n" <<   "filePath: "    << e.downloadRequest.filePath;
    
    
    downloadedYouTubeVideoURLs.push_back(e.downloadRequest.videoURL);
}

void ofApp::onYouTubeDownloadError(YouTubeDownloadEventData& e)
{
    
    downloadInfo    <<  "FAILED\n";
    downloadInfo    <<  "\n"    <<  "url: "         << e.downloadRequest.url;
    downloadInfo    <<  "\n"    <<  "videoID: "     << e.downloadRequest.videoID;
    downloadInfo    <<  "\n"    <<  "filePath: "    << e.downloadRequest.filePath;
}

void ofApp::onYouTubeAllVideosDownloadComplete()
{
    
    ofLogVerbose(__func__) << " RECEIVED";
}

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
    
    youTubeUtils.addListener(this);
    consoleListener.setup(this);
    
    hasVideoPlayerStarted = false;
    doLoadNextMovie = false;
    videoCounter = 0;
    
    string playlistID = "PLW1HSnzWuArWiwWjkkUN7-8CjoRzBuCYi"; 
    
    videoIDs = youTubeUtils.getVideoIDsFromPlaylist(playlistID);
    
    
    
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        //ask YouTube for metadata and urls
        YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        
        //ask YouTube for metadata and urls
        if(videoInfo.isAvailable)
        {
            /*
                The YouTubeVideo object "videoInfo" contains urls and what formats are available
                We can ask for certain formats a few different ways
             */
            
            
            //format 22 is the best for the RPi (Audio/Video, 720P, MP4) but may not be available
            
            YouTubeVideoURL videoURL;
            if(videoInfo.isFormatAvailable(22))
            {
                videoURL = videoInfo.getURL(22);
            }else
            {
                //22 not available - look for 18 (Audio/Video, 360P, MP4)
                if(videoInfo.isFormatAvailable(18))
                {
                    videoURL = videoInfo.getURL(18);
                }
            }
            
            //In case 18 or 22 are not available
            if (videoURL.url.empty()) 
            {
                YouTubeFormat lastDitchFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_AUDIO_VIDEO,
                                                                                    YouTubeFormat::CONTAINER_MP4);
                videoURL = videoInfo.getURL(lastDitchFormat);
                
            }
            if(!videoURL.url.empty())
            {
                youTubeVideoURLs.push_back(videoURL);
            }
            
            
#if 0
            //if you want to download multiple formats of the same video you can do this
            
            //getURLs can take a vector of YouTubeFormat or ints (itags)
            vector<YouTubeVideoURL> urlsForPreferredFormats;
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
        //download options
        ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS  = true;     //default: false
        ofxYouTubeVideoUtils::USE_PRETTY_NAMES              = true;     //default: false, if false, will use videoID+_+itag. if true, video title
        bool doAsync                                        = true;     //default: false
        bool doOverwriteExisting                            = false;    //default: false
        
        /*
         Start downloading, when a download is complete
         onYouTubeVideoDownloadComplete will be called pushing the filepath
         into downloadedYouTubeVideoURLs which is checked in update()
         */
        for(size_t i=0; i<youTubeVideoURLs.size(); i++)
        {
            youTubeUtils.downloadVideo(youTubeVideoURLs[i], doAsync, doOverwriteExisting);
        }
        
    }

}

void ofApp::startVideoPlayer(YouTubeVideoURL currentYouTubeVideoURL)
{
    ofxOMXPlayerSettings settings;
    settings.videoPath = currentYouTubeVideoURL.localFilePath;
    settings.enableAudio = true;
    settings.useHDMIForAudio = true;    //default true
    settings.enableLooping = false;        //default true
    settings.enableTexture = true;        //default true
    settings.listener = this;            //this app extends ofxOMXPlayerListener so it will receive events ;
    hasVideoPlayerStarted = omxPlayer.setup(settings);
}

void ofApp::loadNextMovie()
{
    if(!hasVideoPlayerStarted) return;
    if(videoCounter+1<downloadedYouTubeVideoURLs.size())
    {
        videoCounter++;
    }else
    {
        videoCounter = 0;
    }
    
    omxPlayer.loadMovie(downloadedYouTubeVideoURLs[videoCounter].localFilePath);
    currentVideoURL = downloadedYouTubeVideoURLs[videoCounter];

    doLoadNextMovie = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(!hasVideoPlayerStarted && !downloadedYouTubeVideoURLs.empty())
    {
        startVideoPlayer(downloadedYouTubeVideoURLs[videoCounter]);
        currentVideoURL = downloadedYouTubeVideoURLs[videoCounter];
    }
    if (doLoadNextMovie)
    {
        loadNextMovie();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if (hasVideoPlayerStarted)
    {
        omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
        
        stringstream info;
        
        info << omxPlayer.getInfo() << "\n";
        info << currentVideoURL.metadata.toString() << "\n";
        info << currentVideoURL.format.toString() << "\n";
        
        ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
    }
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

