#pragma once
#include "ofMain.h"
#include "Poco/URI.h"
#include "YouTubeFormatCollection.h"
#include "YouTubeVideoURL.h"



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
    
    static string API_URL;
    bool hasLoaded;
    
    YouTubeVideoInfo();
    bool fetchInfo(string videoID_);
    vector<YouTubeVideoURL> getPreferredFormats(vector<YouTubeFormat>& preferredFormats);
    vector<YouTubeVideoURL> getPreferredFormats(vector<int>& itags);
    string getLargestImagePathAvailable();
    
    void print();
    string toString();
    void writeToLog(string logPath="");
private:
    ofHttpResponse httpResponse;

};