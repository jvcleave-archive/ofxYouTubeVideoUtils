#include "YouTubeVideoInfo.h"

string YouTubeVideoInfo::API_URL = "http://www.youtube.com/get_video_info?&video_id=";

YouTubeVideoInfo::YouTubeVideoInfo()
{
    videoID = "";
    title = "";
    author = "";
    smallImagePath = "";
    mediumImagePath = "";
    largeImagePath = "";
    largestImagePath = "";
    lengthInSeconds = 0;
    hasLoaded = false;
};


bool YouTubeVideoInfo::fetchInfo(string videoID_)
{
    videoID = videoID_;
    
    bool wasSuccessful = false;
    string url = API_URL+videoID;
    
    httpResponse = ofLoadURL(url);
    if(httpResponse.status > 0)
    {
        string decoded;
        Poco::URI::decode (httpResponse.data.getText(), decoded);
        
        //ofLogVerbose(__func__) << "decoded: \n" << decoded << "\n\n\n\n\n\n";
        urlParams = ofSplitString(decoded, "&");
        
        for(size_t i=0; i<urlParams.size(); i++)
        {
            string& currentParam = urlParams[i];
            
            if (currentParam.find("=") != string::npos)
            {
                int firstEqualSignPosition = currentParam.find("=", 1);
                string key = currentParam.substr(0, firstEqualSignPosition);
                string value = currentParam.substr(firstEqualSignPosition+1, currentParam.length());
                
                
                
                if(key == "url_encoded_fmt_stream_map")//has multiple equal signs so skip over this key
                {
                    continue;
                }
                
                if(key == "url")
                {
                    
                    if (value.find(",") != string::npos)
                    {
                        value = value.substr(0, value.find(",", 1));
                    }
                    
                    string decodedURL;
                    Poco::URI::decode(value, decodedURL);
                    value = decodedURL;
                    urls.push_back(decodedURL);
                }
                
                if(key == "itag")
                {
                    int intTag = ofToInt(value);
                    ofLogVerbose(__func__) << "intTag: " << intTag;
                    itags.push_back(intTag);
                    
                }
                
                if(key == "title")
                {
                    title = value;
                    ofStringReplace(title, "+", " ");
                }
                
                if(key == "author")
                {
                    author = value;
                    ofStringReplace(author, "+", " ");
                }
                if(key == "thumbnail_url")
                {
                    smallImagePath = value;
                    imageURLs.push_back(value);
                }
                if(key == "iurlmq")
                {
                    mediumImagePath = value;
                    imageURLs.push_back(value);
                }
                if(key == "iurlsd")
                {
                    largeImagePath = value;
                    imageURLs.push_back(value);
                }
                
                if(key == "iurlmaxres")
                {
                    largestImagePath = value;
                    imageURLs.push_back(value);
                }
                
                
                
                if(key == "length_seconds")
                {
                    lengthInSeconds = ofToInt(value);
                }
                
                keys.push_back(key);
                values.push_back(value);
            }
            
        }
        for(size_t i=0; i<urls.size(); i++)
        {
            //ofLogVerbose(__func__) << i <<  " url: " << urls[i];
            YouTubeVideoURL youtubeVideoURL;
            youtubeVideoURL.setup(videoID, urls[i]);
            if(youtubeVideoURL.itag != -1)
            {
                ofLogVerbose(__func__) << "youtubeVideoURL.format.itag: " << youtubeVideoURL.format.itag;
                videoURLs.push_back(youtubeVideoURL);
            }else
            {
#if 0
                string decodedRejectedURL;
                Poco::URI::decode(urls[i], decodedRejectedURL);
                ofLogVerbose(__func__) << "urls REJECTED: " << decodedRejectedURL;
                
                vector <string> decodedRejectedURL_params = ofSplitString(decodedRejectedURL, "&");
                for(size_t k=0; k<decodedRejectedURL_params.size(); k++)
                {
                    string& currentParam = decodedRejectedURL_params[k];
                    vector <string> rejectkeyValues = ofSplitString(currentParam, "=");
                    if(rejectkeyValues.size()>=2)
                    {
                        if(rejectkeyValues[0] == "itag")
                        {
                            int rejected_itag = ofToInt(rejectkeyValues[1]);
                        }
                        //valueMap[keyValues[0]] = keyValues[1];
                        //valueMapNames.push_back(keyValues[0]);
                    }
                }
#endif
            }
            
        }
        
        wasSuccessful = true;
    }else
    {
        ofLogError() << "COULD NOT LOAD URL \n" << url << "\n" << "httpResponse.status: " << httpResponse.status;
    }
    hasLoaded = wasSuccessful;
    return wasSuccessful;
}

