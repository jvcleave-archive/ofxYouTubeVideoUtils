#include "ofApp.h"



void ofApp::onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << "COMPLETED";
    info <<"\nCOMPLETED\n" <<	"url: "		<< e.downloadRequest.url;
    info <<"\n" <<	"videoID: " << e.downloadRequest.videoID;
    info <<"\n" <<	"filePath: "		<< e.downloadRequest.filePath;
    videoPaths.push_back(e.downloadRequest.filePath);
    
}

void ofApp::onYouTubeDownloadEventError(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << e.message;
    
    info <<"\nFAILED\n" <<	"url: "		<< e.downloadRequest.url;
    info <<"\n" <<	"videoID: " << e.downloadRequest.videoID;
    info <<"\n" <<	"filePath: "		<< e.downloadRequest.filePath;
}



bool doLoadNextMovie = false;
//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(ofColor::black);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
    youTubeUtils.addListener(this);

    videoCounter = 0;
    //videoIDs.push_back("7zD5u9yy9kg");//wat
    //videoIDs.push_back("6pxRHBw-k8M");//4k
    
    //videoIDs.push_back("RF6cPHo_yb0");//moon
    videoIDs.push_back("1EROmqidZQc");//mkx
    
    videoIDs.push_back("qmfm6JzuoZQ"); //reptile
    //videoIDs.push_back("zlbXf3kg5zM");//nade
    
    //videoIDs.push_back("iSxcOfRym-M");
   
    //youTubeUtils.setup();
    
    vector<YouTubeFormat> selectedFormats;
    
    for(size_t i=0; i<youTubeUtils.formatCollection.size(); i++)
    {
        YouTubeFormat currentFormat = youTubeUtils.formatCollection[i];
        
        if(currentFormat.streamType == YouTubeFormat::STREAM_AUDIO_VIDEO &&
           currentFormat.container == YouTubeFormat::CONTAINER_MP4 &&
           currentFormat.videoProfile != YouTubeFormat::VIDEO_PROFILE_3D &&
           currentFormat.videoResolution == YouTubeFormat::VIDEO_RESOLUTION_720P)
        {
            selectedFormats.push_back(currentFormat);            
        }
        
    }
    for(size_t i=0; i<selectedFormats.size(); i++)
    {
        ofLogVerbose() << "selectedFormats: " << selectedFormats[i].toString();;

    }
    
    ofLogVerbose() << "selectedFormats SIZE: " << selectedFormats.size();
    //selectedFormats.push_back(22);
    //selectedFormats.push_back(133);
    //selectedFormats.push_back(299);
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
       // info << "GRABBING: " << videoIDs[i] << "\n";
        YouTubeVideoInfo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        youTubeUtils.downloadAllImages(videoInfo);
        ofImage image;
        image.loadImage(videoInfo.largestImagePath);
        largeImages.push_back(image);
        bool doSelectedFormats = true;
        //grab only selected formats;
        if(doSelectedFormats)
        {
            vector<YouTubeVideoURL> optimalVideoURLs = videoInfo.getPreferredFormats(selectedFormats);
            ofLogVerbose() << "optimalVideoURLs SIZE: " << optimalVideoURLs.size();
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), optimalVideoURLs.begin(), optimalVideoURLs.end());
        }else
        {
            //grab them all
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), videoInfo.videoURLs.begin(), videoInfo.videoURLs.end());
        }
    }
    
    ofLogVerbose() << "youTubeVideoURLs SIZE: " << youTubeVideoURLs.size();
    
    
    if(youTubeVideoURLs.empty())
    {
        info << "NO MOVIES AVAILABLE" << "\n";
    }else
    {
        for(size_t i=0; i<youTubeVideoURLs.size(); i++)
        {
            bool doAsync                =   true;   //default: false
            bool doOverwriteExisting    =   false;  //default: false
            bool groupIntoFolder        =   true;   //default: false
            string customPath           =   "";     //default: ""
            info << "STARTING videoID: " << youTubeVideoURLs[i].videoID << "\n";
            info << "doOverwriteExisting: " << doOverwriteExisting << "\n";
            info << "groupIntoFolder: "     << groupIntoFolder << "\n";
            youTubeUtils.downloadVideo(youTubeVideoURLs[i], doAsync, doOverwriteExisting, groupIntoFolder, customPath);
        }
        
    }
 
    
   
}

bool ofApp::loadNextMovie()
{
    if(videoPaths.empty()) return false;
    
    if(videoCounter+1<videoPaths.size())
    {
        videoCounter++;
    }else{
        videoCounter = 0;
    
    }
    
    videoPlayer.loadMovie(videoPaths[videoCounter]);
   // videoPlayer.play();
    

    return true;
}

//--------------------------------------------------------------
void ofApp::update()
{
    if(doLoadNextMovie)
    {
        bool didLoad = loadNextMovie();
        doLoadNextMovie = !didLoad;
    }
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
	
    for(size_t i=0; i<largeImages.size(); i++)
    {
        largeImages[i].draw(0, 360*i, 640, 360);
    }
    if(videoPlayer.isPlaying())
    {
        videoPlayer.draw(640, 0, videoPlayer.getWidth()/2, videoPlayer.getHeight()/2);
    }

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

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
			break;
		}
		case 's':
		{
			
			break;
		}
	}
}


