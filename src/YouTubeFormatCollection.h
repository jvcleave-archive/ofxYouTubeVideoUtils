#pragma once
#include "YouTubeFormat.h"

class YouTubeFormatCollection
{
public:
    static YouTubeFormatCollection& getInstance()
    {
        static YouTubeFormatCollection    instance; // Guaranteed to be destroyed.
        // Instantiated on first use.
        return instance;
    }
    vector<YouTubeFormat> formatCollection;
    vector<YouTubeFormat>& getFormatCollection()
    {
        return formatCollection;
    };
    YouTubeFormat getFormatForItag(int itag)
    {
        YouTubeFormat format;
        for(size_t i=0; i<formatCollection.size(); i++)
        {
            if(formatCollection[i].itag == itag)
            {
                return formatCollection[i];
            }
        }
        ofLogVerbose() << "format.itag: " << format.itag;
        
        return format;
    }

private:
    YouTubeFormatCollection() {
        YouTubeFormat formatter;
        formatter.createFormats(formatCollection);
    };
    YouTubeFormatCollection(YouTubeFormatCollection const&);
    void operator=(YouTubeFormatCollection const&);
};
