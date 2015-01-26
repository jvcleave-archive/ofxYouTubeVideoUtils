#pragma once
#include "ofMain.h"


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


class YouTubeVideo
{
public:
    string url;
    int itag;
    map<string, string> valueMap;
    vector<string> valueMapNames;
    
    string videoID;
    string title;
    string author;
    
    YouTubeVideo()
    {
        videoID = "";
        title = "";
        author = "";
        url = "";
        itag = -1;
    }
    
   void setup(string url_)
    {
        url = url_;
        
        vector <string> params = ofSplitString(url, "&");
        for(size_t i=0; i<params.size(); i++)
        {
            string currentParam = params[i];
            vector <string> keyValues = ofSplitString(currentParam, "=");
            if(keyValues.size()>=2)
            {
                if(keyValues[0] == "itag")
                {
                    itag = ofToInt(keyValues[1]);
                }
                valueMap[keyValues[0]] = keyValues[1];
                valueMapNames.push_back(keyValues[0]);
            }
        }
    }
    
    void print()
    {
        stringstream info;
        info << "url: " << url << "\n";
        info << "itag: " << itag << "\n";
        
        info << "valueMap key/values START \n";
        for(size_t i=0; i<valueMapNames.size(); i++)
        {
            info << valueMapNames[i] << " : " << valueMap[valueMapNames[i]] << "\n";
        }
        ofLogVerbose(__func__) << info.str();
    }
};