#include "ofxYouTubeVideoUtils.h"

ofxYouTubeVideoUtils::ofxYouTubeVideoUtils()
{
    createKnownFormats();
}



YouTubeVideoInfo ofxYouTubeVideoUtils::loadVideoInfo(string youTubeVideoID)
{
    ofLogVerbose() << "LOADING youTubeVideoID: " << youTubeVideoID;
    YouTubeVideoInfo videoInfo;
    videoInfo.fetchInfo(youTubeVideoID);
    infoCollection[youTubeVideoID] = videoInfo;
    return videoInfo;
}

void ofxYouTubeVideoUtils::printKeyValueMap(string youTubeVideoID)
{
    infoCollection[youTubeVideoID].printKeyValueMap();
}


void ofxYouTubeVideoUtils::createVideoFormat(int itag, string fileExtension, int width, int height, string audioCodec, string videoCodec, float fps)
{
    YouTubeVideoFormat format;
    format.setup(itag, fileExtension, width, height, audioCodec, videoCodec, fps);
    videoFormats[itag] = format;
}

string isRedirect(string& url)
{
    ofHttpResponse response = ofLoadURL(url);
    
    if(response.status == 302)
    {
        ofLogVerbose(__func__) << "302 !!!!!!!!!! \n response.data.getText: " << response.data.getText();
        ofLogVerbose() << "response.request.url: \n\n\n" << response.request.url;
        
        ofLogVerbose() << "url: \n\n\n" << url;
    }
    return url;
}

