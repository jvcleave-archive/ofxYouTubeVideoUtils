#pragma once

#include "ofMain.h"

#ifdef TARGET_OSX
#define __func__ __PRETTY_FUNCTION__
#endif

#include "YouTubeVideo.h"
#include "YouTubeFormatCollection.h"
#include "YouTubeDownloadEventListener.h"


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
        void broadcastAllDownloadsComplete();
    
        void handleRedirect(YouTubeDownloadRequest downloadRequest, string redirectedURL, string filePath);
        void onVideoHTTPResponse(ofHttpResponse&);
        
        string playlistIDName;

};

