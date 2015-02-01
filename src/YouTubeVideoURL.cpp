#include "YouTubeVideoURL.h"

YouTubeVideoURL::YouTubeVideoURL()
{
    videoID = "";
    url = "";
    itag = -1;
}

void YouTubeVideoURL::setup(string videoID_, string url_)
{
    videoID = videoID_;
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
                if(itag !=-1)
                {
                    format = YouTubeFormatCollection::getInstance().getFormatForItag(itag);
                }
            }
            valueMap[keyValues[0]] = keyValues[1];
            valueMapNames.push_back(keyValues[0]);
        }
    }
}

void YouTubeVideoURL::print()
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