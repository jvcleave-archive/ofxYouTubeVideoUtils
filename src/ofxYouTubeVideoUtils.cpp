#include "ofxYouTubeVideoUtils.h"
#include "Poco/RegularExpression.h"
using Poco::RegularExpression;


ofxYouTubeVideoUtils::ofxYouTubeVideoUtils()
{
    listener = NULL;
    formatCollection = YouTubeFormatCollection::getInstance().getFormatCollection();
}

vector<string> ofxYouTubeVideoUtils::getVideoIDsFromPlaylist(string playlistID)
{
    vector<string> videoIDs;
    string url = "http://gdata.youtube.com/feeds/api/playlists/"+playlistID+"/?v=2";
    
    ofHttpResponse response = ofLoadURL(url);
    if(response.status>0)
    {
        
        RegularExpression regEx("<yt:videoid>(.+?)<\\/yt:videoid>");
        
        string haystack = response.data.getText();
        
        RegularExpression::Match match;
        string::size_type haystackOffset = 0;
        
        vector<string> needles;
        
        while (regEx.match(haystack, haystackOffset, match))
        {
            haystackOffset = match.offset + match.length;
            needles.push_back(string(haystack, match.offset, match.length));
        }
        
        for (int i = 0; i < needles.size(); ++i)
        {
            string openingTag = "<yt:videoid>";
            string closingTag = "</yt:videoid>";
            needles[i].erase(0, openingTag.length());
            int endTagPosition = needles[i].find(closingTag);
            needles[i].erase(endTagPosition, needles[i].length());
            videoIDs.push_back(needles[i]);
        }
        
    }else
    {
        ofLogVerbose() << "FAIL: " << response.status;
    }
    return videoIDs;
}

ofxYouTubeVideoUtils::~ofxYouTubeVideoUtils()
{
    ofLogVerbose(__func__) << "downloadRequests.empty: " << downloadRequests.empty();
    if(!downloadRequests.empty())
    {
        ofStopURLLoader();
        ofRemoveAllURLRequests();
    }
}

void ofxYouTubeVideoUtils::addListener(YouTubeDownloadEventListener* listener_)
{
    listener = listener_;
}

void ofxYouTubeVideoUtils::removeListener(YouTubeDownloadEventListener* listener_)
{
    if(listener_ == listener)
    {
        listener = NULL;
    }
}

YouTubeFormat ofxYouTubeVideoUtils::getFormat(int itag)
{
    return YouTubeFormatCollection::getInstance().getFormatForItag(itag);
}

void ofxYouTubeVideoUtils::findiTagsForVideoResolution(vector<YouTubeFormat>& formats, YouTubeFormat::VIDEO_RESOLUTION videoResolution)
{
    for(size_t i=0; i<formatCollection.size(); i++ )
    {
        if(formatCollection[i].videoResolution == videoResolution)
        {
            formats.push_back(formatCollection[i]);
        }else
        {
           // ofLogError(__func__) << formatCollection[i].videoResolution << " IS NOT " << videoResolution;
        }
    }
}


YouTubeVideoInfo ofxYouTubeVideoUtils::loadVideoInfo(string youTubeVideoID)
{
    ofLogVerbose(__func__) << "LOADING youTubeVideoID: " << youTubeVideoID;
    YouTubeVideoInfo videoInfo;
    videoInfo.fetchInfo(youTubeVideoID);
    infoCollection.push_back(videoInfo);
    return videoInfo;
}



string ofxYouTubeVideoUtils::createFileName(YouTubeVideoURL& videoURL,
                                            bool groupIntoFolder) //default: false
{
    //TODO Pretty format option (e.g. Title + Resolution)
    stringstream name;
    name << videoURL.videoID;
    name << "_";
    name << videoURL.itag;
    
    YouTubeFormat format = videoURL.format;
    ofLogVerbose(__func__) << "VIDEO_RESOLUTION: " << format.videoResolution;
    switch (format.container)
    {
        case YouTubeFormat::CONTAINER_3GP:  { name << ".3gp";   break;  }
        case YouTubeFormat::CONTAINER_FLV:  { name << ".flv";   break;  }
        case YouTubeFormat::CONTAINER_MP4:  { name << ".mp4";   break;  }
        case YouTubeFormat::CONTAINER_WEBM: { name << ".webm";  break;  }
        case YouTubeFormat::CONTAINER_TS:   { name << ".ts";  break;  }
        default:                            { name << ".unknownformat"; }
    }
    
    string fileName;
    if(groupIntoFolder)
    {
        ofDirectory groupFolder = ofToDataPath(videoURL.videoID, true);
        if(!groupFolder.exists())
        {
            groupFolder.create();
        }
        fileName = groupFolder.getAbsolutePath()+"/"+name.str();
    }else
    {
        fileName = ofToDataPath(name.str(), true);
    }
    
    ofLogVerbose(__func__) << "fileName: " << fileName;

    return fileName;
}



