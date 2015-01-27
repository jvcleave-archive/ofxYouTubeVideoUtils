#pragma once
#include "ofMain.h"
#include "YouTubeVideoInfo.h"

class YouTubeVideoFormat
{

public:
    int itag;
    int width;
    int height;
    float fps;
    string fileExtension;
    string audioCodec;
    string videoCodec;
    YouTubeVideoFormat()
    {
        itag=0;
        width=0;
        height=0;
        fps=0.0;
        fileExtension = "";
        audioCodec = "";
        videoCodec = "";
    }
    void setup(int itag, string fileExtension, int width=0, int height=0, string audioCodec="", string videoCodec="", float fps=0.0)
    {
        itag = itag;
        fileExtension=fileExtension;
        height=height;
        width=width;
        audioCodec=audioCodec;
        videoCodec=videoCodec;
        fps=fps;
    }
    
};

class YouTubeDownloadRequest
{
    public:
        string url;
        string videoID;
        string filePath;
        YouTubeDownloadRequest()
        {
            url="";
            videoID="";
            filePath = "";
        }
};

class YouTubeDownloadEventData
{
public:
    YouTubeDownloadEventData(YouTubeDownloadRequest& downloadRequest, ofHttpResponse& httpResponse, void* listener)
    {
        this->downloadRequest  = downloadRequest;
        this->httpResponse = httpResponse;
        this->listener = listener;
    }
    void* listener;
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
    
        ofxYouTubeVideoUtils();
    
        YouTubeVideoInfo loadVideoInfo(string youTubeVideoID);
        map<string, YouTubeVideoInfo> infoCollection;
        void printKeyValueMap(string youTubeVideoID);
        map<int, YouTubeVideoFormat> videoFormats;
        bool downloadVideo(YouTubeVideoURL videoURL, bool doAsync=false);
        void urlResponse(ofHttpResponse & response);
    
        vector<YouTubeDownloadRequest> downloadRequests;
    
        YouTubeDownloadEventListener* listener;
    private:
        void createVideoFormat(int itag, string fileExtension, int width=0, int height=0, string audioCodec="", string videoCodec="", float fps=0);
        void createKnownFormats();
};



/*
 299 1080p 60fps
 303 302 244 243 242 5 unknown (a9 codec), crashes
 140 171 Crashes
 
 298 (no audio)
 Video codec: omx-h264
 Video width: 1280
 Video height: 720
 Video profile: 77
 Video fps: 59.9401
 
 135 (no audio)
 Video codec: omx-h264
 Video width: 854
 Video height: 480
 Video profile: 77
 Video fps: 29.97
 
 134 (no audio)
 Video codec: omx-h264
 Video width: 640
 Video height: 360
 Video profile: 77
 Video fps: 29.97
 133 (no audio)
 Video codec: omx-h264
 Video width: 426
 Video height: 240
 Video profile: 77
 Video fps: 29.97
 
 *   36
 Video codec: omx-mpeg4
 Video width: 320
 Video height: 180
 Video profile: -99
 Video fps: 29.97
 
 *    22
 Video codec: omx-h264
 Video width: 1280
 Video height: 720
 Video profile: 100
 Video fps: 29.97
 *     18
 Video codec: omx-h264
 Video width: 640
 Video height: 360
 Video profile: 578
 Video fps: 29.97
 *    17
 Video codec: omx-mpeg4
 Video width: 176
 Video height: 144
 Video profile: -99
 Video fps: 9.99
 
 43  Crashes
 Video codec: omx-vp8
 Video width: 640
 Video height: 360
 Video profile: -99
 Video fps: 29.97
 
 160 (no audio)
 Video codec: omx-h264
 Video width: 256
 Video height: 144
 Video profile: 77
 Video fps: 14.985
 */