vector<YouTubeVideoURL> YouTubeVideoInfo::getPreferredFormats(vector<YouTubeFormat>& preferredFormats)
{
    vector<YouTubeVideoURL> preferredURLs;
    
    for(size_t i=0; i<videoURLs.size(); i++)
    {
        for(size_t j=0; j<preferredFormats.size(); j++)
        {
            if(videoURLs[i].format.itag == preferredFormats[j].itag)
            {
                preferredURLs.push_back(videoURLs[i]);
            }
        }
    }
    
    if(preferredURLs.empty())
    {
        stringstream info;
        
        vector<YouTubeFormat> availableFomats;
        for(size_t i=0; i<videoURLs.size(); i++)
        {
            availableFomats.push_back(videoURLs[i].format);
        }
        
        if(availableFomats.empty())
        {
            //printKeyValues();
        }else
        {
            for(size_t i=0; i<availableFomats.size(); i++)
            {
                info << "NO PREFERRED FORMATS  - AVAILABLE ARE: \n" << availableFomats[i].toString() << "\n";
            }
            
        }
        ofLogVerbose(__func__) << info.str();
    }
    return preferredURLs;
}

vector<YouTubeVideoURL> YouTubeVideoInfo::getPreferredFormats(vector<int>& itags)
{
    vector<YouTubeVideoURL> preferredURLs;
    
    for(size_t i=0; i<videoURLs.size(); i++)
    {
        for(size_t j=0; j<itags.size(); j++)
        {
            if(videoURLs[i].itag == itags[j])
            {
                preferredURLs.push_back(videoURLs[i]);
            }
        }
    }
    
    if(preferredURLs.empty())
    {
        
        stringstream tagLog;
        for(size_t i=0; i<itags.size(); i++)
        {
            tagLog << itags[i] << " ";
        }
        
        vector<int> availableTags;
        
        for(size_t i=0; i<videoURLs.size(); i++)
        {
            availableTags.push_back(videoURLs[i].itag);
        }
        
        stringstream info;
        for(size_t i=0; i<availableTags.size(); i++)
        {
            info << availableTags[i] << "\n";
        }
        if(availableTags.empty())
        {
            ofLogError(__func__) << "NO TAGS FOR videoID: " << videoID;
        }else
        {
            info << "NO PREFFERED TAGS " << tagLog.str() << " AVAILABLE FOR title: " << title << " id: "<< videoID << " AVAILABLE FORMATS ARE: \n";
        }
        
        ofLogVerbose(__func__) << info.str();
    }
    return preferredURLs;
}


string YouTubeVideoInfo::getLargestImagePathAvailable()
{
    if(largestImagePath != "") return largestImagePath;
    if(largeImagePath != "") return largeImagePath;
    if(mediumImagePath != "") return mediumImagePath;
    if(smallImagePath != "") return smallImagePath;
    return "";
}

void YouTubeVideoInfo::print()
{
    ofLogVerbose(__func__) << toString();
}

void YouTubeVideoInfo::writeToLog(string logPath)
{
   
    if(logPath.empty())
    {
        logPath = videoID+".log";
    }
    stringstream ss;
    
    ss << "\n URL SECTION \n";
    for(size_t i=0; i<keys.size(); i++)
    {
        if(keys[i] == "url")
        {
            ss << values[i] << "\n";
        }
    }
    ss << "\n META SECTION \n";
    
    ss << "author:" << author << "\n";
    ss << "title:" << title << "\n";
    
    for(size_t i=0; i<itags.size(); i++)
    {
        ofLogVerbose(__func__)<< " itag : " << itags[i];
    }
    string info = toString();
    info += ss.str();
    ofBuffer buffer(info);
    ofBufferToFile(ofToDataPath(logPath, true), buffer);
}

string YouTubeVideoInfo::toString()
{
    stringstream ss;
    
    for(size_t i=0; i<keys.size(); i++)
    {
        ss << keys[i] << " : " << values[i] << "\n";
    }
}