bool ofxYouTubeVideoUtils::downloadVideo(YouTubeVideoURL videoURL, bool doAsync)
{
    bool success = false;
    stringstream info;
    
    string fileName = videoURL.videoID + "_" + ofToString(videoURL.itag) + ".mp4";
    fileName = ofToDataPath(fileName, true);
    ofFile file(fileName);
    if(file.exists())
    {
        info << "fileName: " << fileName << " EXISTS - NOT OVERWRITING";
        success = true;
    }else
    {
        
        
        int response = -1;
        
        info << "DOWNLOADING TO: " << fileName;
        info << " ASYNC: " << doAsync;
        if (doAsync)
        {
            ofRegisterURLNotification(this);
            YouTubeDownloadRequest downloadRequest;
            downloadRequest.url = videoURL.url;
            downloadRequest.videoID = videoURL.videoID;
            downloadRequest.filePath = fileName;
            
            downloadRequests.push_back(downloadRequest);
            //isRedirect(downloadRequest.url);
            
            int queueID = ofSaveURLAsync(downloadRequest.url, fileName);
            info << " queueID: " << queueID;
            success = true;
        }else
        {
            ofHttpResponse httpResponse = ofSaveURLTo(videoURL.url, fileName);
            info << " response: " << httpResponse.status;
            if(httpResponse.status == 302)
            {
                ofLogVerbose(__func__) << "302 !!!!!!!!!! \n response.data.getText: " << httpResponse.data.getText();
                ofLogVerbose() << "response.request.url: \n\n\n" << httpResponse.request.url;
                
                ofLogVerbose() << "url: \n\n\n" << videoURL.url;
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
        }
        
    }
    ofLogVerbose(__func__) << info.str();
    
    return success;
}

void ofxYouTubeVideoUtils::urlResponse(ofHttpResponse& response)
{
    //ofLogVerbose(__func__) << "response.request.url: " << response.request.url;
    
    for (size_t i=0; i<downloadRequests.size(); i++)
    {
        if(response.request.url == downloadRequests[i].url)
        {
            if(response.status>0 && response.status != 302)
            {
                if(listener)
                {
                    YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                    listener->onYouTubeDownloadEventComplete(eventData);
                }
            }else
            {
                if(listener)
                {
                    YouTubeDownloadEventData eventData(downloadRequests[i], response, (void *)this);
                    listener->onYouTubeDownloadEventError(eventData);
                }
            }
            
        }
    }
}

#if 0
enum YouTubeFormatType
{
    WIDTH_UNKNOWN,
    HEIGHT_UNKNOWN,
    MP4,
    3GP,
    AUDIOCODEC_NONE,
    VIDEOCODEC_H264
};
#endif
void ofxYouTubeVideoUtils::createKnownFormats()
{
    int WIDTH_UNKNOWN=0;
    string NO_AUDIO="";
    string NO_VIDEO="";
    string H264="";
    string MPEG4="";
    string VP9="";
    string FLASH="";
    
    string AAC="";
    string MPEG_AUDIO = "";
    string WEBM_AUDIO = "";
    
    createVideoFormat(5, "flv",     426,    240,    MPEG_AUDIO, FLASH);//checked
    createVideoFormat(13, "3gp");
    createVideoFormat(17, "3gp",    176,    144,    AAC,        MPEG4); //checked
    createVideoFormat(18, "mp4",    640,    360,    AAC,        H264);  //checked
    createVideoFormat(22, "mp4",    1280,   720,    AAC,        H264);  //checked
    createVideoFormat(36, "3gp",    320,    240,    AAC,        MPEG4); //checked
    createVideoFormat(37, "mp4",    1920,   1080);
    createVideoFormat(38, "mp4",    4096,   3072);
    //probably 3D
    createVideoFormat(82, "mp4",    640,    360,    AAC,        H264);  //3d, checked
    createVideoFormat(83, "mp4",    640,    480);
    createVideoFormat(84, "mp4",    1280,   720, AAC);//3d
    createVideoFormat(85, "mp4",    1920,   1080);
    //probably WEBM
    createVideoFormat(92, "mp4",    320,    240);
    createVideoFormat(93, "mp4",    480,    360);
    createVideoFormat(94, "mp4",    640,    480);
    createVideoFormat(95, "mp4",    1280,   720);
    createVideoFormat(96, "mp4",    1920,   1080);
    
    createVideoFormat(160, "mp4",   256,    144,    NO_AUDIO,   H264); //checked
    createVideoFormat(151, "mp4",   128,    72);
    createVideoFormat(132, "mp4", WIDTH_UNKNOWN, 240);
    createVideoFormat(133, "mp4",   426,    240,    NO_AUDIO,   H264); //checked
    createVideoFormat(134, "mp4",   640,    360,    NO_AUDIO,   H264); //checked
    createVideoFormat(135, "mp4",   854,    480,    NO_AUDIO,   H264); //checked
    createVideoFormat(136, "mp4",   1280,   720,    NO_AUDIO,   H264); //checked
    createVideoFormat(137, "mp4",   1920,   1080,   NO_AUDIO,   H264); //checked
    createVideoFormat(140, "mp4",   0,      0,      AAC,        NO_VIDEO); //checked
    createVideoFormat(171, "mp4",   0,      0,      WEBM_AUDIO, NO_VIDEO); //checked
    
    
    createVideoFormat(264, "mp4",   2560,   1440,   NO_AUDIO,   H264); //checked
    createVideoFormat(242, "VP9",   426,    240,    NO_AUDIO,   VP9); //checked
    createVideoFormat(298, "mp4",   1280,   720,    NO_AUDIO,   H264, 60.0); //checked
    createVideoFormat(299, "mp4",   1920,   1080,   NO_AUDIO,   H264, 60.0); //checked
    createVideoFormat(266, "mp4", WIDTH_UNKNOWN, 2160, "none");
}
/*
Non-DASH
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+----+
| itag | DefaultContainer | VideoResolution | VideoEncoding | VideoProfile | VideoBitrateMbits | AudioEncoding | AudioBitratekbits |    |
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+----+
|    5 | FLV              | 240P            | Sorenson      | H263         | NA                | 0.25          | MP3               | 64 |
|    6 | FLV              | 270P            | Sorenson      | H263         | NA                | 0.8           | MP3               | 64 |
|   13 | 3GP              | NA              | MPEG4         | Visual       | NA                | 0.5           | AAC               | NA |
|   17 | 3GP              | 144p            | MPEG4         | Visual       | Simple            | 0.05          | AAC               | 24 |
|   18 | MP4              | 360P            | H264          | Baseline     | 0.5               | AAC           | 96                |    |
|   22 | MP4              | 720P            | H264          | High         | 2-3               | AAC           | 192               |    |
|   34 | FLV              | 360P            | H264          | Main         | 0.5               | AAC           | 128               |    |
|   35 | FLV              | 480P            | H264          | Main         | 0.8-1             | AAC           | 128               |    |
|   36 | 3GP              | 240P            | MPEG-4        | Visual       | Simple            | 0.175         | AAC               | 32 |
|   37 | MP4              | 1080P           | H264          | High         | 3–5.9             | AAC           | 192               |    |
|   38 | MP4              | 3072p           | H264          | High         | 3.5-5             | AAC           | 192               |    |
|   43 | WebM             | 360P            | VP8           | NA           | 0.5               | Vorbis        | 128               |    |
|   44 | WebM             | 480P            | VP8           | NA           | 1                 | Vorbis        | 128               |    |
|   45 | WebM             | 720P            | VP8           | NA           | 2                 | Vorbis        | 192               |    |
|   46 | WebM             | 1080P           | VP8           | NA           | NA                | Vorbis        | 192               |    |
|   82 | MP4              | 360P            | H264          | 3D           | 0.5               | AAC           | 96                |    |
|   83 | MP4              | 240P            | H264          | 3D           | 0.5               | AAC           | 96                |    |
|   84 | MP4              | 720P            | H264          | 3D           | 2-3               | AAC           | 192               |    |
|   85 | MP4              | 1080P           | H264          | 3D           | 3-4               | AAC           | 192               |    |
|  100 | WebM             | 360P            | VP8           | 3D           | NA                | Vorbis        | 128               |    |
|  101 | WebM             | 360P            | VP8           | 3D           | NA                | Vorbis        | 192               |    |
|  102 | WebM             | 720P            | VP8           | 3D           | NA                | Vorbis        | 192               |    |
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+----+

DASH (video only)
+------+------------------+-----------------+---------------+--------------+-------------------+
| itag | DefaultContainer | VideoResolution | VideoEncoding | VideoProfile | VideoBitrateMbits |
+------+------------------+-----------------+---------------+--------------+-------------------+
|  133 | MP4              | 240p            | H264          | Main         | 0.2-0.3           |
|  134 | MP4              | 360p            | H264          | Main         | 0.3-0.4           |
|  135 | MP4              | 480p            | H264          | Main         | 0.5-1             |
|  136 | MP4              | 720p            | H264          | Main         | 1-1.5             |
|  137 | MP4              | 1080p           | H264          | High         | 2.5-3             |
|  138 | MP4              | 2160p           | H264          | High         | 13.5              |
|  160 | MP4              | 144p            | H264          | Main         | 0.1               |
|  242 | WebM             | 240p            | VP9           | NA           | 0.1-0.2           |
|  243 | WebM             | 360p            | VP9           | NA           | 0.25              |
|  244 | WebM             | 480p            | VP9           | NA           | 0.5               |
|  247 | WebM             | 720p            | VP9           | NA           | 0.7-0.8           |
|  248 | WebM             | 1080p           | VP9           | NA           | 1.5               |
|  264 | MP4              | 1440p           | H264          | High         | 4-4.5             |
|  266 | MP4              | 2160p           | H264          | High         | 12.5-13.5         |
|  271 | WebM             | 1440p           | VP9           | NA           | 9                 |
|  272 | WebM             | 2160p           | VP9           | NA           | 15-17.5           |
|  278 | WebM             | 144p            | VP9           | NA           | 0.08              |
|  298 | MP4              | 720p_HFR        | H264          | Main         | 3-3.5             |
|  299 | MP4              | 1080p_HFR       | H264          | High         | 5.5               |
|  302 | WebM             | 720p_HFR        | VP9           | NA           | 2.5               |
|  303 | WebM             | 1080p_HFR       | VP9           | NA           | 5                 |
|  313 | WebM             | 2160p           | VP9           | NA           | 13-15             |
+------+------------------+-----------------+---------------+--------------+-------------------+

DASH (audio only)
+------+------------------+---------------+-------------------+
| itag | DefaultContainer | AudioEncoding | AudioBitratekbits |
+------+------------------+---------------+-------------------+
|  139 | MP4              | AAC           |                48 |
|  140 | MP4              | AAC           |               128 |
|  141 | MP4              | AAC           |               256 |
|  171 | WebM             | Vorbis        |               128 |
|  172 | WebM             | Vorbis        |               192 |
+------+------------------+---------------+-------------------+

Live streaming
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+
| itag | DefaultContainer | VideoResolution | VideoEncoding | VideoProfile | VideoBitrateMbits | AudioEncoding | AudioBitratekbits |
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+
|   92 | TS               | 240p            | H264          | Main         | 0.15-0.3          | AAC           |                48 |
|   93 | TS               | 360p            | H264          | Main         | 0.5-1             | AAC           |               128 |
|   94 | TS               | 480p            | H264          | Main         | 0.8-1.25          | AAC           |               128 |
|   95 | TS               | 720p            | H264          | Main         | 1.5-3             | AAC           |               256 |
|   96 | TS               | 1080p           | H264          | High         | 2.5-6             | AAC           |               256 |
|  120 | FLV              | 720p            | H264          | Main@L3.1    | 2                 | AAC           |               128 |
|  127 | TS               | NA              | NA            | NA           | NA                | AAC           |                96 |
|  128 | TS               | NA              | NA            | NA           | NA                | AAC           |                96 |
|  132 | TS               | 240p            | H264          | Baseline     | 0.15-0.2          | AAC           |                48 |
|  151 | TS               | 72p             | H264          | Baseline     | 0.05              | AAC           |                24 |
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+
 
*/


