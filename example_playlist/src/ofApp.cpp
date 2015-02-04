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
    youTubeUtils.addListener(this);
    doLoadNextMovie = false;
    videoCounter = 0;
    string playlistID = "PLW1HSnzWuArWiwWjkkUN7-8CjoRzBuCYi";
    
    videoIDs = youTubeUtils.getVideoIDsFromPlaylist(playlistID);
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        if(videoInfo.isAvailable)
        {
            ofxYouTubeVideoUtils::USE_PRETTY_NAMES              =   true; //default: false
            ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS  =   true; //default: false
            
            youTubeUtils.downloadAllImages(videoInfo);
            //TODO - show youTubeUtils infoCollection example
            
            
            string largestImagePath = videoInfo.getLargestImagePathAvailable();
            if (!largestImagePath.empty())
            {
                ofImage image;
                bool didLoadImage = image.loadImage(largestImagePath);
                if(didLoadImage)
                {
                    //images.push_back(image);
                }
            }
            
            
            bool doSelectedFormats = true;
            
            //grab only selected formats;
            if(doSelectedFormats)
            {
                
                YouTubeFormat largestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_AUDIO_VIDEO, YouTubeFormat::CONTAINER_MP4);
                vector<YouTubeFormat> selectedFormats; //getURLs takes a vector
                selectedFormats.push_back(largestFormat);
                
                
                vector<YouTubeVideoURL> urlsForPreferredFormats = videoInfo.getURLs(selectedFormats);
                ofLogVerbose(__func__) << "urlsForPreferredFormats SIZE: " << urlsForPreferredFormats.size();
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
        ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS   =   true; //default: false
        
        for(int i=0; i<youTubeVideoURLs.size(); i++)
        {
            bool doAsync                =   true;   //default: false
            bool doOverwriteExisting    =   false;  //default: false
            
            info << "\n" << "STARTING videoID: "        << youTubeVideoURLs[i].videoID ;
            info << "\n" << "doOverwriteExisting: "     << doOverwriteExisting;
            youTubeUtils.downloadVideo(youTubeVideoURLs[i], doAsync, doOverwriteExisting);
        }
        
    }
}

bool isFirstRun = true;
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
                
            }else{
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
