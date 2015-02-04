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
            
            vector<YouTubeVideoURL> urlsForPreferredFormats;
            
            bool useiTags = false;
            if(useiTags)
            {
                /*
                 itags are known formats see table http://en.wikipedia.org/wiki/YouTube#Quality_and_formats
                 or look at bottom of YouTubeFormat.cpp for ascii version
                 */
            
                vector<int> selected_itags;  //getURLs takes a vector of itags
                
                /*
                 format 22 (720P) and format 18 (360P) is good for the RPi, 18 is more common
                 */
                
                 selected_itags.push_back(18);
                //selected_itags.push_back(22);
                
                /*
                 Capture the urls for available formats
                 If there are multiple formats available you will get back multiple urls/videos
                 */
                
                urlsForPreferredFormats = videoInfo.getURLs(selected_itags);                
            }else
            {
                /*
                 If you just want the largest video you can ask for it via getLargestResolutionVideo()
                 However, it can be in a hard to playback form (webm) or may not contain audio, etc
                
                 In the RPI's case we want audio and video and a MP4 file so we specify that
                 */
                
                YouTubeFormat bestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_AUDIO_VIDEO, YouTubeFormat::CONTAINER_MP4);
                vector<YouTubeFormat> selectedFormats; //getURLs takes a vector of itags or YouTubeFormats
                selectedFormats.push_back(bestFormat);
                
                urlsForPreferredFormats = videoInfo.getURLs(selectedFormats);
                
            }
            
            /*
             append to our list of urls to download
             */
            
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), urlsForPreferredFormats.begin(), urlsForPreferredFormats.end());
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
    currentMetaData = downloadedYouTubeVideoURLs[videoCounter].metadata;

    doLoadNextMovie = false;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(!hasVideoPlayerStarted && !downloadedYouTubeVideoURLs.empty())
    {
        startVideoPlayer(downloadedYouTubeVideoURLs[videoCounter]);
        currentMetaData = downloadedYouTubeVideoURLs[videoCounter].metadata;
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
        info <<  "APP FPS: " << ofGetFrameRate() << "\n";
        info << currentMetaData.toString() << "\n";
        info << omxPlayer.getInfo() << "\n";
        
        
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

