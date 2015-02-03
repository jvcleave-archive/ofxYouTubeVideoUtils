#include "ofApp.h"



void ofApp::onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e)
{
    info << "\n" << "COMPLETED ";
    info <<"\n" <<	"videoID: "     << e.downloadRequest.videoID;
    info <<"\n" <<	"filePath: "	<< e.downloadRequest.filePath;
    videoPaths.push_back(e.downloadRequest.filePath);
    
}

void ofApp::onYouTubeDownloadEventError(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << e.message;
    
    info << "\n" << "FAILED ";
    info << "\n" << "url: "         << e.downloadRequest.url;
    info << "\n" <<	"videoID: "     << e.downloadRequest.videoID;
    info << "\n" <<	"filePath: "    << e.downloadRequest.filePath;
}



//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(ofColor::black);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
    youTubeUtils.addListener(this);
    
    string videoID = "1EROmqidZQc";
    
    YouTubeVideoInfo videoInfo = youTubeUtils.loadVideoInfo(videoID);
    
    if(videoInfo.isAvailable) //ie - if embedding is not allowed, can't download
    {
        youTubeUtils.downloadAllImages(videoInfo);
        //TODO - show youTubeUtils infoCollection example
        
        
        string largestImagePath = videoInfo.getLargestImagePathAvailable();
        if (!largestImagePath.empty())
        {
            
            image.loadImage(largestImagePath);
        }
        
        YouTubeFormat largestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_AUDIO_VIDEO, YouTubeFormat::CONTAINER_MP4);
        
        //largestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::STREAM_VIDEO_ONLY, YouTubeFormat::CONTAINER_MP4);
        //largestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::CONTAINER_MP4);
        //largestFormat = videoInfo.getLargestResolutionVideo(YouTubeFormat::VIDEO_ENCODING_MPEG4);
        vector<YouTubeFormat> selectedFormats; //getPreferredFormats takes a vector for now
        selectedFormats.push_back(largestFormat);
        
        
        bool doSelectedFormats = true;
        
        //grab only selected formats;
        if(doSelectedFormats)
        {
            
            
            
            vector<YouTubeVideoURL> optimalVideoURLs = videoInfo.getPreferredFormats(selectedFormats);
            ofLogVerbose(__func__) << "optimalVideoURLs SIZE: " << optimalVideoURLs.size();
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), optimalVideoURLs.begin(), optimalVideoURLs.end());
        }else
        {
            //grab them all
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), videoInfo.videoURLs.begin(), videoInfo.videoURLs.end());
        }
    }else
    {
        ofLogError(__func__) << videoID  << "IS UNAVAILABLE reason: " << videoInfo.failReason;
        //videoIDs.erase(videoIDs.begin()+i);
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
            string customPath           =   "";     //default: ""
            
            info << "\n" << "STARTING videoID: " << youTubeVideoURLs[i].videoID ;
            info << "\n" << "doOverwriteExisting: " << doOverwriteExisting;
            youTubeUtils.downloadVideo(youTubeVideoURLs[i], doAsync, doOverwriteExisting, customPath);
        }
        
    }
 
    
   
}



//--------------------------------------------------------------
void ofApp::update()
{
    if(!videoPaths.empty() && !videoPlayer.isPlaying())
    {
        videoCounter = 0;
        videoPlayer.setLoopState(OF_LOOP_NONE);
        videoPlayer.loadMovie(videoPaths[videoCounter]);
        
        videoPlayer.play();
        
        
    }
    if(videoPlayer.isPlaying())
    {
        videoPlayer.update();
    }
    
    ofSetWindowTitle("getIsMovieDone: "+ofToString(videoPlayer.getIsMovieDone()));
}


//--------------------------------------------------------------
void ofApp::draw(){
	
    if(videoPlayer.isPlaying())
    {
        videoPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    }
    
    image.draw(0, 0, image.getWidth()/4, image.getHeight()/4);
    

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

	//ofLogVerbose(__func__) << "key: " << key;
	switch (key) 
	{
		case 'n':
		{
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
			break;
		}
		case 's':
		{
			
			break;
		}
	}
}


