#include "YouTubeVideo.h"

string YouTubeVideo::API_URL = "http://www.youtube.com/get_video_info?&video_id=";

YouTubeVideo::YouTubeVideo()
{
    videoID = "";
    
    hasLoaded = false;
    isAvailable = false;
    failReason = "";
};


bool YouTubeVideo::fetchInfo(string videoID_)
{
    videoID = videoID_;
    metadata.videoID = videoID;
    
    bool wasSuccessful = false;
    string url = API_URL+videoID;
    
    httpResponse = ofLoadURL(url);
    if(httpResponse.status > 0)
    {
        string decoded;
        Poco::URI::decode (httpResponse.data.getText(), decoded);
        
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
                if(value.find("url") != string::npos)
                {
                    if(value.find("=") != string::npos)
                    {
                        if (value.find(",") != string::npos)
                        {
                            //ofLogVerbose() << "found buried URL married with another key/value pair";
                            vector<string> pieces = ofSplitString(value, ",");
                            for(size_t p=0; p<pieces.size(); p++)
                            {
                                string& currentPiece = pieces[p];
                                if(currentPiece.find("url") != string::npos && currentPiece.find("=") != string::npos)
                                {
                                    vector<string> urlKeyValues = ofSplitString(currentPiece, "=");
                                    key = urlKeyValues[0];
                                    
                                    value = urlKeyValues[1];
                                    string decodeYetAgain;
                                    Poco::URI::decode (value, decodeYetAgain);
                                    value = decodeYetAgain;
                                }
                            }
                            
                        }
                    }
                    
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
                    itags.push_back(intTag);
                    
                }
                
                if(key == "title")
                {
                    metadata.title = value;
                    ofStringReplace(metadata.title, "+", " ");
                }
                
                if(key == "author")
                {
                    metadata.author = value;
                    ofStringReplace(metadata.author, "+", " ");
                }
                if(key == "thumbnail_url")
                {
                    metadata.smallImagePath = value;
                    imageURLs.push_back(value);
                }
                if(key == "iurlmq")
                {
                    metadata.mediumImagePath = value;
                    imageURLs.push_back(value);
                }
                if(key == "iurlsd")
                {
                    metadata.largeImagePath = value;
                    imageURLs.push_back(value);
                }
                
                if(key == "iurlmaxres")
                {
                    metadata.largestImagePath = value;
                    imageURLs.push_back(value);
                }
                if(key == "fallback_host")
                {
                    fallback_hosts.push_back(value);
                }
                
                
                if(key == "length_seconds")
                {
                    metadata.lengthInSeconds = ofToInt(value);
                }
                if(key == "status")
                {
                    if(value == "fail")
                    {
                        ofLogWarning(__func__) << "FAIL WITH videoID: " << videoID << " WILL BE UNAVAILABLE";
                        isAvailable = false;
                    }
                }
                if(key == "reason")
                {
                    failReason = value;
                }
                keys.push_back(key);
                values.push_back(value);
            }
            
        }
        for(size_t i=0; i<urls.size(); i++)
        {
            YouTubeVideoURL youtubeVideoURL;
            bool didSetup = youtubeVideoURL.setup(videoID, metadata, urls[i]);
            if(didSetup)
            {
                videoURLs.push_back(youtubeVideoURL);
                formats.push_back(youtubeVideoURL.format);
            }else
            {
                if(youtubeVideoURL.url.find("www.youtube.com/video") != string::npos)
                {
                    ofLogError(__func__) << videoID << " URL FAILED: url" << youtubeVideoURL.url;
                }
                
            }
        }
        if(videoURLs.empty() && failReason.empty())
        {
            ofLogWarning(__func__) << videoID <<  " HAS NO VIDEO URLS";
            if(!fallback_hosts.empty())
            {
                /*for(size_t i=0; i<fallback_hosts.size(); i++)
                {
                    string& fallbackHost = fallback_hosts[i];
                    vector<string> fallbackHostPieces = ofSplitString(fallbackHost, ",");
                    string URLSection = fallbackHostPieces[1];
                    vector<string> urlPieces = ofSplitString(URLSection, "=");
                    string fallbackURL;
                    Poco::URI::decode(urlPieces[1], fallbackURL);
                    urls.push_back(fallbackURL);
                    
                }*/
                
            }
        }else
        {
            wasSuccessful = true;
        }
        
    }else
    {
        ofLogError() << "COULD NOT LOAD URL \n" << url << "\n" << "httpResponse.status: " << httpResponse.status;
    }
    
    hasLoaded = wasSuccessful;
    if(failReason.empty())
    {
        isAvailable = true;
    }
    return wasSuccessful;
}