bool ofxYouTubeVideoUtils::downloadVideo(YouTubeVideoURL videoURL,
                                         bool doAsync,              //default: false
                                         bool doOverwriteExisting,  //default: false
                                         bool groupIntoFolder,      //default: false
                                         string customPath)         //default: ""  //TODO implement customPath
{
    bool success = false;
    stringstream info;
    
    string fileName = createFileName(videoURL, groupIntoFolder);
    
    ofFile file(fileName);
    
    YouTubeDownloadRequest downloadRequest;
    downloadRequest.url = videoURL.url;
    downloadRequest.videoID = videoURL.videoID;
    downloadRequest.filePath = fileName;
    if(file.exists() && file.getSize()==0)
    {
        info << "fileName: " << fileName << " EXISTS - BUT IS 0 bytes";
        doOverwriteExisting = true;
    }
    if(file.exists() && !doOverwriteExisting)
    {
        info << "fileName: " << fileName << " EXISTS - NOT OVERWRITING";
        if(listener)
        {
            YouTubeDownloadEventData eventData(downloadRequest, (void *)this, fileName+ " EXISTS - DID NOT OVERWRITE");
            broadcastDownloadEventComplete(eventData);
        }
        success = true;
    }else
    {
        
        info << "DOWNLOADING TO: " << fileName;
        info << " ASYNC: " << doAsync;
        
        if (doAsync)
        {
            ofAddListener(ofURLResponseEvent(),this,&ofxYouTubeVideoUtils::onVideoHTTPResponse);
            
            downloadRequest.isAsync = true;
            downloadRequests.push_back(downloadRequest);
            
            int queueID = ofSaveURLAsync(downloadRequest.url, fileName);
            info << " queueID: " << queueID;
            success = true;
        }else
        {
            downloadRequest.isAsync = false;
            
            ofHttpResponse httpResponse = ofSaveURLTo(videoURL.url, fileName);
            info << " response: " << httpResponse.status;
            
            if(httpResponse.status == 302)
            {
                handleRedirect(videoURL.url);
            }else
            {
                
                if(httpResponse.status>0)
                {
                    
                    success = true;
                }else
                {
                    info << " DOWNLOAD FAILED with response " << httpResponse.status;
                }
            }
            YouTubeDownloadEventData eventData(downloadRequest, httpResponse, (void *)this);
            if(success)
            {
                broadcastDownloadEventComplete(eventData);
            }else
            {
                broadcastDownloadEventError(eventData);
            }
            
        }
        
    }
    ofLogVerbose(__func__) << info.str();
    
    return success;

}

void ofxYouTubeVideoUtils::downloadAllImages(YouTubeVideoInfo& videoInfo)
{
    for(size_t i=0; i<videoInfo.imageURLs.size(); i++)
    {
        string url = videoInfo.imageURLs[i];
        vector<string> urlPieces = ofSplitString(url, "/");
        string filePath = ofToDataPath(videoInfo.videoID+ "_" +urlPieces.back(), true);
        ofSaveURLTo(url, filePath); //TODO Async option? may interfere with callback for videos
    }
}

void ofxYouTubeVideoUtils::broadcastDownloadEventComplete(YouTubeDownloadEventData& eventData)
{
    if(!listener) return;
    listener->onYouTubeDownloadEventComplete(eventData);
}

void ofxYouTubeVideoUtils::broadcastDownloadEventError(YouTubeDownloadEventData& eventData)
{
    if(!listener) return;
    listener->onYouTubeDownloadEventError(eventData);
}


void ofxYouTubeVideoUtils::onVideoHTTPResponse(ofHttpResponse& response)
{
    //ofLogVerbose(__func__) << "response.request.url: " << response.request.url;
    
    for (size_t i=0; i<downloadRequests.size(); i++)
    {
        if(response.request.url == downloadRequests[i].url)
        {
            if(response.status>0)
            {
                if(response.status != 302)
                {
                    if(listener)
                    {
                        YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                        broadcastDownloadEventComplete(eventData);
                    }
                }else
                {
                    handleRedirect(response.request.url);

                }
                
            }else
            {
                if(listener)
                {
                    YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                    broadcastDownloadEventError(eventData);
                    
                }
            }
            downloadRequests.erase(downloadRequests.begin() + i);
        }
    }
    if (downloadRequests.empty())
    {
        ofLogVerbose(__func__) << "ALL DOWNLOADS COMPLETE";
    }
}

void ofxYouTubeVideoUtils::handleRedirect(string redirectedURL)
{
    //TODO - handle redirects
    ofLogVerbose(__func__) << "got a redirect from URL: " << redirectedURL;

}

void ofxYouTubeVideoUtils::print(string videoID)
{
    for(size_t i=0; i<infoCollection.size(); i++)
    {
        if(infoCollection[i].videoID == videoID)
        {
            infoCollection[i].print();
            break;
        }
    }
}
