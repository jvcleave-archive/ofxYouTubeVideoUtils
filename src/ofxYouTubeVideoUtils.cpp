#include "ofxYouTubeVideoUtils.h"



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
            //isRedirect(downloadRequest.url);
            
            int queueID = ofSaveURLAsync(downloadRequest.url, fileName);
            info << " queueID: " << queueID;
            success = true;
        }else
        {
            ofHttpResponse httpResponse = ofSaveURLTo(videoURL.url, fileName);
            info << " response: " << httpResponse.status;
            if(httpResponse.status == 302)
            {
                ofLogVerbose(__func__) << "302 !!!!!!!!!! \n response.data.getText: " << httpResponse.data.getText();
                ofLogVerbose() << "response.request.url: \n\n\n" << httpResponse.request.url;
                
                ofLogVerbose() << "url: \n\n\n" << videoURL.url;
            }else
            {
                if(httpResponse.status>0)
                {
                    success = true;
                }else
                {
                    info << " DOWNLOAD FAILED with response " << httpResponse.status;
                }
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
/*
void ofxYouTubeVideoUtils::createKnownFormats()
{
    int WIDTH_UNKNOWN=0;
    string NO_AUDIO="";
    string NO_VIDEO="";
    string H264="";
    string MPEG4="";
    string VP9="";
    string FLASH="";
    
    string AAC="";
    string MPEG_AUDIO = "";
    string WEBM_AUDIO = "";
    
    createVideoFormat(5, "flv",     426,    240,    MPEG_AUDIO, FLASH);//checked
    createVideoFormat(13, "3gp");
    createVideoFormat(17, "3gp",    176,    144,    AAC,        MPEG4); //checked
    createVideoFormat(18, "mp4",    640,    360,    AAC,        H264);  //checked
    createVideoFormat(22, "mp4",    1280,   720,    AAC,        H264);  //checked
    createVideoFormat(36, "3gp",    320,    240,    AAC,        MPEG4); //checked
    createVideoFormat(37, "mp4",    1920,   1080);
    createVideoFormat(38, "mp4",    4096,   3072);
    //probably 3D
    createVideoFormat(82, "mp4",    640,    360,    AAC,        H264);  //3d, checked
    createVideoFormat(83, "mp4",    640,    480);
    createVideoFormat(84, "mp4",    1280,   720, AAC);//3d
    createVideoFormat(85, "mp4",    1920,   1080);
    //probably WEBM
    createVideoFormat(92, "mp4",    320,    240);
    createVideoFormat(93, "mp4",    480,    360);
    createVideoFormat(94, "mp4",    640,    480);
    createVideoFormat(95, "mp4",    1280,   720);
    createVideoFormat(96, "mp4",    1920,   1080);
    
    createVideoFormat(160, "mp4",   256,    144,    NO_AUDIO,   H264); //checked
    createVideoFormat(151, "mp4",   128,    72);
    createVideoFormat(132, "mp4", WIDTH_UNKNOWN, 240);
    createVideoFormat(133, "mp4",   426,    240,    NO_AUDIO,   H264); //checked
    createVideoFormat(134, "mp4",   640,    360,    NO_AUDIO,   H264); //checked
    createVideoFormat(135, "mp4",   854,    480,    NO_AUDIO,   H264); //checked
    createVideoFormat(136, "mp4",   1280,   720,    NO_AUDIO,   H264); //checked
    createVideoFormat(137, "mp4",   1920,   1080,   NO_AUDIO,   H264); //checked
    createVideoFormat(140, "mp4",   0,      0,      AAC,        NO_VIDEO); //checked
    createVideoFormat(171, "mp4",   0,      0,      WEBM_AUDIO, NO_VIDEO); //checked
    
    
    createVideoFormat(264, "mp4",   2560,   1440,   NO_AUDIO,   H264); //checked
    createVideoFormat(242, "VP9",   426,    240,    NO_AUDIO,   VP9); //checked
    createVideoFormat(298, "mp4",   1280,   720,    NO_AUDIO,   H264, 60.0); //checked
    createVideoFormat(299, "mp4",   1920,   1080,   NO_AUDIO,   H264, 60.0); //checked
    createVideoFormat(266, "mp4", WIDTH_UNKNOWN, 2160, "none");
}
*/




