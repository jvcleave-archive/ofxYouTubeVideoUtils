#pragma once
#include "ofMain.h"
#include "YouTubeVideoInfo.h"
#include "YouTubeFormat.h"

class YouTubeDownloadRequest
{
    public:
        string url;
        string videoID;
        string filePath;
        bool isAsync;
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
        virtual void onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e) = 0;
        virtual void onYouTubeDownloadEventError(YouTubeDownloadEventData& e) = 0;
};

class ofxYouTubeVideoUtils
{
    public:
        YouTubeVideoInfo loadVideoInfo(string youTubeVideoID);
        map<string, YouTubeVideoInfo> infoCollection;
        void printKeyValueMap(string youTubeVideoID);
    

        map<int, YouTubeFormat> formats;

        bool downloadVideo(YouTubeVideoURL videoURL,
                           bool doAsync=false,
                           bool doOverwriteExisting=false,
                           bool groupIntoFolder=false,
                           string customPath="");

        void downloadAllImages(YouTubeVideoInfo& videoInfo);

        void onVideoHTTPResponse(ofHttpResponse & response);

        vector<YouTubeDownloadRequest> downloadRequests;

        YouTubeDownloadEventListener* listener;

        ofxYouTubeVideoUtils()
        {
            listener = NULL;
            YouTubeFormat formatter;
            formatter.createFormats(formats);
        }
    
        ~ofxYouTubeVideoUtils()
        {
            ofLogVerbose(__func__) << "downloadRequests.empty: " << downloadRequests.empty();
            if(!downloadRequests.empty())
            {
                ofStopURLLoader();
                ofRemoveAllURLRequests();
            }
        }
    
        void addListener(YouTubeDownloadEventListener* listener_)
        {
            listener = listener_;
        }
    
        void removeListener(YouTubeDownloadEventListener* listener_)
        {
            if(listener_ == listener)
            {
                listener = NULL;
            }
        }

    private:
        string createFileName(YouTubeVideoURL& videoURL, bool groupIntoFolder = false);
        void broadcastDownloadEventComplete(YouTubeDownloadEventData& eventData);
        void broadcastDownloadEventError(YouTubeDownloadEventData& eventData);
        void handleRedirect(string redirectedURL);
    
};

