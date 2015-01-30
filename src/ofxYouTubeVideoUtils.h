#pragma once
#include "ofMain.h"
#include "YouTubeVideoInfo.h"
#include "YouTubeFormat.h"

#ifdef TARGET_OSX
#define __func__ __FUNCTION__
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
        YouTubeVideoInfo loadVideoInfo(string youTubeVideoID);
    
        vector<YouTubeVideoInfo> infoCollection;
        vector<YouTubeFormat> formatCollection;
    
    
        bool downloadVideo(YouTubeVideoURL videoURL,
                           bool doAsync=false,
                           bool doOverwriteExisting=false,
                           bool groupIntoFolder=false,
                           string customPath="");

        void downloadAllImages(YouTubeVideoInfo& videoInfo);

    

        vector<YouTubeDownloadRequest> downloadRequests;

    
    
        void printKeyValues(string youTubeVideoID);
    
    
        ofxYouTubeVideoUtils()
        {
            listener = NULL;
            YouTubeFormat formatter;
            formatter.createFormats(formatCollection);
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
    
        YouTubeFormat getFormat(int itag)
        {
            YouTubeFormat format;
            for(size_t i=0; i<formatCollection.size(); i++ )
            {
                if(formatCollection[i].itag == itag)
                {
                    format = formatCollection[i];
                    break;
                }
            }
            return format;
        }
        void findiTagsForVideoResolution(vector<YouTubeFormat>& formats, YouTubeFormat::VIDEO_RESOLUTION videoResolution)
        {
            for(size_t i=0; i<formatCollection.size(); i++ )
            {
                if(formatCollection[i].videoResolution == videoResolution)
                {
                    formats.push_back(formatCollection[i]);
                }else
                {
                    ofLogError(__func__) << formatCollection[i].videoResolution << " IS NOT " << videoResolution;
                }
            }
        }
    
    private:
        YouTubeDownloadEventListener* listener;
    
        string createFileName(YouTubeVideoURL& videoURL, bool groupIntoFolder = false);
        void broadcastDownloadEventComplete(YouTubeDownloadEventData& eventData);
        void broadcastDownloadEventError(YouTubeDownloadEventData& eventData);
        void handleRedirect(string redirectedURL);
        void onVideoHTTPResponse(ofHttpResponse & response);
    
};

