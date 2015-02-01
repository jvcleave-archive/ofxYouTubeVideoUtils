#include "YouTubeFormat.h"

YouTubeFormat::YouTubeFormat()
{
    streamType = STREAM_NOT_SET;
    itag = 0;
    container = CONTAINER_NOT_SET;
    videoResolution = VIDEO_RESOLUTION_NOT_SET;
    videoEncoding = VIDEO_ENCODING_NOT_SET;
    videoProfile = VIDEO_PROFILE_NOT_SET;
    videoBitrateMbits = 0.0;
    audioBitratekbits = 0.0;
    audioEncoding = AUDIO_ENCODING_NOT_SET;
}

YouTubeFormat::YouTubeFormat(STREAM streamType_,
              int itag_,
              CONTAINER container_,
              VIDEO_RESOLUTION videoResolution_,
              VIDEO_ENCODING videoEncoding_,
              VIDEO_PROFILE videoProfile_,
              float videoBitrateMbits_,
              AUDIO_ENCODING audioEncoding_,
              int audioBitratekbits_)
{
    itag = itag_;
    streamType = streamType_;
    container = container_;
    videoResolution = videoResolution_;
    videoEncoding = videoEncoding_;
    videoProfile = videoProfile_;
    videoBitrateMbits = videoBitrateMbits_;
    audioEncoding = audioEncoding_;
    audioBitratekbits = audioBitratekbits_;
}

void YouTubeFormat::print()
{
    ofLogVerbose(__func__) << "\n" << toString();
}

