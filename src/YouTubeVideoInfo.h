#pragma once
#include "ofMain.h"
#include "Poco/URI.h"
#include "YouTubeVideo.h"

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
    
    vector<YouTubeVideo> videos;
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
    
    
    bool load(string videoID)
    {
        this->videoID = videoID;
        
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
                        itags.push_back(ofToInt(value));
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
                YouTubeVideo youtubeVideo;
                youtubeVideo.videoID = videoID;
                youtubeVideo.title = title;
                youtubeVideo.author = author;
                youtubeVideo.setup(urls[i]);
                videos.push_back(youtubeVideo);
            }
            
            wasSuccessful = true;
        }else
        {
            ofLogError() << "COULD NOT LOAD URL \n" << url << "\n" << "httpResponse.status: " << httpResponse.status;
        }
        
        return wasSuccessful;
    }
    
    vector<YouTubeVideo> getOptimizedVideos(vector<int> itags)
    {
        vector<YouTubeVideo> optimizedVideos;
        
        for(size_t i=0; i<videos.size(); i++)
        {
            for(size_t j=0; j<itags.size(); j++)
            {
                int currentTag = itags[j];
                if(videos[i].itag == currentTag)
                {
                    optimizedVideos.push_back(videos[i]);
                    break;
                }
            }
        }
        
        return optimizedVideos;
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