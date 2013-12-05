/* Everett Bloch  12/4/13
   main.cpp

   The main file that aggregates the LED Animation project's interfaces
   into a single program.
*/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "../../../src/youtube_interface/yt-surl.h"
}

#include "../url_stream/vstream.h"

int main(int argc, char* argv[]) {
  char c;
  int buffer_flg = 0;
  int dwnld_flg = 0;
  int fstream_flg = 0;
  int youtube_flg = 1;
  char* buffer_file = NULL;
  char* download_file = NULL;
  char* fstream_file = NULL;

  FILE* ofile = NULL;
  FILE* bfile = NULL;
  FILE* infile = NULL;

  int vid_isfile = 0;
  FILE* temp_file = NULL;

  char* url = NULL;

  int SIZE = 1024;
  char cmd[SIZE];

  /* get options */
  while ((c = getopt(argc, argv, "b:d:l:x?")) != EOF)
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
    printf("Usage: %s [-x] [-?] [-d file] [-b file] [-l file] <video (URL, ID, or PATH)>\n", argv[0]);
    exit(1);
    }

  /* check if valid parameters were input */
  if (argv[optind] == NULL && !(fstream_flg)) {
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
    /*
      stream video from infile, contains uint8_t pixel map
     */
  }

  /* display video */
  /*
    True video stream from file or url
    -l option should pull from saved uint8_t pixel map file
   */
  if (vid_isfile || !youtube_flg)
    stream_video(argv[optind]);
  else 
    stream_video(url);
 
  return 0;
}
