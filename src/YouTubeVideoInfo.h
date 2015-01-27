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
    
    ofHttpResponse httpResponse;
    
    vector <string> urlParams;
    vector <string> keyNames;
    
    map<string, string> keyValueMap;
    vector <string> keys;
    vector <string> values;
    
    vector<YouTubeVideoURL> videoURLs;
    vector <int> itags;
    vector<string> urls;
    
    string API_URL;
 
    YouTubeVideoInfo()
    {
        videoID = "";
        title = "";
        author = "";
        API_URL = "http://www.youtube.com/get_video_info?&video_id=";
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
                        ofLogVerbose() << "intTag: " << intTag;
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
                    
                    keys.push_back(key);
                    values.push_back(value);
                    
                    keyValueMap[key] = value; //will overwrite duplicate keys but may be useful
                    keyNames.push_back(key);
                }
                
            }
            
            for(size_t i=0; i<urls.size(); i++)
            {
                //ofLogVerbose(__func__) << i <<  " url: " << urls[i];
                YouTubeVideoURL youtubeVideoURL;
                youtubeVideoURL.setup(videoID, urls[i]);
                videoURLs.push_back(youtubeVideoURL);
            }
            
            wasSuccessful = true;
        }else
        {
            ofLogError() << "COULD NOT LOAD URL \n" << url << "\n" << "httpResponse.status: " << httpResponse.status;
        }
        
        return wasSuccessful;
    }
    
    vector<YouTubeVideoURL> getPreferredFormats(vector<int> itags)
    {
        vector<YouTubeVideoURL> preferredURLs;
        vector<int> availableTags;
        for(size_t i=0; i<videoURLs.size(); i++)
        {
            for(size_t j=0; j<itags.size(); j++)
            {
                int currentTag = itags[j];
                availableTags.push_back(currentTag);
                if(videoURLs[i].itag == currentTag)
                {
                    preferredURLs.push_back(videoURLs[i]);
                }
            }
        }
        
        if(preferredURLs.empty())
        {
            stringstream info;
            info << "NO PREFFERED TAGS AVAILABLE FOR " << videoID << " AVAILABLE FORMATS ARE: \n";
            for(size_t i=0; i<availableTags.size(); i++)
            {
                info << availableTags[i] << "\n";
            }
            if(availableTags.empty())
            {
                printKeyValueMap();
            }
            ofLogVerbose() << info.str();
        }
        return preferredURLs;
    }
    
    void printKeyValueMap()
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