vector<YouTubeVideoURL> YouTubeVideo::getPreferredFormats(vector<YouTubeFormat>& preferredFormats)
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
        
        if(formats.empty())
        {
            ofLogError(__func__) << "NO FORMATS AVAILABLE FOR title: " << metadata.title << " videoID: " << videoID;
            print();
        }else
        {
            stringstream preferredFormatsList;
            for(size_t i=0; i<preferredFormats.size(); i++)
            {
                preferredFormatsList << preferredFormats[i].itag << " ";
            }
            for(size_t i=0; i<formats.size(); i++)
            {
                info << "FORMATS " << preferredFormatsList.str() << "NOT AVAILABLE FOR title: " << metadata.title << " videoID: " << videoID;
                info << " AVAILABLE ARE: \n" << formats[i].toString() << "\n";
            }
            
        }
    }
    return preferredURLs;
}

vector<YouTubeVideoURL> YouTubeVideo::getPreferredFormats(vector<int>& preferredTags)
{
    vector<YouTubeVideoURL> preferredURLs;
    
    for(size_t i=0; i<videoURLs.size(); i++)
    {
        for(size_t j=0; j<preferredTags.size(); j++)
        {
            if(videoURLs[i].itag == preferredTags[j])
            {
                preferredURLs.push_back(videoURLs[i]);
            }
        }
    }
    
    if(preferredURLs.empty())
    {
        
        stringstream itagLog;
        for(size_t i=0; i<itags.size(); i++)
        {
            itagLog << itags[i] << " ";
        }
        
        stringstream preferredTagLog;
        for(size_t i=0; i<preferredTags.size(); i++)
        {
            preferredTagLog << preferredTags[i] << " ";
        }
        
        
        if(itags.empty())
        {
            ofLogWarning(__func__) << "NO TAGS FOR title: " << metadata.title << " videoID: "<< videoID;
        }else
        {
            ofLogWarning(__func__) << "NO PREFFERED TAGS " << preferredTagLog.str() << " AVAILABLE FOR title: " << metadata.title << " videoID: "<< videoID << " AVAILABLE FORMATS ARE: \n" << itagLog.str();
        }
    }
    return preferredURLs;
}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo()
{
    
    return getLargestResolutionVideo( -1, -1, -1 );
}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo(YouTubeFormat::STREAM streamType)
{
    return getLargestResolutionVideo( (int)streamType, -1, -1 );
}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo(YouTubeFormat::CONTAINER container)
{
    return getLargestResolutionVideo( -1, (int)container, -1 );

}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo(YouTubeFormat::VIDEO_ENCODING videoEncoding)
{
    return getLargestResolutionVideo( -1, -1, (int)videoEncoding );
}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo(YouTubeFormat::STREAM streamType,
                                                          YouTubeFormat::CONTAINER container)
{
    return getLargestResolutionVideo( (int)streamType, (int)container, -1 );
}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo(YouTubeFormat::STREAM streamType,
                                                          YouTubeFormat::CONTAINER container,
                                                          YouTubeFormat::VIDEO_ENCODING videoEncoding)
{
    return getLargestResolutionVideo( (int)streamType, (int)container, (int)videoEncoding );
}

YouTubeFormat YouTubeVideo::getLargestResolutionVideo(int streamType,
                                                          int container,
                                                          int videoEncoding)
{
    int highestResolution = 0;
    YouTubeFormat bestFormat;
    for(size_t i=0; i<formats.size(); i++)
    {
        YouTubeFormat& currentFormat = formats[i];
        
        if(highestResolution<currentFormat.videoResolution)
        {
            if((((int)currentFormat.streamType == streamType) || streamType == -1) &&
               (((int)currentFormat.videoEncoding == videoEncoding || videoEncoding == -1))  &&
               (((int)currentFormat.container == container || container == -1)))
            {
                highestResolution = currentFormat.videoResolution;
                bestFormat = currentFormat;
                
            }
            
        }
        
        
        
    }
    return bestFormat;
}

string YouTubeVideo::getLargestImagePathAvailable()
{
    return metadata.getLargestImagePathAvailable();
}

void YouTubeVideo::print()
{
    string info = toString();
    ofLogVerbose(__func__) << info;
}

void YouTubeVideo::writeToLog(string logPath)
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
    
    ss << metadata.toString() << "\n";
    
    ss << "\n iTAGS SECTION \n";
    for(size_t i=0; i<itags.size(); i++)
    {
        ss << itags[i] << "\n";
    }
    string info = toString();
    info += ss.str();
    ofBuffer buffer(info);
    ofBufferToFile(ofToDataPath(logPath, true), buffer);
}

string YouTubeVideo::toString()
{
    stringstream ss;
    
    for(size_t i=0; i<keys.size(); i++)
    {
        ss << keys[i] << " : " << values[i] << "\n";
    }
    return ss.str();
}
