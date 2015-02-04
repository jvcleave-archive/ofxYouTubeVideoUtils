#include "YouTubeVideoURL.h"
#include "Poco/URI.h"

YouTubeVideoURL::YouTubeVideoURL()
{
    videoID = "";
    url = "";
    localFilePath = "";
    itag = -1;
    didTryDecodeAgain = false;
    triedLastTime = false;
}

bool YouTubeVideoURL::setup(string videoID_, YouTubeVideoMetadata metadata_, string url_, string delimiter)
{
    bool success = false;
    videoID = videoID_;
    metadata = metadata_;
    url = url_;
    
    vector <string> params = ofSplitString(url, delimiter);
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
    
    if(itag != -1)
    {
        format = YouTubeFormatCollection::getInstance().getFormatForItag(itag);
        success = true;
    }else
    {
        if(!didTryDecodeAgain)
        {
            didTryDecodeAgain = true;
            //ofLogVerbose(__func__) << "itag error?";
            string decodedRejectedURL;
            Poco::URI::decode(url_, decodedRejectedURL);
            success = setup(videoID, metadata, decodedRejectedURL);
        }
        if(!success && !triedLastTime)
        {
            triedLastTime= true;
            success = setup(videoID, metadata, url, "?");
            if(!success)
            {
                //ofLogVerbose() << "WTF";
            }else
            {
                //ofLogVerbose() << "LAST DITCH EFFORT WORKED WITH url: " << url;
            }
        }
        
    }
    /*
    if(!success)
    {
        //likely url is like http://www.youtube.com/video/K1vXEDEAYg0
        ofLogError(__func__) << "videoID_: " << videoID_ << " url_: " << url_;
    }
    */
    return success;
}

void YouTubeVideoURL::print()
{
    ofLogVerbose(__func__) << toString();
}

string YouTubeVideoURL::toString()
{
    stringstream info;
    info << "videoID: " << videoID << "\n";
    info << "url: " << url << "\n";
    info << "localFilePath: " << localFilePath << "\n";
    info << "itag: " << itag << "\n";
    
    info << "valueMap key/values START \n";
    for(size_t i=0; i<valueMapNames.size(); i++)
    {
        info << valueMapNames[i] << " : " << valueMap[valueMapNames[i]] << "\n";
    }
    return info.str();
}