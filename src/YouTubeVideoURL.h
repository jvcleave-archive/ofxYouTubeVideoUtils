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
    string localFilePath;

    
    YouTubeVideoURL();
    bool setup(string videoID_,
               YouTubeVideoMetadata metadata_,
               string url_,
               string delimiter="&");
    void print();
    string toString();
    
private:
    map<string, string> valueMap;
    vector<string> valueMapNames;
    bool didTryDecodeAgain;
    bool triedLastTime;
    
};

