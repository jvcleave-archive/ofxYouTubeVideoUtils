#pragma once
#include "ofMain.h"
#include "YouTubeVideoMetadata.h"
#include "YouTubeFormatCollection.h"

class YouTubeVideoURL
{
public:
    string videoID;
    string url;
    int itag;
    YouTubeVideoMetadata metadata;
    YouTubeFormat format;
    map<string, string> valueMap;
    vector<string> valueMapNames;
    
    YouTubeVideoURL();
    bool setup(string videoID_, YouTubeVideoMetadata metadata_, string url_, string delimiter="&");
    bool didTryDecodeAgain;
    bool triedLastTime;
    void print();
};