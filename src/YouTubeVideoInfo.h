#pragma once
#include "ofMain.h"
#include "Poco/URI.h"
#include "YouTubeVideoURL.h"

class YouTubeVideoURL;

class YouTubeVideoInfo
{
public:
    string videoID;
    string title;
    string author;
    int lengthInSeconds;
    string smallImagePath;
    string mediumImagePath;
    string largeImagePath;
    string largestImagePath;
    
    vector <string> urlParams;
    
    vector <string> keys;
    vector <string> values;
    
    vector<YouTubeVideoURL> videoURLs;
    vector<string> imageURLs;
    vector <int> itags;
    vector<string> urls;
    vector<YouTubeFormat> formats;
    vector<string> fallback_hosts;
    
    static string API_URL;
    bool hasLoaded;
    bool isAvailable;
    string failReason;
    
    YouTubeVideoInfo();
    bool fetchInfo(string videoID_);
    vector<YouTubeVideoURL> getPreferredFormats(vector<YouTubeFormat>& preferredFormats);
    vector<YouTubeVideoURL> getPreferredFormats(vector<int>& preferredTags);
    string getLargestImagePathAvailable();
    YouTubeFormat getFormatForLargestResolutionForStreamTypeAndContainer(YouTubeFormat::STREAM streamType, YouTubeFormat::CONTAINER container);
    void print();
    string toString();
    void writeToLog(string logPath="");
private:
    ofHttpResponse httpResponse;

};