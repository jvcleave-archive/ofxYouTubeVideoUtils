#include "ofxYouTubeVideoUtils.h"
#include "Poco/RegularExpression.h"




bool ofxYouTubeVideoUtils::GROUP_DOWNLOADS_INTO_FOLDERS = false;
bool ofxYouTubeVideoUtils::USE_PRETTY_NAMES = false;

ofxYouTubeVideoUtils::ofxYouTubeVideoUtils()
{
    listener = NULL;
    formatCollection = YouTubeFormatCollection::getInstance().getFormatCollection();
    ofAddListener(ofURLResponseEvent(),this,&ofxYouTubeVideoUtils::onVideoHTTPResponse);
}

vector<string> ofxYouTubeVideoUtils::getVideoIDsFromPlaylist(string playlistID)
{
    vector<string> videoIDs;
    string url = "http://gdata.youtube.com/feeds/api/playlists/"+playlistID+"/?v=2";
    
    ofHttpResponse response = ofLoadURL(url);
    if(response.status>0)
    {
        
        Poco::RegularExpression regEx("<yt:videoid>(.+?)<\\/yt:videoid>");
        
        string haystack = response.data.getText();
        
        Poco::RegularExpression::Match match;
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



YouTubeFormat ofxYouTubeVideoUtils::getFormat(int itag)
{
    return YouTubeFormatCollection::getInstance().getFormatForItag(itag);
}

vector<YouTubeFormat> ofxYouTubeVideoUtils::findiTagsForVideoResolution(YouTubeFormat::VIDEO_RESOLUTION videoResolution)
{
    vector<YouTubeFormat> formats;
    for(size_t i=0; i<formatCollection.size(); i++ )
    {
        if(formatCollection[i].videoResolution == videoResolution)
        {
            formats.push_back(formatCollection[i]);
        }
    }
    return formats;
}

YouTubeVideo ofxYouTubeVideoUtils::loadVideoInfo(string youTubeVideoID)
{
    ofLogVerbose(__func__) << "LOADING youTubeVideoID: " << youTubeVideoID;
    YouTubeVideo videoInfo;
    bool didLoad = videoInfo.fetchInfo(youTubeVideoID);

    if(!didLoad || videoInfo.isAvailable == false)
    {
        failedCollection.push_back(videoInfo);
    }else
    {
        infoCollection.push_back(videoInfo);
    }
    
    return videoInfo;
}

string ofxYouTubeVideoUtils::createFileName(YouTubeVideoURL& videoURL) //default: false
{
    stringstream name;
    if(USE_PRETTY_NAMES)
    {
        name << videoURL.metadata.title;
        name << "_";
        name << videoURL.videoID;
        name << "_";
        name << videoURL.itag;
        
    }else
    {
        name << videoURL.videoID;
        name << "_";
        name << videoURL.itag;
    }

    
    YouTubeFormat& format = videoURL.format;
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
    string folderName;
    if(USE_PRETTY_NAMES)
    {
        folderName = videoURL.metadata.title;
    }else
    {
        folderName = videoURL.videoID;
    }
    if(GROUP_DOWNLOADS_INTO_FOLDERS)
    {
        ofDirectory groupFolder = ofToDataPath(folderName, true);
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
                                         string customPath)         //default: ""
{
    bool success = false;
    stringstream info;
    
    string fileName;
    
    if(customPath.empty())
    {
        fileName = createFileName(videoURL);
    }else
    {
        fileName = customPath;
    }
    
    YouTubeDownloadRequest downloadRequest;
    downloadRequest.videoURL = videoURL; //TODO - better way - packing in for redirects
    downloadRequest.url = videoURL.url;
    downloadRequest.videoID = videoURL.videoID;
    downloadRequest.filePath = fileName;
    
    ofFile file(fileName);
    if(file.exists() && file.getSize()==0)
    {
        info << "fileName: " << fileName << " EXISTS - BUT IS 0 bytes, WILL OVERWRITE";
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
            
            if(httpResponse.status > 200)
            {
                ofLogVerbose() << "httpResponse.status: " << httpResponse.status;
                //handleRedirect(httpResponse);
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

void ofxYouTubeVideoUtils::downloadAllImages(YouTubeVideo& videoInfo)
{
    for(size_t i=0; i<videoInfo.imageURLs.size(); i++)
    {
        string filePath;
        string url = videoInfo.imageURLs[i];
        
        vector<string> urlPieces = ofSplitString(url, "/");
        string title;
        
        if(USE_PRETTY_NAMES)
        {
            title = videoInfo.metadata.title;
        }else
        {
            title = videoInfo.videoID;
        }
        
        if(GROUP_DOWNLOADS_INTO_FOLDERS)
        {
            ofDirectory groupFolder = ofToDataPath(title, true);
            
            if(!groupFolder.exists())
            {
                groupFolder.create();
            }
            
            filePath = groupFolder.getAbsolutePath()+"/"+title+ "_" +urlPieces.back();
        }else
        {
            filePath = ofToDataPath(title+ "_" +urlPieces.back(), true);
        }
        
        //TODO? add to YouTubeVideoMetadata so can access already downloaded image?
        
        ofSaveURLTo(url, filePath); //TODO Async option? may interfere with callback for videos
    }
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
                if(response.status >= 200)
                {
                    if(response.status == 302)
                    {
                        ofLogVerbose(__func__) << "302 :////////////////// " << response.status;
                        handleRedirect(downloadRequests[i], response.request.url, response.request.name);
                    }
                    if(listener)
                    {
                        YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                        broadcastDownloadEventComplete(eventData);
                    }
                }else
                {
                    ofLogVerbose(__func__) << response.status;
                    //handleRedirect(response);

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

void ofxYouTubeVideoUtils::handleRedirect(YouTubeDownloadRequest downloadRequest, string redirectedURL, string filePath)
{
    //try async
    
    downloadVideo(downloadRequest.videoURL, false, true, filePath);
    
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

ofxYouTubeVideoUtils::~ofxYouTubeVideoUtils()
{
    ofRemoveListener(ofURLResponseEvent(), this, &ofxYouTubeVideoUtils::onVideoHTTPResponse);
    ofLogVerbose(__func__) << "downloadRequests.empty: " << downloadRequests.empty();
    if(!downloadRequests.empty())
    {
        ofStopURLLoader();
        ofRemoveAllURLRequests();
    }
}





#if 0

//may need for redirects

#include "Poco/Net/HTTPSession.h"
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPSClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/StreamCopier.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"
#include "Poco/URIStreamOpener.h"
#include "Poco/Net/HTTPStreamFactory.h"
#include "Poco/Net/HTTPSStreamFactory.h"
#include "Poco/Net/SSLManager.h"
#include "Poco/Net/KeyConsoleHandler.h"
#include "Poco/Net/ConsoleCertificateHandler.h"

using namespace Poco::Net;

using namespace Poco;
void ofxYouTubeVideoUtils::handleRedirect(string redirectedURL, string filePath)
{
   

    ofLogVerbose(__func__) << "redirectedURL" << redirectedURL;
    
    ofHttpRequest request = httpResponse.request;
    ofHttpResponse result;
        try {
            URI uri(request.url);
            std::string path(uri.getPathAndQuery());
            if (path.empty()) path = "/";
            
            Poco::Net::HTTPRequest req(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
            HTTPResponse res;
            ofPtr<HTTPSession> session;
            istream * rs;
            if(uri.getScheme()=="https"){
                //const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "rootcert.pem" ) );
                HTTPSClientSession * httpsSession = new HTTPSClientSession(uri.getHost(), uri.getPort());//,context);
                httpsSession->setTimeout(Poco::Timespan(20,0));
                httpsSession->sendRequest(req);
                rs = &httpsSession->receiveResponse(res);
                session = ofPtr<HTTPSession>(httpsSession);
            }else{
                HTTPClientSession * httpSession = new HTTPClientSession(uri.getHost(), uri.getPort());
                httpSession->setTimeout(Poco::Timespan(20,0));
                httpSession->sendRequest(req);
                rs = &httpSession->receiveResponse(res);
                session = ofPtr<HTTPSession>(httpSession);
            }
            if(!request.saveTo){
                result = ofHttpResponse(request,*rs,res.getStatus(),res.getReason());
            }else{
                ofFile saveTo(request.name,ofFile::WriteOnly,true);
                char aux_buffer[1024];
                rs->read(aux_buffer, 1024);
                std::streamsize n = rs->gcount();
                while (n > 0){
                    // we resize to size+1 initialized to 0 to have a 0 at the end for strings
                    saveTo.write(aux_buffer,n);
                    if (rs->good()){
                        rs->read(aux_buffer, 1024);
                        n = rs->gcount();
                    }
                    else n = 0;
                }
                result = ofHttpResponse(request,res.getStatus(),res.getReason());
            }
            
        } catch (const Exception& exc) {
            ofLogError(__func__) << "handleRequest(): "+ exc.displayText();
            
            result = ofHttpResponse(request,-1,exc.displayText());
            
        } catch (...) {
            result = ofHttpResponse(request,-1,"ofURLFileLoader: fatal error, couldn't catch Exception");
        }
        
        result = ofHttpResponse(request,-1,"ofURLFileLoader: fatal error, couldn't catch Exception");
    
    if(result.status == 302)
    {
        ofLogVerbose() << "302 AGAIN: " << result.status;
    }else
    {
        ofLogVerbose() << "FINE: " << result.status;
    }
    return result;
    

}
#endif

