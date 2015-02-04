#include "YouTubeVideo.h"

YouTubeVideoMetadata::YouTubeVideoMetadata()
{
    videoID = "";
    title = "";
    author = "";
    lengthInSeconds = 0;
    smallImagePath = "";
    mediumImagePath = "";
    largeImagePath = "";
    largestImagePath = "";
    
}

string YouTubeVideoMetadata::getLargestImagePathAvailable()
{
    if(largestImagePath != "") return largestImagePath;
    if(largeImagePath != "") return largeImagePath;
    if(mediumImagePath != "") return mediumImagePath;
    if(smallImagePath != "") return smallImagePath;
    return "";
}


void YouTubeVideoMetadata::print()
{
    ofLogVerbose(__func__) << toString();
}

string YouTubeVideoMetadata::toString()
{
    stringstream info;
    info << "videoID: " << videoID << "\n";
    info << "title: " << title << "\n";
    info << "author: " << author << "\n";
    info << "lengthInSeconds: " << lengthInSeconds << "\n";
    info << "smallImagePath: " << smallImagePath << "\n";
    info << "mediumImagePath: " << mediumImagePath << "\n";
    info << "largeImagePath: " << largeImagePath << "\n";
    info << "largestImagePath: " << largestImagePath << "\n";
    return info.str();
    
}