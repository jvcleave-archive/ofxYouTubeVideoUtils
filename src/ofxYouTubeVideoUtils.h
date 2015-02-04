#pragma once

#include "ofMain.h"
#include "YouTubeVideo.h"
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
        virtual void onYouTubeDownloadEventComplete(YouTubeDownloadEventData&) = 0;
        virtual void onYouTubeDownloadEventError(YouTubeDownloadEventData&) = 0;
};

class ofxYouTubeVideoUtils
{
    public:
    
    
        vector<YouTubeVideo> infoCollection;
        vector<YouTubeVideo> failedCollection;
    
        vector<YouTubeFormat> formatCollection;
        vector<YouTubeDownloadRequest> downloadRequests;
    
        ofxYouTubeVideoUtils();
        ~ofxYouTubeVideoUtils();
    
        YouTubeVideo loadVideoInfo(string youTubeVideoID);
        bool downloadVideo(YouTubeVideoURL videoURL,
                           bool doAsync=false,
                           bool doOverwriteExisting=false,
                           string customPath="");

        void downloadAllImages(YouTubeVideo&);

        YouTubeFormat getFormat(int itag);
        vector<YouTubeFormat>  findiTagsForVideoResolution(YouTubeFormat::VIDEO_RESOLUTION);
    
        void addListener(YouTubeDownloadEventListener*);
        void removeListener(YouTubeDownloadEventListener*);
        void print(string youTubeVideoID);
        vector<string> getVideoIDsFromPlaylist(string playlistID);
    
        static bool GROUP_DOWNLOADS_INTO_FOLDERS;
        static bool USE_PRETTY_NAMES;

    private:
        YouTubeDownloadEventListener* listener;
    
        string createFileName(YouTubeVideoURL& videoURL);
        void broadcastDownloadEventComplete(YouTubeDownloadEventData&);
        void broadcastDownloadEventError(YouTubeDownloadEventData&);
    
        void handleRedirect(YouTubeDownloadRequest downloadRequest, string redirectedURL, string filePath);
        void onVideoHTTPResponse(ofHttpResponse&);
    
    
};

