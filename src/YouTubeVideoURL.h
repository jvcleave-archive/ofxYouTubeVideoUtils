#pragma once
#include "ofMain.h"
#include "YouTubeFormatCollection.h"

class YouTubeVideoURL
{
public:
    string videoID;
    string url;
    int itag;
    YouTubeFormat format;
    map<string, string> valueMap;
    vector<string> valueMapNames;
    
    YouTubeVideoURL();
    bool setup(string videoID_, string url_, string delimiter="&");
    bool didTryDecodeAgain;
    bool triedLastTime;
    void print();
};