string YouTubeFormat::toString()
{
    stringstream info;
    info << "itag: " << itag << "\n";
    
    switch (streamType)
    {
        case STREAM_AUDIO_VIDEO:            {info << "STREAM_AUDIO_VIDEO" << "\n";}         break;
        case STREAM_VIDEO_ONLY:             {info << "STREAM_VIDEO_ONLY" << "\n";}          break;
        case STREAM_AUDIO_ONLY:             {info << "STREAM_AUDIO_ONLY" << "\n";}          break;
        case STREAM_LIVE:                   {info << "STREAM_LIVE" << "\n";}                break;
        case STREAM_NOT_SET:                {info << "STREAM_NOT_SET" << "\n";}             break;
        default:                            {info << "UNKNOWN streamType " << "\n";}        break;
    }
    
    switch (container)
    {
        case CONTAINER_NOT_SET:             {info << "CONTAINER_NOT_SET" << "\n";}          break;
        case CONTAINER_FLV:                 {info << "CONTAINER_FLV" << "\n";}              break;
        case CONTAINER_3GP:                 {info << "CONTAINER_3GP" << "\n";}              break;
        case CONTAINER_MP4:                 {info << "CONTAINER_MP4" << "\n";}              break;
        case CONTAINER_WEBM:                {info << "CONTAINER_WEBM" << "\n";}             break;
        case CONTAINER_TS:                  {info << "CONTAINER_TS" << "\n";}               break;
        default:                            {info << "UNKNOWN container " << "\n";}         break;
    }
    
    switch (videoResolution)
    {
        case VIDEO_RESOLUTION_NOT_SET:      {info << "VIDEO_RESOLUTION_NOT_SET" << "\n";}   break;
        case VIDEO_RESOLUTION_240P:         {info << "VIDEO_RESOLUTION_240P" << "\n";}      break;
        case VIDEO_RESOLUTION_270P:         {info << "VIDEO_RESOLUTION_270P" << "\n";}      break;
        case VIDEO_RESOLUTION_144P:         {info << "VIDEO_RESOLUTION_144P" << "\n";}      break;
        case VIDEO_RESOLUTION_360P:         {info << "VIDEO_RESOLUTION_360P" << "\n";}      break;
        case VIDEO_RESOLUTION_480P:         {info << "VIDEO_RESOLUTION_480P" << "\n";}      break;
        case VIDEO_RESOLUTION_720P:         {info << "VIDEO_RESOLUTION_720P" << "\n";}      break;
        case VIDEO_RESOLUTION_720P_HFR:     {info << "VIDEO_RESOLUTION_720P_HFR" << "\n";}  break;
        case VIDEO_RESOLUTION_1080P:        {info << "VIDEO_RESOLUTION_1080P" << "\n";}     break;
        case VIDEO_RESOLUTION_1080P_HFR:    {info << "VIDEO_RESOLUTION_1080P_HFR" << "\n";} break;
        case VIDEO_RESOLUTION_1440P:        {info << "VIDEO_RESOLUTION_1440P" << "\n";}     break;
        case VIDEO_RESOLUTION_2160P:        {info << "VIDEO_RESOLUTION_2160P" << "\n";}     break;
        case VIDEO_RESOLUTION_3072P:        {info << "VIDEO_RESOLUTION_3072P" << "\n";}     break;
        case VIDEO_RESOLUTION_72P:          {info << "VIDEO_RESOLUTION_72P" << "\n";}       break;
            
        default:                            {info << "UNKNOWN videoResolution " << "\n";}   break;
    }
    
    switch (videoEncoding)
    {
        case VIDEO_ENCODING_NOT_SET:        {info << "VIDEO_ENCODING_NOT_SET" << "\n";}     break;
        case VIDEO_ENCODING_H263:           {info << "VIDEO_ENCODING_H263" << "\n";}        break;
        case VIDEO_ENCODING_MPEG4:          {info << "VIDEO_ENCODING_MPEG4" << "\n";}       break;
        case VIDEO_ENCODING_H264:           {info << "VIDEO_ENCODING_H264" << "\n";}        break;
        case VIDEO_ENCODING_VP8:            {info << "VIDEO_ENCODING_VP8" << "\n";}         break;
        case VIDEO_ENCODING_VP9:            {info << "VIDEO_ENCODING_VP9" << "\n";}         break;
        default:                            {info << "UNKNOWN videoEncoding " << "\n";}     break;
    }
    
    switch (videoProfile)
    {
        case VIDEO_PROFILE_NOT_SET:         {info << "VIDEO_PROFILE_NOT_SET" << "\n";}      break;
        case VIDEO_PROFILE_SIMPLE:          {info << "VIDEO_PROFILE_SIMPLE" << "\n";}       break;
        case VIDEO_PROFILE_BASELINE:        {info << "VIDEO_PROFILE_BASELINE" << "\n";}     break;
        case VIDEO_PROFILE_HIGH:            {info << "VIDEO_PROFILE_HIGH" << "\n";}         break;
        case VIDEO_PROFILE_MAIN:            {info << "VIDEO_PROFILE_MAIN" << "\n";}         break;
        case VIDEO_PROFILE_3D:              {info << "VIDEO_PROFILE_3D" << "\n";}           break;
        default:                            {info << "UNKNOWN videoProfile " << "\n";}      break;
    }
    
    switch (audioEncoding)
    {
        case AUDIO_ENCODING_NOT_SET:        {info << "AUDIO_ENCODING_NOT_SET" << "\n";}     break;
        case AUDIO_ENCODING_MP3:            {info << "AUDIO_ENCODING_MP3" << "\n";}         break;
        case AUDIO_ENCODING_AAC:            {info << "AUDIO_ENCODING_AAC" << "\n";}         break;
        case AUDIO_ENCODING_VORBIS:         {info << "AUDIO_ENCODING_VORBIS" << "\n";}      break;
    }
    
    return info.str();
    
    
}
void YouTubeFormat::createFormats(vector<YouTubeFormat>& formats)
{
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO,  5,  CONTAINER_FLV,  VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_H263,    VIDEO_PROFILE_NOT_SET,  0.25, AUDIO_ENCODING_MP3,     64));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO,  6,  CONTAINER_FLV,  VIDEO_RESOLUTION_270P,      VIDEO_ENCODING_H263,    VIDEO_PROFILE_NOT_SET,  0.8,  AUDIO_ENCODING_MP3,     64));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 13,  CONTAINER_3GP,  VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_MPEG4,   VIDEO_PROFILE_NOT_SET,  0.5,  AUDIO_ENCODING_AAC,     0));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 17,  CONTAINER_3GP,  VIDEO_RESOLUTION_144P,      VIDEO_ENCODING_MPEG4,   VIDEO_PROFILE_SIMPLE,   0.05, AUDIO_ENCODING_AAC,     24));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 18,  CONTAINER_MP4,  VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_BASELINE, 0.5,  AUDIO_ENCODING_AAC,     96));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 22,  CONTAINER_MP4,  VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     3,    AUDIO_ENCODING_AAC,     192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 34,  CONTAINER_FLV,  VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     0.5,  AUDIO_ENCODING_AAC,     128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 35,  CONTAINER_FLV,  VIDEO_RESOLUTION_480P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     1,    AUDIO_ENCODING_AAC,     128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 36,  CONTAINER_3GP,  VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_SIMPLE,   0.175,AUDIO_ENCODING_AAC,     32));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 37,  CONTAINER_MP4,  VIDEO_RESOLUTION_1080P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     35.9, AUDIO_ENCODING_AAC,     192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 38,  CONTAINER_MP4,  VIDEO_RESOLUTION_3072P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     5,    AUDIO_ENCODING_AAC,     192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 43,  CONTAINER_WEBM, VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_VP8,     VIDEO_PROFILE_NOT_SET,  0.5,  AUDIO_ENCODING_VORBIS,  128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 44,  CONTAINER_WEBM, VIDEO_RESOLUTION_480P,      VIDEO_ENCODING_VP8,     VIDEO_PROFILE_NOT_SET,  1,    AUDIO_ENCODING_VORBIS,  128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 45,  CONTAINER_WEBM, VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_VP8,     VIDEO_PROFILE_NOT_SET,  2,    AUDIO_ENCODING_VORBIS,  192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 46,  CONTAINER_WEBM, VIDEO_RESOLUTION_1080P,     VIDEO_ENCODING_VP8,     VIDEO_PROFILE_NOT_SET,  0.0,  AUDIO_ENCODING_VORBIS,  192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 82,  CONTAINER_MP4,  VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_3D,       0.5,  AUDIO_ENCODING_AAC,     96));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 83,  CONTAINER_MP4,  VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_3D,       0.5,  AUDIO_ENCODING_AAC,     96));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 84,  CONTAINER_MP4,  VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_3D,       3,    AUDIO_ENCODING_AAC,     192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 85,  CONTAINER_MP4,  VIDEO_RESOLUTION_1080P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_3D,       4,    AUDIO_ENCODING_AAC,     192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 100, CONTAINER_WEBM, VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_VP8,     VIDEO_PROFILE_3D,       0.0,  AUDIO_ENCODING_VORBIS,  128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 101, CONTAINER_WEBM, VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_VP8,     VIDEO_PROFILE_3D,       0.0,  AUDIO_ENCODING_VORBIS,  192));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_VIDEO, 102, CONTAINER_WEBM, VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_VP8,     VIDEO_PROFILE_3D,       0.0,  AUDIO_ENCODING_VORBIS,  192));
    
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  133, CONTAINER_MP4,  VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     0.3,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  134, CONTAINER_MP4,  VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     0.4,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  135, CONTAINER_MP4,  VIDEO_RESOLUTION_480P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     1,    AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  136, CONTAINER_MP4,  VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     1.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  137, CONTAINER_MP4,  VIDEO_RESOLUTION_1080P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     3,    AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  138, CONTAINER_MP4,  VIDEO_RESOLUTION_2160P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     13.5, AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  160, CONTAINER_MP4,  VIDEO_RESOLUTION_144P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     0.1,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  242, CONTAINER_WEBM, VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  0.2,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  243, CONTAINER_WEBM, VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  0.25, AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  244, CONTAINER_WEBM, VIDEO_RESOLUTION_480P,      VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  0.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  247, CONTAINER_WEBM, VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  0.8,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  248, CONTAINER_WEBM, VIDEO_RESOLUTION_1080P,     VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  1.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  264, CONTAINER_MP4,  VIDEO_RESOLUTION_1440P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     4.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  266, CONTAINER_MP4,  VIDEO_RESOLUTION_2160P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     13.5, AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  271, CONTAINER_WEBM, VIDEO_RESOLUTION_1440P,     VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  9,    AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  272, CONTAINER_WEBM, VIDEO_RESOLUTION_2160P,     VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  7.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  278, CONTAINER_WEBM, VIDEO_RESOLUTION_144P,      VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  0.08, AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  298, CONTAINER_MP4,  VIDEO_RESOLUTION_720P_HFR,  VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     3.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  299, CONTAINER_MP4,  VIDEO_RESOLUTION_1080P_HFR, VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     5.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  302, CONTAINER_WEBM, VIDEO_RESOLUTION_720P_HFR,  VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  2.5,  AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  303, CONTAINER_WEBM, VIDEO_RESOLUTION_1080P_HFR, VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  5,    AUDIO_ENCODING_NOT_SET, 0));
    formats.push_back(YouTubeFormat(STREAM_VIDEO_ONLY,  313, CONTAINER_WEBM, VIDEO_RESOLUTION_2160P,     VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  15,   AUDIO_ENCODING_NOT_SET, 0));
    
    
    formats.push_back(YouTubeFormat(STREAM_AUDIO_ONLY,  139, CONTAINER_MP4,  VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_NOT_SET, VIDEO_PROFILE_NOT_SET,  0.0,  AUDIO_ENCODING_AAC,     48));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_ONLY,  140, CONTAINER_MP4,  VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_NOT_SET, VIDEO_PROFILE_NOT_SET,  0.0,  AUDIO_ENCODING_AAC,     128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_ONLY,  141, CONTAINER_MP4,  VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_NOT_SET, VIDEO_PROFILE_NOT_SET,  0.0,  AUDIO_ENCODING_AAC,     256));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_ONLY,  171, CONTAINER_WEBM, VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_VP9,     VIDEO_PROFILE_NOT_SET,  0.0,  AUDIO_ENCODING_VORBIS,  128));
    formats.push_back(YouTubeFormat(STREAM_AUDIO_ONLY,  172, CONTAINER_WEBM, VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_NOT_SET, VIDEO_PROFILE_NOT_SET,  0.0,  AUDIO_ENCODING_VORBIS,  192));
    
    
    formats.push_back(YouTubeFormat(STREAM_LIVE,        92,  CONTAINER_TS,   VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     0.3,  AUDIO_ENCODING_AAC,     48));
    formats.push_back(YouTubeFormat(STREAM_LIVE,        93,  CONTAINER_TS,   VIDEO_RESOLUTION_360P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     1,    AUDIO_ENCODING_AAC,     128));
    formats.push_back(YouTubeFormat(STREAM_LIVE,        94,  CONTAINER_TS,   VIDEO_RESOLUTION_480P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     1.25, AUDIO_ENCODING_AAC,     128));
    formats.push_back(YouTubeFormat(STREAM_LIVE,        95,  CONTAINER_TS,   VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     3,    AUDIO_ENCODING_AAC,     256));
    formats.push_back(YouTubeFormat(STREAM_LIVE,        96,  CONTAINER_TS,   VIDEO_RESOLUTION_1080P,     VIDEO_ENCODING_H264,    VIDEO_PROFILE_HIGH,     6,    AUDIO_ENCODING_AAC,     256));
    
    formats.push_back(YouTubeFormat(STREAM_LIVE,       120, CONTAINER_FLV,   VIDEO_RESOLUTION_720P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_MAIN,     2,    AUDIO_ENCODING_AAC,     128));
    formats.push_back(YouTubeFormat(STREAM_LIVE,       127, CONTAINER_TS,    VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_NOT_SET, VIDEO_PROFILE_NOT_SET,  0,    AUDIO_ENCODING_AAC,     96));
    formats.push_back(YouTubeFormat(STREAM_LIVE,       128, CONTAINER_TS,    VIDEO_RESOLUTION_NOT_SET,   VIDEO_ENCODING_NOT_SET, VIDEO_PROFILE_NOT_SET,  0,    AUDIO_ENCODING_AAC,     96));
    formats.push_back(YouTubeFormat(STREAM_LIVE,       132, CONTAINER_TS,    VIDEO_RESOLUTION_240P,      VIDEO_ENCODING_H264,    VIDEO_PROFILE_BASELINE, 0.2,  AUDIO_ENCODING_AAC,     48));
    formats.push_back(YouTubeFormat(STREAM_LIVE,       151, CONTAINER_TS,    VIDEO_RESOLUTION_72P,       VIDEO_ENCODING_H264,    VIDEO_PROFILE_BASELINE, 0.05, AUDIO_ENCODING_AAC,     24));
    
}



