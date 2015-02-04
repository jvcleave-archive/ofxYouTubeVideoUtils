#pragma once

#include "ofMain.h"
#include "Poco/URI.h"
#include "YouTubeVideoURL.h"
#include "YouTubeVideoMetadata.h"

class YouTubeVideo
{
public:

    string videoID;
    YouTubeVideoMetadata metadata;
    
    vector<YouTubeVideoURL> videoURLs;
    vector<string>          imageURLs;
    vector<YouTubeFormat>   formats;

    static string API_URL;
    bool hasLoaded;
    bool isAvailable;
    string failReason;
    
    YouTubeVideo();
    bool fetchInfo(string videoID_);
    vector<YouTubeVideoURL> getURLs(vector<YouTubeFormat>&);
    vector<YouTubeVideoURL> getURLs(vector<int>&);
    
    string getLargestImagePathAvailable();
   
    YouTubeFormat getLargestResolutionVideo();
    YouTubeFormat getLargestResolutionVideo(YouTubeFormat::STREAM);
    YouTubeFormat getLargestResolutionVideo(YouTubeFormat::CONTAINER);
    YouTubeFormat getLargestResolutionVideo(YouTubeFormat::VIDEO_ENCODING);
    YouTubeFormat getLargestResolutionVideo(YouTubeFormat::STREAM, YouTubeFormat::CONTAINER);
    YouTubeFormat getLargestResolutionVideo(YouTubeFormat::STREAM, YouTubeFormat::CONTAINER, YouTubeFormat::VIDEO_ENCODING);
    
    
    void print();
    string toString();
    void writeToLog(string logPath="");
    
private:
    ofHttpResponse httpResponse;
    YouTubeFormat getLargestResolutionVideo(int streamType, int container, int videoEncoding);
    
    vector <int>    itags;
    vector<string>  urls;
    vector<string>  fallback_hosts;
    
    vector <string> urlParams;
    vector <string> keys;
    vector <string> values;
    

};