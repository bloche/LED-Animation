#ifndef LED_VIDEO_READER
#define LED_VIDEO_READER

#include "videoreaderdelegate.h"
#include <stdio.h>
class LEDVideoReader: public VideoReaderDelegate{

  public:
        LEDVideoReader(char*,uint8_t*,int);
       ~LEDVideoReader();
        void getNextFrame();
        void setBuffer(uint8_t* buffer);
  private:
        FILE* fileStream;
        uint8_t* buffer;
        int size;
        char* location;
};
#endif
