#include "ofxYouTubeVideoUtils.h"

ofxYouTubeVideoUtils::ofxYouTubeVideoUtils()
{
    createKnownFormats();
}



YouTubeVideoInfo ofxYouTubeVideoUtils::loadVideoInfo(string youTubeVideoID)
{
    ofLogVerbose() << "LOADING youTubeVideoID: " << youTubeVideoID;
    YouTubeVideoInfo videoInfo;
    videoInfo.fetchInfo(youTubeVideoID);
    infoCollection[youTubeVideoID] = videoInfo;
    return videoInfo;
}

void ofxYouTubeVideoUtils::printKeyValueMap(string youTubeVideoID)
{
    infoCollection[youTubeVideoID].printKeyValueMap();
}


void ofxYouTubeVideoUtils::createVideoFormat(int itag, string fileExtension, int width, int height, string audioCodec, string videoCodec, float fps)
{
    YouTubeVideoFormat format;
    format.setup(itag, fileExtension, width, height, audioCodec, videoCodec, fps);
    videoFormats[itag] = format;
}

string isRedirect(string& url)
{
    ofHttpResponse response = ofLoadURL(url);
    
    if(response.status == 302)
    {
        ofLogVerbose(__func__) << "302 !!!!!!!!!! \n response.data.getText: " << response.data.getText();
        ofLogVerbose() << "response.request.url: \n\n\n" << response.request.url;
        
        ofLogVerbose() << "url: \n\n\n" << url;
    }
    return url;
}

bool ofxYouTubeVideoUtils::downloadVideo(YouTubeVideoURL videoURL, bool doAsync)
{
    bool success = false;
    stringstream info;
    
    string fileName = videoURL.videoID + "_" + ofToString(videoURL.itag) + ".mp4";
    fileName = ofToDataPath(fileName, true);
    ofFile file(fileName);
    if(file.exists())
    {
        info << "fileName: " << fileName << " EXISTS - NOT OVERWRITING";
        success = true;
    }else
    {
        
        
        int response = -1;
        
        info << "DOWNLOADING TO: " << fileName;
        info << " ASYNC: " << doAsync;
        if (doAsync)
        {
            ofRegisterURLNotification(this);
            YouTubeDownloadRequest downloadRequest;
            downloadRequest.url = videoURL.url;
            downloadRequest.videoID = videoURL.videoID;
            downloadRequest.filePath = fileName;
            
            downloadRequests.push_back(downloadRequest);
            isRedirect(downloadRequest.url);
            
            response = ofSaveURLAsync(downloadRequest.url, fileName);
            info << " response: " << response;
            success = true;
        }else
        {
            ofHttpResponse httpResponse = ofSaveURLTo(videoURL.url, fileName);
            info << " response: " << httpResponse.status;
            response = httpResponse.status;
            if(response>0)
            {
                success = true;
            }else
            {
                info << " DOWNLOAD FAILED with response " << response;
            }
        }
        
    }
    ofLogVerbose(__func__) << info.str();
    
    return success;
}

void ofxYouTubeVideoUtils::urlResponse(ofHttpResponse& response)
{
    //ofLogVerbose(__func__) << "response.request.url: " << response.request.url;
    
    for (size_t i=0; i<downloadRequests.size(); i++)
    {
        if(response.request.url == downloadRequests[i].url)
        {
            if(response.status>0 && response.status != 302)
            {
                if(listener)
                {
                    YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                    listener->onYouTubeDownloadEventComplete(eventData);
                }
            }else
            {
                if(listener)
                {
                    YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                    listener->onYouTubeDownloadEventError(eventData);
                }
            }
            
        }
    }
}

#if 0
enum YouTubeFormatType
{
    WIDTH_UNKNOWN,
    HEIGHT_UNKNOWN,
    MP4,
    3GP,
    AUDIOCODEC_NONE,
    VIDEOCODEC_H264
};
#endif
void ofxYouTubeVideoUtils::createKnownFormats()
{
    int WIDTH_UNKNOWN=0;
    string NO_AUDIO="";
    string NO_VIDEO="";
    string AAC="";
    
    createVideoFormat(5, "flv", 426, 240);
    createVideoFormat(13, "3gp");
    createVideoFormat(17, "3gp", 176, 144);
    createVideoFormat(18, "mp4", 640, 360);
    createVideoFormat(22, "mp4", 1280, 720);
    createVideoFormat(36, "3gp", 320, 240);
    createVideoFormat(37, "mp4", 1920, 1080);
    createVideoFormat(38, "mp4", 4096, 3072);
    createVideoFormat(82, "mp4", 640, 360, AAC);//3d
    createVideoFormat(83, "mp4", 640, 480);
    createVideoFormat(84, "mp4", 1280, 720, AAC);//3d
    createVideoFormat(85, "mp4", 1920, 1080);
    createVideoFormat(92, "mp4", 320, 240);
    createVideoFormat(93, "mp4", 480, 360);
    createVideoFormat(94, "mp4", 640, 480);
    createVideoFormat(95, "mp4", 1280, 720);
    createVideoFormat(96, "mp4", 1920, 1080);
    createVideoFormat(132, "mp4", WIDTH_UNKNOWN, 240);
    createVideoFormat(151, "mp4", WIDTH_UNKNOWN, 72);
    createVideoFormat(133, "mp4", 426, 240, NO_AUDIO);
    createVideoFormat(134, "mp4", 640, 360, NO_AUDIO);
    createVideoFormat(135, "mp4", 854, 480, NO_AUDIO);
    createVideoFormat(136, "mp4", 1280, 720, NO_AUDIO);
    createVideoFormat(137, "mp4", 1920, 1080, NO_AUDIO);
    createVideoFormat(140, "mp4", 0, 0, AAC, NO_VIDEO);
    createVideoFormat(160, "mp4", 256, 144, NO_AUDIO);
    createVideoFormat(264, "mp4", WIDTH_UNKNOWN, 1440, "none");
    createVideoFormat(298, "mp4", WIDTH_UNKNOWN, 720, "none", "h264", 60.0);
    createVideoFormat(299, "mp4", WIDTH_UNKNOWN, 1080, "none", "h264", 60.0);
    createVideoFormat(266, "mp4", WIDTH_UNKNOWN, 2160, "none");
}
