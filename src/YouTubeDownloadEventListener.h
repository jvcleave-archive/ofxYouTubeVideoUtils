#pragma once

#include "ofMain.h"

class YouTubeDownloadRequest
{
public:
    string url;
    string videoID;
    string filePath;
    bool isAsync;
    YouTubeVideoURL videoURL;
    YouTubeDownloadRequest()
    {
        url="";
        videoID="";
        filePath = "";
        isAsync = false;
    }
};

class YouTubeDownloadEventData
{
public:
    YouTubeDownloadEventData(YouTubeDownloadRequest& downloadRequest_, ofHttpResponse& httpResponse_, void* listener_, string message_="")
    {
        downloadRequest  = downloadRequest_;
        httpResponse = httpResponse_;
        listener = listener_;
        message = message_;
    }
    
    YouTubeDownloadEventData(YouTubeDownloadRequest& downloadRequest_,  void* listener_, string message_="")
    {
        downloadRequest  = downloadRequest_;
        listener = listener_;
        message = message_;
    }
    
    //didn't make a request - so just give a message
    YouTubeDownloadEventData(void* listener_, string message_="")
    {
        listener = listener_;
        message = message_;
    }
    YouTubeDownloadEventData()
    {
        listener=NULL;
        message="";
    }
    void* listener;
    string message;
    ofHttpResponse httpResponse;
    YouTubeDownloadRequest downloadRequest;
};

class YouTubeDownloadEventListener
{
public:
    virtual void onYouTubeVideoDownloadComplete(YouTubeDownloadEventData&) = 0;
    virtual void onYouTubeDownloadError(YouTubeDownloadEventData&) = 0;
    virtual void onYouTubeAllVideosDownloadComplete() = 0;
};
