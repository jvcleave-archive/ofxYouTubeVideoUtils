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

string ofxYouTubeVideoUtils::createFileName(YouTubeVideoURL& videoURL,
                                            bool groupIntoFolder) //default: false
{
    //TODO Pretty format option (e.g. Title + Resolution)
    stringstream name;
    name << videoURL.videoID;
    name << "_";
    name << videoURL.itag;
    
    YouTubeFormat& format = formats[videoURL.itag];
    ofLogVerbose() << "VIDEO_RESOLUTION: " << format.videoResolution;
    switch (format.container)
    {
        case YouTubeFormat::CONTAINER_3GP:  { name << ".3gp";   break;  }
        case YouTubeFormat::CONTAINER_FLV:  { name << ".flv";   break;  }
        case YouTubeFormat::CONTAINER_MP4:  { name << ".mp4";   break;  }
        case YouTubeFormat::CONTAINER_WEBM: { name << ".webm";  break;  }
        case YouTubeFormat::CONTAINER_TS:   { name << ".webm";  break;  }
        default:                            { name << ".unknownformat"; }
    }
    
    string fileName;
    if(groupIntoFolder)
    {
        ofDirectory groupFolder = ofToDataPath(videoURL.videoID, true);
        if(!groupFolder.exists())
        {
            groupFolder.create();
        }
        fileName = groupFolder.getAbsolutePath()+"/"+name.str();
    }else
    {
        fileName = ofToDataPath(name.str(), true);
    }
    
    ofLogVerbose() << "fileName: " << fileName;

    return fileName;
}



bool ofxYouTubeVideoUtils::downloadVideo(YouTubeVideoURL videoURL,
                                         bool doAsync,              //default: false
                                         bool doOverwriteExisting,  //default: false
                                         bool groupIntoFolder,      //default: false
                                         string customPath)         //default: ""  //TODO implement customPath
{
    bool success = false;
    stringstream info;
    
    string fileName = createFileName(videoURL, groupIntoFolder);
    
    ofFile file(fileName);
    
    YouTubeDownloadRequest downloadRequest;
    downloadRequest.url = videoURL.url;
    downloadRequest.videoID = videoURL.videoID;
    downloadRequest.filePath = fileName;
    if(file.exists() && file.getSize()==0)
    {
        info << "fileName: " << fileName << " EXISTS - BUT IS 0 bytes";
        doOverwriteExisting = true;
    }
    if(file.exists() && !doOverwriteExisting)
    {
        info << "fileName: " << fileName << " EXISTS - NOT OVERWRITING";
        if(listener)
        {
            YouTubeDownloadEventData eventData(downloadRequest, (void *)this, fileName+ " EXISTS - DID NOT OVERWRITE");
            broadcastDownloadEventComplete(eventData);
        }
        success = true;
    }else
    {
        
        info << "DOWNLOADING TO: " << fileName;
        info << " ASYNC: " << doAsync;
        
        if (doAsync)
        {
            ofAddListener(ofURLResponseEvent(),this,&ofxYouTubeVideoUtils::onVideoHTTPResponse);
            
            downloadRequest.isAsync = true;
            downloadRequests.push_back(downloadRequest);
            
            int queueID = ofSaveURLAsync(downloadRequest.url, fileName);
            info << " queueID: " << queueID;
            success = true;
        }else
        {
            downloadRequest.isAsync = false;
            
            ofHttpResponse httpResponse = ofSaveURLTo(videoURL.url, fileName);
            info << " response: " << httpResponse.status;
            
            if(httpResponse.status == 302)
            {
                handleRedirect(videoURL.url);
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
            YouTubeDownloadEventData eventData(downloadRequest, httpResponse, (void *)this);
            if(success)
            {
                broadcastDownloadEventComplete(eventData);
            }else
            {
                broadcastDownloadEventError(eventData);
            }
            
        }
        
    }
    ofLogVerbose(__func__) << info.str();
    
    return success;

}

void ofxYouTubeVideoUtils::downloadAllImages(YouTubeVideoInfo& videoInfo)
{
    for(size_t i=0; i<videoInfo.imageURLs.size(); i++)
    {
        string url = videoInfo.imageURLs[i];
        vector<string> urlPieces = ofSplitString(url, "/");
        string filePath = ofToDataPath(videoInfo.videoID+ "_" +urlPieces.back(), true);
        ofSaveURLTo(url, filePath); //TODO Async option? may interfere with callback for videos
    }
}

void ofxYouTubeVideoUtils::broadcastDownloadEventComplete(YouTubeDownloadEventData& eventData)
{
    if(!listener) return;
    listener->onYouTubeDownloadEventComplete(eventData);
}

void ofxYouTubeVideoUtils::broadcastDownloadEventError(YouTubeDownloadEventData& eventData)
{
    if(!listener) return;
    listener->onYouTubeDownloadEventError(eventData);
}


void ofxYouTubeVideoUtils::onVideoHTTPResponse(ofHttpResponse& response)
{
    //ofLogVerbose(__func__) << "response.request.url: " << response.request.url;
    
    for (size_t i=0; i<downloadRequests.size(); i++)
    {
        if(response.request.url == downloadRequests[i].url)
        {
            if(response.status>0)
            {
                if(response.status != 302)
                {
                    if(listener)
                    {
                        YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                        broadcastDownloadEventComplete(eventData);
                    }
                }else
                {
                    handleRedirect(response.request.url);

                }
                
            }else
            {
                if(listener)
                {
                    YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                    broadcastDownloadEventError(eventData);
                    
                }
            }
            downloadRequests.erase(downloadRequests.begin() + i);
        }
    }
    if (downloadRequests.empty())
    {
        ofLogVerbose(__func__) << "ALL DOWNLOADS COMPLETE";
    }
}

void ofxYouTubeVideoUtils::handleRedirect(string redirectedURL)
{
    //TODO - handle redirects
    ofLogVerbose(__func__) << "got a redirect from URL: " << redirectedURL;

}


