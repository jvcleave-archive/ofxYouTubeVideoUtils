#include "ofApp.h"



void ofApp::onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << "COMPLETED";
    info <<"COMPLETED\n" <<	"url: "		<< e.downloadRequest.url;
    info <<"\n" <<	"videoID: " << e.downloadRequest.videoID;
    info <<"\n" <<	"filePath: "		<< e.downloadRequest.filePath;
}

void ofApp::onYouTubeDownloadEventError(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << e.message;
    
    info <<"FAILED\n" <<	"url: "		<< e.downloadRequest.url;
    info <<"\n" <<	"videoID: " << e.downloadRequest.videoID;
    info <<"\n" <<	"filePath: "		<< e.downloadRequest.filePath;
}




//--------------------------------------------------------------
void ofApp::setup()
{
	ofBackground(ofColor::black);
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
    youTubeUtils.addListener(this);
    
    
    videoCounter = 0;
    videoIDs.push_back("7zD5u9yy9kg");//wat
    //videoIDs.push_back("6pxRHBw-k8M");//4k
    
    //videoIDs.push_back("RF6cPHo_yb0");//moon
    //videoIDs.push_back("1EROmqidZQc");//mkx
    
    videoIDs.push_back("qmfm6JzuoZQ"); //reptile
    //videoIDs.push_back("zlbXf3kg5zM");//nade
    
    //videoIDs.push_back("iSxcOfRym-M");
   
    vector<int> selectedFormats;
    selectedFormats.push_back(22);
    selectedFormats.push_back(133);
    selectedFormats.push_back(299);
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        YouTubeVideoInfo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        
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
        ofLogVerbose() << "NO MOVIES AVAILABLE";
    }else
    {
        for(size_t i=0; i<youTubeVideoURLs.size(); i++)
        {
            bool doAsync                =   true;   //default: false
            bool doOverwriteExisting    =   true;  //default: false
            bool groupIntoFolder        =   true;   //default: false
            string customPath           =   "";     //default: ""

            youTubeUtils.downloadVideo(youTubeVideoURLs[i], doAsync, doOverwriteExisting, groupIntoFolder, customPath);
        }
        
    }
 
    
   
}


//--------------------------------------------------------------
void ofApp::update()
{
  
}


//--------------------------------------------------------------
void ofApp::draw(){
	

	

	ofDrawBitmapStringHighlight(info.str(), 60, 60, ofColor(ofColor::black, 90), ofColor::yellow);
}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){

	ofLogVerbose(__func__) << "key: " << key;
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


