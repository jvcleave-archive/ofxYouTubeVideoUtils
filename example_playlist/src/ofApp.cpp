#include "ofApp.h"


void ofApp::onYouTubeVideoDownloadComplete(YouTubeDownloadEventData& e)
{
    info << "\n" << "COMPLETED ";
    info << e.downloadRequest.videoURL.metadata.toString();
    info <<"\n" <<	"filePath: "	<< e.downloadRequest.filePath;
    videoPaths.push_back(e.downloadRequest.filePath);
    
}

void ofApp::onYouTubeDownloadError(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << e.message;
    
    info << "\n" << "FAILED ";
    info << e.downloadRequest.videoURL.metadata.toString();
    info << "\n" << "url: "         << e.downloadRequest.url;
    info << "\n" <<	"filePath: "    << e.downloadRequest.filePath;
}


void ofApp::onYouTubeAllVideosDownloadComplete()
{
    ofLogVerbose(__FUNCTION__) << "!";
}

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_SILENT);
    
    //Setup callbacks to download events
    youTubeUtils.addListener(this);
    
    isFirstRun = true;
    doLoadNextMovie = false;
    videoCounter = 0;
    
    //use playlist id from https://www.youtube.com/playlist?list=PLW1HSnzWuArWiwWjkkUN7-8CjoRzBuCYi
    string playlistID = "PLW1HSnzWuArWiwWjkkUN7-8CjoRzBuCYi";
    
    //grab videos ids from playlist
    videoIDs = youTubeUtils.getVideoIDsFromPlaylist(playlistID);
    
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        //get metadata, urls, image paths from YouTube
        YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        
        if(videoInfo.isAvailable)
        {
            //Set up download options
            ofxYouTubeVideoUtils::USE_PRETTY_NAMES              =   true; //default: false if false, will use videoID+_+itag. if true, video title
            ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS  =   true; //default: put each video in it's own folder
            
            //download the images
            youTubeUtils.downloadAllImages(videoInfo);
            //TODO - show youTubeUtils infoCollection example
            
            
            string largestImagePath = videoInfo.getLargestImagePathAvailable();
            if (!largestImagePath.empty())
            {
                ofImage image;
                bool didLoadImage = image.loadImage(largestImagePath);
                if(didLoadImage)
                {
                    images.push_back(image);
                }
            }
            
            
            
            
            bool doSelectedFormats = true;
            
            //grab only selected formats;
            if(doSelectedFormats)
            {
                
                /*
                 The YouTubeVideo object "videoInfo" contains urls and what formats are available
                 We can ask for certain formats a few different ways
                 */
                
                /*
                If you just want the largest video you can ask for it via getLargestResolutionVideo()
                    However, it can be in a hard to playback form (webm) or may not contain audio, etc
                    
                    On the Mac we will want audio and video and a MP4 file, so we specify that
                */
                    
                YouTubeFormat largestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_AUDIO_VIDEO, YouTubeFormat::CONTAINER_MP4);
                vector<YouTubeFormat> selectedFormats; //getURLs takes a vector
                selectedFormats.push_back(largestFormat);
                
                
                vector<YouTubeVideoURL> urlsForPreferredFormats = videoInfo.getURLs(selectedFormats);
                
                /*
                 append results to our list of urls to download
                 */
            
                youTubeVideoURLs.insert(youTubeVideoURLs.end(), urlsForPreferredFormats.begin(), urlsForPreferredFormats.end());
            }else
            {
                //grab them all
                youTubeVideoURLs.insert(youTubeVideoURLs.end(), videoInfo.videoURLs.begin(), videoInfo.videoURLs.end());
            }
        }else
        {
            ofLogError(__func__) << videoIDs[i] << "IS UNAVAILABLE reason: " << videoInfo.failReason;
            //videoIDs.erase(videoIDs.begin()+i);
        }
    }
    
    if(youTubeVideoURLs.empty())
    {
        info << "NO MOVIES AVAILABLE" << "\n";
    }else
    {
        
        for(int i=0; i<youTubeVideoURLs.size(); i++)
        {
            bool doAsync                =   true;   //default: false
            bool doOverwriteExisting    =   false;  //default: false
            
            
            /*
             Start downloading, when a download is complete
             onYouTubeVideoDownloadComplete will be called pushing the filepath
             into videoPaths which is checked in update()
             */
            
            info << "\n" << "STARTING videoID: "        << youTubeVideoURLs[i].videoID ;
            info << "\n" << "doOverwriteExisting: "     << doOverwriteExisting;
            youTubeUtils.downloadVideo(youTubeVideoURLs[i], doAsync, doOverwriteExisting);
        }
        
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    if (!videoPaths.empty())
    {
        if(doLoadNextMovie || !videoPlayer.isPlaying())
        {
            doLoadNextMovie = false;
            if (videoCounter+1 < videoPaths.size())
            {
                if(isFirstRun)
                {
                    isFirstRun = false;
                }else
                {
                    videoCounter++;
                }
                
            }else
            {
                videoCounter = 0;
            }
            videoPlayer.loadMovie(videoPaths[videoCounter]);
            videoPlayer.setLoopState(OF_LOOP_NONE);
            videoPlayer.play();
        }
    }
    
    ofSetWindowTitle(ofToString(videoCounter));
    if(videoPlayer.isPlaying())
    {
        videoPlayer.update();
    }
}

//--------------------------------------------------------------
void ofApp::draw()
{
    if(videoPlayer.isPlaying())
    {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    ofDrawBitmapStringHighlight(info.str(), 20, 20, ofColor(0, 0, 0, 128), ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == 'n')
    {
        doLoadNextMovie = true;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
