#include "ofxYouTubeVideoUtils.h"


ofxYouTubeVideoUtils::ofxYouTubeVideoUtils()
{
    createKnownFormats();
}



YouTubeVideoInfo ofxYouTubeVideoUtils::loadVideoInfo(string youTubeVideoID)
{
    ofLogVerbose() << "LOADING youTubeVideoID: " << youTubeVideoID;
    YouTubeVideoInfo videoInfo;
    videoInfo.load(youTubeVideoID);
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


void ofxYouTubeVideoUtils::createKnownFormats()
{
    int WIDTH_UNKNOWN=0;
    
    createVideoFormat(13, "3gp");
    createVideoFormat(17, "3gp", 176, 144);
    createVideoFormat(18, "mp4", 640, 360);
    createVideoFormat(22, "mp4", 1280, 720);
    createVideoFormat(36, "3gp", 320, 240);
    createVideoFormat(37, "mp4", 1920, 1080);
    createVideoFormat(38, "mp4", 4096, 3072);
    createVideoFormat(82, "mp4", WIDTH_UNKNOWN, 360);
    createVideoFormat(83, "mp4", WIDTH_UNKNOWN, 480);
    createVideoFormat(84, "mp4", WIDTH_UNKNOWN, 720);
    createVideoFormat(85, "mp4", WIDTH_UNKNOWN, 1080);
    createVideoFormat(92, "mp4", WIDTH_UNKNOWN, 240);
    createVideoFormat(93, "mp4", WIDTH_UNKNOWN, 360);
    createVideoFormat(94, "mp4", WIDTH_UNKNOWN, 480);
    createVideoFormat(95, "mp4", WIDTH_UNKNOWN, 720);
    createVideoFormat(96, "mp4", WIDTH_UNKNOWN, 1080);
    createVideoFormat(132, "mp4", WIDTH_UNKNOWN, 240);
    createVideoFormat(151, "mp4", WIDTH_UNKNOWN, 72);
    createVideoFormat(133, "mp4", WIDTH_UNKNOWN, 240, "none");
    createVideoFormat(134, "mp4", WIDTH_UNKNOWN, 360, "none");
    createVideoFormat(135, "mp4", WIDTH_UNKNOWN, 480, "none");
    createVideoFormat(136, "mp4", WIDTH_UNKNOWN, 720, "none");
    createVideoFormat(137, "mp4", WIDTH_UNKNOWN, 1080, "none");
    createVideoFormat(160, "mp4", WIDTH_UNKNOWN, 144, "none");
    createVideoFormat(264, "mp4", WIDTH_UNKNOWN, 1440, "none");
    createVideoFormat(298, "mp4", WIDTH_UNKNOWN, 720, "none", "h264", 60.0);
    createVideoFormat(299, "mp4", WIDTH_UNKNOWN, 1080, "none", "h264", 60.0);
    createVideoFormat(266, "mp4", WIDTH_UNKNOWN, 2160, "none");
}
