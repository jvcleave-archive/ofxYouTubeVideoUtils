#pragma once

#include "ofMain.h"

class YouTubeFormat
{
public:
    
    enum STREAM
    {
        STREAM_NOT_SET=0,
        STREAM_AUDIO_VIDEO,
        STREAM_VIDEO_ONLY,
        STREAM_AUDIO_ONLY,
        STREAM_LIVE
    };
    
    enum CONTAINER
    {
        CONTAINER_NOT_SET=0,
        CONTAINER_FLV,
        CONTAINER_3GP,
        CONTAINER_MP4,
        CONTAINER_WEBM,
        CONTAINER_TS
        
    };
    enum VIDEO_RESOLUTION
    {
        VIDEO_RESOLUTION_NOT_SET=0,
        VIDEO_RESOLUTION_240P=240,
        VIDEO_RESOLUTION_270P=270,
        VIDEO_RESOLUTION_144P=144,
        VIDEO_RESOLUTION_360P=360,
        VIDEO_RESOLUTION_480P=480,
        VIDEO_RESOLUTION_720P=720,
        VIDEO_RESOLUTION_720P_HFR=72060,
        VIDEO_RESOLUTION_1080P=1080,
        VIDEO_RESOLUTION_1080P_HFR=108060,
        VIDEO_RESOLUTION_1440P=1440,
        VIDEO_RESOLUTION_2160P=2160,
        VIDEO_RESOLUTION_3072P=3072,
        VIDEO_RESOLUTION_72P=72
    };
    enum VIDEO_ENCODING
    {
        VIDEO_ENCODING_NOT_SET=0,
        VIDEO_ENCODING_H263,
        VIDEO_ENCODING_MPEG4,
        VIDEO_ENCODING_H264,
        VIDEO_ENCODING_VP8,
        VIDEO_ENCODING_VP9
    };
    
    enum VIDEO_PROFILE
    {
        VIDEO_PROFILE_NOT_SET=0,
        VIDEO_PROFILE_SIMPLE,
        VIDEO_PROFILE_BASELINE,
        VIDEO_PROFILE_HIGH,
        VIDEO_PROFILE_MAIN,
        VIDEO_PROFILE_3D
    };
    
    enum AUDIO_ENCODING
    {
        AUDIO_ENCODING_NOT_SET=0,
        AUDIO_ENCODING_MP3,
        AUDIO_ENCODING_AAC,
        AUDIO_ENCODING_VORBIS
    };
    STREAM streamType;
    int itag;
    CONTAINER  container;
    string  fileExtension;
    VIDEO_RESOLUTION  videoResolution;
    VIDEO_ENCODING  videoEncoding;
    VIDEO_PROFILE  videoProfile;
    float   videoBitrateMbits;
    int     audioBitratekbits;
    AUDIO_ENCODING  audioEncoding;
    
    
    YouTubeFormat();
    YouTubeFormat(STREAM streamType_,
                  int itag_,
                  CONTAINER container_,
                  VIDEO_RESOLUTION videoResolution_,
                  VIDEO_ENCODING videoEncoding_,
                  VIDEO_PROFILE videoProfile_,
                  float videoBitrateMbits_,
                  AUDIO_ENCODING audioEncoding_,
                  int audioBitratekbits_);
    
    void createFormats(vector<YouTubeFormat>& formats);
    
    void print();
    string toString();
    
    static string streamTypeToString(STREAM);
    static string containerToString(CONTAINER);
    static string videoResolutionToString(VIDEO_RESOLUTION);
    static string videoEncodingToString(VIDEO_ENCODING);
    static string videoProfileToString(VIDEO_PROFILE);
    static string audioEncodingToString(AUDIO_ENCODING);
    string getFileExtension();
    

};

