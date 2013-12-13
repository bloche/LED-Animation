#include "ledvideoreader.h"
#include <stdio.h>
#define DEBUG
#undef DEBUG
LEDVideoReader::LEDVideoReader(char* location,uint8_t* buffer,int size) : VideoReaderDelegate(location,buffer,size){
      this->size = size;
      this->buffer = buffer;
      this->location = location;
      this->fileStream = fopen(this->location, "rb");
      printf("file  name: %s\n",location);
  
      if(fileStream == NULL){
          printf("(LEDVideoReader) Error: Can not open file\n");
      }
}

void LEDVideoReader::getNextFrame(){
    static int readerCounter =  0;
    int val = fread(this->buffer, sizeof(uint8_t),this->size,this->fileStream);
    #ifdef DEBUG
    printf("Pulling frame %i, read %i bytes\n",readerCounter, val);
    #endif
    
}

LEDVideoReader::~LEDVideoReader(){
  fclose(this->fileStream);
}

void LEDVideoReader::setBuffer(uint8_t* buffer){
  printf("in set buffer\n");
  this->buffer = buffer;
}
