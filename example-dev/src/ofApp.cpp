#include "ofApp.h"



void ofApp::onYouTubeVideoDownloadComplete(YouTubeDownloadEventData& e)
{
    info << "\n" << "COMPLETED ";
    //info << "\n" << "title: "       << e.downloadRequest.videoURL.metadata.title;
    //info <<"\n" <<	"videoID: "     << e.downloadRequest.videoID;
    
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
void ofApp::setup()
{
	ofBackground(ofColor::black);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
    youTubeUtils.addListener(this);
    
    string videoID = "1EROmqidZQc";
    ofxYouTubeVideoUtils::USE_PRETTY_NAMES              =   true; //default: false
    ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS  =   true; //default: false
    
    YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoID);
    
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
        vector<YouTubeFormat> selectedFormats; //getURLs takes a vector for now
        selectedFormats.push_back(largestFormat);
        
        
        bool doSelectedFormats = true;
        
        //grab only selected formats;
        if(doSelectedFormats)
        {
            
            
            
            vector<YouTubeVideoURL> optimalVideoURLs = videoInfo.getURLs(selectedFormats);
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


