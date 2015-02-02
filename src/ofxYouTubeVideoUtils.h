#pragma once
#include "ofMain.h"
#include "YouTubeVideoInfo.h"
#include "YouTubeFormatCollection.h"

#ifdef TARGET_OSX
#define __func__ __PRETTY_FUNCTION__
#endif
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
        virtual void onYouTubeDownloadEventComplete(YouTubeDownloadEventData& e) = 0;
        virtual void onYouTubeDownloadEventError(YouTubeDownloadEventData& e) = 0;
};

class ofxYouTubeVideoUtils
{
    public:
    
    
        vector<YouTubeVideoInfo> infoCollection;
        vector<YouTubeVideoInfo> failedCollection;
    
        vector<YouTubeFormat> formatCollection;
        vector<YouTubeDownloadRequest> downloadRequests;
    
        ofxYouTubeVideoUtils();
        ~ofxYouTubeVideoUtils();
    
        YouTubeVideoInfo loadVideoInfo(string youTubeVideoID);
        bool downloadVideo(YouTubeVideoURL videoURL,
                           bool doAsync=false,
                           bool doOverwriteExisting=false,
                           bool groupIntoFolder=false,
                           string customPath="");

        void downloadAllImages(YouTubeVideoInfo& videoInfo);

        YouTubeFormat getFormat(int itag);
        void findiTagsForVideoResolution(vector<YouTubeFormat>& formats,
                                         YouTubeFormat::VIDEO_RESOLUTION videoResolution);
    
        void addListener(YouTubeDownloadEventListener* listener_);
        void removeListener(YouTubeDownloadEventListener* listener_);
        void print(string youTubeVideoID);
        vector<string> getVideoIDsFromPlaylist(string playlistID);
    
    private:
        YouTubeDownloadEventListener* listener;
    
        string createFileName(YouTubeVideoURL& videoURL,
                              bool groupIntoFolder = false);
        void broadcastDownloadEventComplete(YouTubeDownloadEventData& eventData);
        void broadcastDownloadEventError(YouTubeDownloadEventData& eventData);
    
        ofHttpResponse handleRedirect(ofHttpResponse httpResponse);
        void onVideoHTTPResponse(ofHttpResponse& response);
    
    
};