#if 0
Non-DASH
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+
| itag | DefaultContainer | VideoResolution | VideoEncoding | VideoProfile | VideoBitrateMbits | AudioEncoding | AudioBitratekbits |
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+
|    5 | FLV              | 240P            | Sorenson_H263 | NA           | 0.25              | MP3           | 64                |
|    6 | FLV              | 270P            | Sorenson_H263 | NA           | 0.8               | MP3           | 64                |
|   13 | 3GP              | NA              | MPEG4_Visual  | NA           | 0.5               | AAC           | NA                |
|   17 | 3GP              | 144p            | MPEG4_Visual  | Simple       | 0.05              | AAC           | 24                |
|   18 | MP4              | 360P            | H264          | Baseline     | 0.5               | AAC           | 96                |
|   22 | MP4              | 720P            | H264          | High         | 2-3               | AAC           | 192               |
|   34 | FLV              | 360P            | H264          | Main         | 0.5               | AAC           | 128               |
|   35 | FLV              | 480P            | H264          | Main         | 0.8-1             | AAC           | 128               |
|   36 | 3GP              | 240P            | MPEG4_Visual  | Simple       | 0.175             | AAC           | 32                |
|   37 | MP4              | 1080P           | H264          | High         | 3‚Äì5.9             | AAC           | 192               |
|   38 | MP4              | 3072p           | H264          | High         | 3.5-5             | AAC           | 192               |
|   43 | WebM             | 360P            | VP8           | NA           | 0.5               | Vorbis        | 128               |
|   44 | WebM             | 480P            | VP8           | NA           | 1                 | Vorbis        | 128               |
|   45 | WebM             | 720P            | VP8           | NA           | 2                 | Vorbis        | 192               |
|   46 | WebM             | 1080P           | VP8           | NA           | NA                | Vorbis        | 192               |
|   82 | MP4              | 360P            | H264          | 3D           | 0.5               | AAC           | 96                |
|   83 | MP4              | 240P            | H264          | 3D           | 0.5               | AAC           | 96                |
|   84 | MP4              | 720P            | H264          | 3D           | 2-3               | AAC           | 192               |
|   85 | MP4              | 1080P           | H264          | 3D           | 3-4               | AAC           | 192               |
|  100 | WebM             | 360P            | VP8           | 3D           | NA                | Vorbis        | 128               |
|  101 | WebM             | 360P            | VP8           | 3D           | NA                | Vorbis        | 192               |
|  102 | WebM             | 720P            | VP8           | 3D           | NA                | Vorbis        | 192               |
+------+------------------+-----------------+---------------+--------------+-------------------+---------------+-------------------+

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


