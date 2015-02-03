#include "ofApp.h"



void ofApp::onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e)
{
    info << "\n" << "COMPLETED ";
    info << e.downloadRequest.videoURL.metadata.toString();
    info <<"\n" <<	"filePath: "	<< e.downloadRequest.filePath;
    //videoPaths.push_back(e.downloadRequest.filePath);
    
}

void ofApp::onYouTubeDownloadEventError(YouTubeDownloadEventData& e)
{
    ofLogVerbose(__func__) << e.message;
    
    info << "\n" << "FAILED ";
    info << e.downloadRequest.videoURL.metadata.toString();
    info << "\n" << "url: "         << e.downloadRequest.url;
    info << "\n" <<	"filePath: "    << e.downloadRequest.filePath;
}


void ofApp::onVideoEnd(ofxOMXPlayerListenerEventData& e)
{
    ofLogVerbose(__func__) << " RECEIVED";
    doLoadNextMovie = true;
}





//--------------------------------------------------------------
void ofApp::setup()
{
#if 0
    ofSetLogLevel(OF_LOG_VERBOSE);
    ofSetLogLevel("ofThread", OF_LOG_SILENT);
    
    youTubeUtils.addListener(this);
    //consoleListener.setup(this);
    
    doLoadNextMovie = false;
    videoCounter = 0;
    string test_list = "PLW1HSnzWuArWiwWjkkUN7-8CjoRzBuCYi";
    
    videoIDs = youTubeUtils.getVideoIDsFromPlaylist(test_list);
    
    for(size_t i=0; i<videoIDs.size(); i++)
    {
        YouTubeVideo videoInfo = youTubeUtils.loadVideoInfo(videoIDs[i]);
        if(videoInfo.isAvailable)
        {
            ofxYouTubeVideoUtils::USE_PRETTY_NAMES              =   true; //default: false
            ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS  =   true; //default: false
            
            youTubeUtils.downloadAllImages(videoInfo);
            
            string largestImagePath = videoInfo.getLargestImagePathAvailable();
            if (!largestImagePath.empty())
            {
                /*ofImage image;
                bool didLoadImage = image.loadImage(largestImagePath);
                if(didLoadImage)
                {
                    //images.push_back(image);
                }*/
            }
            
            
            vector<int> selectedItags; //getPreferredFormats takes a vector
            selectedItags.push_back(18);
            vector<YouTubeVideoURL> urlsForPreferredFormats = videoInfo.getPreferredFormats(selectedItags);
            youTubeVideoURLs.insert(youTubeVideoURLs.end(), urlsForPreferredFormats.begin(), urlsForPreferredFormats.end());
        }else
        {
            ofLogError(__func__) << videoIDs[i] << "IS UNAVAILABLE reason: " << videoInfo.failReason;
            //videoIDs.erase(videoIDs.begin()+i);
        }
    }
#endif
    /*
    if(youTubeVideoURLs.empty())
    {
        info << "NO MOVIES AVAILABLE" << "\n";
    }else

    {
        ofLogVerbose() << "TRYING ITAG: " << youTubeVideoURLs[videoCounter].itag << "\n\n\n\n";
        
        ofxOMXPlayerSettings settings;
        settings.videoPath = youTubeVideoURLs[videoCounter].url;
        ofLogVerbose() << "settings.videoPath: " << settings.videoPath;
        settings.enableAudio = true;
        settings.useHDMIForAudio = true;	//default true
        settings.enableLooping = false;		//default true
        settings.enableTexture = true;		//default true
        settings.listener = this;			//this app extends ofxOMXPlayerListener so it will receive events ;
        omxPlayer.setup(settings);
    }
    */
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
    if(!omxPlayer.isTextureEnabled()) return;
    
    omxPlayer.draw(0, 0, ofGetWidth(), ofGetHeight());
    
    //draw a smaller version in the lower right
    int scaledHeight = omxPlayer.getHeight()/4;
    int scaledWidth = omxPlayer.getWidth()/4;
    omxPlayer.draw(ofGetWidth()-scaledWidth, ofGetHeight()-scaledHeight, scaledWidth, scaledHeight);
    
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

void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
    keyPressed((int)e.character);
}
