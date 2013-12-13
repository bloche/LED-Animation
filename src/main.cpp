/* Everett Bloch  12/4/13
   main.cpp

   The main file that aggregates the LED Animation project's interfaces
   into a single program.
*/

#include <string.h>
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <QCoreApplication>
//#include "led_interface/videocontroller.h"
#include "video_stream_interface/videoreaderdelegate.h"
#include "video_stream_interface/videoreader.h"
#include "video_stream_interface/ledvideoreader.h"
#include "led_interface/ledmatrixdriver.cpp"

extern "C" {
#include "youtube_interface/yt-surl.h"
}

#include "../tests/integration/url_stream/vstream.h"

#define MAIN_OTHER 0
#define MAIN_LED 1

int main(int argc, char* argv[]) {
  char c;
  int buffer_flg = 0;
  int dwnld_flg = 0;
  int fstream_flg = 0;
  int youtube_flg = 1;
  char* buffer_file = NULL;
  char* download_file = NULL;
  char* fstream_file = NULL;

  char* mfilename;  
  uint8_t* convertBuffer;
  int counter= 0;

  FILE* ofile = NULL;
  FILE* bfile = NULL;
  FILE* infile = NULL;

  int vid_isfile = 0;
  FILE* temp_file = NULL;

  LEDMatrixDriver* driver;

  char* url = NULL;

  int SIZE = 1024;
  char cmd[SIZE];

  VideoReaderDelegate* reader;
 // VideoController* controller;

  QCoreApplication* a = new QCoreApplication(argc, argv);
  /* get options */
  while ((c = getopt(argc, argv, "b:d:l:x?")) != 255){ 
  switch (c) {
   case 'b':
      buffer_file = optarg;
      buffer_flg = 1;
      break;
    case 'd':
      download_file = optarg;
      dwnld_flg = 1;
      break;
    case 'l':
      printf("found case 1");
      fstream_file = optarg;
      fstream_flg = 1;
      vid_isfile = 1;
      break;
    case 'x':
      youtube_flg = 0;
      break;
    case '?':
      printf("LED Animation:\n");
      printf("Display a video on a Raspberry PI LED panel.\n");
      printf("Usage: %s [-x?] [-d file] [-b file] [-l file] <video (URL, ID, or PATH)>\n\n", argv[0]);
      printf("Options description:\n");
      printf("\tb file - Buffer LED display pixel map into specified file from video location\n");
      printf("\td file - Download YouTube video to specified file from video location\n");
      printf("\tl file - Load file containting LED pixel map as source file\n");
      printf("\t	   (requires -b option to have already been run on specified file)\n");
      printf("\tx      - Video is not from YouTube, use with URLs\n");
      printf("\t?      - Print options\n");
      exit(1);
      break;
    default :
    printf("at 1\n");
    printf("Usage: %s [-x] [-?] [-d file] [-b file] [-l file] <video (URL, ID, or PATH)>\n", argv[0]);
    exit(1);
    }
  }
  /* check if valid parameters were input */
  if (argv[optind] == NULL && !(fstream_flg)) {
    printf("at 2");
    printf("Usage: %s [-x] [-?] [-d file] [-b file] [-l file] <video (URL, ID, or PATH)>\n", argv[0]);
    exit(1);
  }

  /* check if video location provided is a file or not */
  if ((temp_file = fopen(argv[optind], "r")) != NULL) {
    vid_isfile = 1;
    fclose(temp_file);
  }

  /* set valid url for video */
  if (!vid_isfile) {
    /* if URL, video is from YouTube */
    if (youtube_flg) {
      /* get video stream url */
      url = getStreamURL(argv[optind], VF_MP4, VQ_LOW);
      if (url != NULL) {
	if (strlen(url) < 150) {
	  printf("%s\n", url);
	  exit(1);
	}
      }
      else
	printf("Error: A problem occured while processing stream URL\n");
    }
    else /* video is not from YouTube */
      url = argv[optind];    
  }

  /* check if buffer as pixmap flag was set*/
  if (buffer_flg) {
    bfile = fopen(buffer_file, "w");
    if (buffer_file == NULL) {
      printf("Error: '%s' could not be opened\n", buffer_file);
      exit(1);
    }
    /*
      convert video into uint8_t pixel map, put pixel map in bfile
      system command could do this, instead of rewiring what Grant 
      already has.
     */
    convertBuffer = new uint8_t[3072]();
    VideoReader* reader = new VideoReader(argv[optind],convertBuffer,3072);
    reader->getNextFrame();
    while(1){
      fwrite(convertBuffer, sizeof(uint8_t), 3072, bfile);
     // printf("Grabed frame %i\n",counter);
      reader->getNextFrame();
    }   
    counter = 0;
    exit(1);
  }

  /* check if download flag was set */
  if (dwnld_flg) {
    if (vid_isfile) {
      printf("Error: Cannot download a local file\n");
      exit(1);
    }
    ofile = fopen(download_file, "w");
    if (ofile == NULL) {
      printf("Error: '%s' could not be opened\n", download_file);
      exit(1);
    }
    
    // download video from url
    snprintf(cmd, SIZE, "curl --location -o %s \"%s\"", download_file, url);
    printf("Downloading video...\n");
    system(cmd);
    exit(1);
  }
  
  /* check is load and stream pixmap from file flag was set */
  if (fstream_flg) {
    infile = fopen(fstream_file, "r");
    if (infile == NULL) {
      printf("Error: '%s' could not be opened\n", fstream_file);
      exit(1);
    }
    driver = new LEDMatrixDriver(argv[2], MAIN_LED);
    driver->start();
    
/*
    printf("Displaying .LED file");
    mfilename = strdup(argv[2]);
    printf("mfilename is %s\n",mfilename);
    reader = new LEDVideoReader(mfilename,NULL, 3072);
    controller = new VideoController(reader, mfilename);
    printf("reader and controller complete\n");
*/
  } else {  
  /* display video */
  /*
    True video stream from file or url
    -l option should pull from saved uint8_t pixel map file
   */
  if (vid_isfile || !youtube_flg){
    driver = new LEDMatrixDriver(argv[optind],MAIN_OTHER);
    driver->start();
  }
  else {
    driver = new LEDMatrixDriver(url,MAIN_OTHER);
    driver->start();
  }
}
  //finish QMain code
  printf("about to call a->exec\n");
  a->exec();
  if(NULL != mfilename){
    free(mfilename);
  }
  return 0;
}
