#ifndef VIDEO_READER_DELEGATE
#define VIDEO_READER_DELEGATE
#include <stdint.h>

/*************************************************
Interface for VideoReader, a class in charge of filling
a buffer passed to it through initialisation.
**************************************************/
class VideoReaderDelegate{
    public:
        VideoReaderDelegate(char*,uint8_t*,int){}
        virtual void getNextFrame() = 0;
        virtual void setBuffer(uint8_t* buffer) = 0;
        virtual ~VideoReaderDelegate(){}
};
#endif
