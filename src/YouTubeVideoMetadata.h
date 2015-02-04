#pragma once
#include "ofMain.h"


class YouTubeVideoMetadata
{
public:
    YouTubeVideoMetadata();
    
    string videoID;
    string title;
    string author;
    int lengthInSeconds;
    
    string smallImagePath;
    string mediumImagePath;
    string largeImagePath;
    string largestImagePath;
    
    string getLargestImagePathAvailable();
    void print();
    string toString();
};