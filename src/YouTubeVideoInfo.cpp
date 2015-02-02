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
    isAvailable = false;
    failReason = "";
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
                if(key.find("url") != string::npos && key.find("=") != string::npos && key.find(",") != string::npos)
                {
                    
                    ofLogVerbose() << "damn.";
                }
                if(value.find("url") != string::npos)
                {
                    if(value.find("=") != string::npos)
                    {
                        if (value.find(",") != string::npos)
                        {
                            ofLogVerbose() << "silent damn.";
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
                                    ofLogVerbose() << "key: " << key << " value: " << value;
                                   
                                    
                                    //vector<string> pieces = ofSplitString(value, ",");
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
                if(key == "fallback_host")
                {
                    fallback_hosts.push_back(value);
                }
                
                
                if(key == "length_seconds")
                {
                    lengthInSeconds = ofToInt(value);
                }
                if(key == "status")
                {
                    if(value == "fail")
                    {
                        ofLogVerbose(__func__) << "VIDEO WILL BE UNAVAILABLE";
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
            //ofLogVerbose(__func__) << i <<  " url: " << urls[i];
            YouTubeVideoURL youtubeVideoURL;
            bool didSetup = youtubeVideoURL.setup(videoID, urls[i]);
            if(didSetup)
            {
                ofLogVerbose(__func__) << "videoID: " << videoID << " youtubeVideoURL.format.itag: " << youtubeVideoURL.format.itag;
                videoURLs.push_back(youtubeVideoURL);
                formats.push_back(youtubeVideoURL.format);
            }else
            {
                ofLogError(__func__) << videoID <<" URL FAILED: url" << youtubeVideoURL.url;
            }
        }
        if(videoURLs.empty())
        {
            ofLogVerbose() << "EMPTY VIDEO URLS";
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
        }
        wasSuccessful = true;
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
        
        if(formats.empty())
        {
            ofLogError(__func__) << "NO FORMATS AVAILABLE FOR title: " << title << " videoID: " << videoID;
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
                info << "FORMATS " << preferredFormatsList.str() << "NOT AVAILABLE FOR title: " << title << " videoID: " << videoID;
                info << " AVAILABLE ARE: \n" << formats[i].toString() << "\n";
            }
            
        }
        ofLogVerbose(__func__) << info.str();
    }
    return preferredURLs;
}

vector<YouTubeVideoURL> YouTubeVideoInfo::getPreferredFormats(vector<int>& preferredTags)
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
            ofLogVerbose(__func__) << "NO TAGS FOR title: " << title << " videoID: "<< videoID;
        }else
        {
            ofLogVerbose(__func__) << "NO PREFFERED TAGS " << preferredTagLog.str() << " AVAILABLE FOR title: " << title << " videoID: "<< videoID << " AVAILABLE FORMATS ARE: \n" << itagLog.str();
        }
    }
    return preferredURLs;
}

YouTubeFormat YouTubeVideoInfo::getFormatForLargestResolutionForStreamTypeAndContainer(YouTubeFormat::STREAM streamType, YouTubeFormat::CONTAINER container)
{
    int highestResolution = 0;
    YouTubeFormat bestFormat;
    for(size_t i=0; i<formats.size(); i++)
    {
        YouTubeFormat& currentFormat = formats[i];
        if(currentFormat.streamType == streamType &&
           currentFormat.container == container)
        {
            
            if(highestResolution<currentFormat.videoResolution)
            {
                highestResolution = currentFormat.videoResolution;
                bestFormat = currentFormat;
            }
        }
    }
    
    if(bestFormat.itag == 0)
    {
        ofLogVerbose(__func__) << "NOTHING FOUND FOR SEARCH CRITERIA streamType: " << YouTubeFormat::streamTypeToString(streamType) << " container: " << YouTubeFormat::containerToString(container);
        for(size_t i=0; i<formats.size(); i++)
        {
            ofLogVerbose() << formats[i].toString();
        }
        ofLogVerbose() << "-----------------------";

    }else
    {
        ofLogVerbose(__func__) << "RETURNING " << bestFormat.toString();
    }
    
    //bestFormat.print();
    return bestFormat;
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
    string info = toString();
    ofLogVerbose(__func__) << info;
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
    
    ss << "videoID:" << videoID << "\n";
    ss << "author:" << author << "\n";
    ss << "title:" << title << "\n";
    ss << "lengthInSeconds:" << lengthInSeconds << "\n";

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
    return ss.str();
}