299 1080p 60fps
303 302 244 243 242 5 unknown (a9 codec), crashes
140 171 Crashes

298 (no audio)
Video codec: omx-h264
Video width: 1280
Video height: 720
Video profile: 77
Video fps: 59.9401

135 (no audio)
Video codec: omx-h264
Video width: 854
Video height: 480
Video profile: 77
Video fps: 29.97

134 (no audio)
Video codec: omx-h264
Video width: 640
Video height: 360
Video profile: 77
Video fps: 29.97
133 (no audio)
Video codec: omx-h264
Video width: 426
Video height: 240
Video profile: 77
Video fps: 29.97

*   36
Video codec: omx-mpeg4
Video width: 320
Video height: 180
Video profile: -99
Video fps: 29.97

*    22
Video codec: omx-h264
Video width: 1280
Video height: 720
Video profile: 100
Video fps: 29.97
*     18
Video codec: omx-h264
Video width: 640
Video height: 360
Video profile: 578
Video fps: 29.97
*    17
Video codec: omx-mpeg4
Video width: 176
Video height: 144
Video profile: -99
Video fps: 9.99

43  Crashes
Video codec: omx-vp8
Video width: 640
Video height: 360
Video profile: -99
Video fps: 29.97

160 (no audio)
Video codec: omx-h264
Video width: 256
Video height: 144
Video profile: 77
Video fps: 14.985

#endif

