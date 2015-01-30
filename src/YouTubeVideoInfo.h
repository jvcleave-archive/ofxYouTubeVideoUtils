#pragma once
#include "ofMain.h"
#include "Poco/URI.h"

class YouTubeVideoURL
{
public:
    string videoID;
    string url;
    int itag;
    map<string, string> valueMap;
    vector<string> valueMapNames;
    
    YouTubeVideoURL()
    {
        videoID = "";
        url = "";
        itag = -1;
    }
    
    void setup(string videoID_, string url_)
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
    
    ofHttpResponse httpResponse;
    
    vector <string> urlParams;
    
    
    vector <string> keys;
    vector <string> values;
    
    vector<YouTubeVideoURL> videoURLs;
    vector<string> imageURLs;
    vector <int> itags;
    vector<string> urls;
    
    string API_URL;
    bool hasLoaded;
    YouTubeVideoInfo()
    {
        videoID = "";
        title = "";
        author = "";
        smallImagePath = "";
        mediumImagePath = "";
        largeImagePath = "";
        largestImagePath = "";
        
        lengthInSeconds = 0;
        API_URL = "http://www.youtube.com/get_video_info?&video_id=";
        hasLoaded = false;
    };
    
    
    bool fetchInfo(string videoID_)
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
                string redecodedURL;
                Poco::URI::decode(urls[i], redecodedURL);
                youtubeVideoURL.setup(videoID, redecodedURL);
                if(youtubeVideoURL.itag != -1)
                {
                    videoURLs.push_back(youtubeVideoURL);
                }else
                {
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
    
    
    vector<YouTubeVideoURL> getPreferredFormats(vector<int> itags)
    {
        vector<YouTubeVideoURL> preferredURLs;
        vector<int> availableTags;
        
        stringstream tagLog;
        
        for(size_t j=0; j<itags.size(); j++)
        {
            tagLog << itags[j] << " ";
        }
        
        for(size_t i=0; i<videoURLs.size(); i++)
        {
            availableTags.push_back(videoURLs[i].itag);
            
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
            stringstream info;
            info << "NO PREFFERED TAGS " << tagLog.str() << " AVAILABLE FOR title: " << title << " id: "<< videoID << " AVAILABLE FORMATS ARE: \n";
            for(size_t i=0; i<availableTags.size(); i++)
            {
                info << availableTags[i] << "\n";
            }
            if(availableTags.empty())
            {
                //printKeyValueMap();
            }
            ofLogVerbose(__func__) << info.str();
        }
        return preferredURLs;
    }
    
    void printKeyValues()
    {
        stringstream ss;
        
        for(size_t i=0; i<keys.size(); i++)
        {
            if(keys[i] == "url")
            {
                string decodedURL;
                Poco::URI::decode(values[i], decodedURL) ;
                values[i] = decodedURL;
            }
            ss << keys[i] << " : " << values[i] << "\n";
            
        }
        
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
        
        ofLogVerbose(__func__) << ss.str();
        
       /* ofLogVerbose(__func__) << ss.str();
        ofBuffer buffer(ss.str());
        bool didWriteFile = ofBufferToFile(ofToDataPath(videoID+".log", true), buffer);
        ofLogVerbose(__func__) << "didWriteFile: " << didWriteFile;*/
        
        for(size_t i=0; i<itags.size(); i++)
        {
            ofLogVerbose(__func__)<< " itag : " << itags[i];
        }
        
    }
  
    
};