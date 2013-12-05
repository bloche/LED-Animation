/* Everett Bloch  12/4/13
   main.cpp

   The main file the aggregates tha LED Animation project interfaces
   into a single program.
*/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "youtube_interface/yt-surl.h"
}

#include "../tests/integration/url_stream/vstream.h"

int main(int argc, char* argv[]) {
  char c;
  int errflg = 0;
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
      break;
    case 'x':
      youtube_flg = 0;
      break;
    case '?':
      printf("Usage: %s [-x?] [-d file] [-b file] [-l file] <video (URL, ID, or PATH)>\n", argv[0]);
      printf("Options description:\n");
      printf("\tb file - Buffer LED display pixel map to specified file\n");
      printf("\td file - Download YouTube video to specified file\n");
      printf("\tl file - Load file containtin LED pixel map and stream data\n");
      printf("\t	   (requires -b option to have already been run)\n");
      printf("\tx      - Video is not from YouTube, use with URLs\n");
      printf("\t?      - Print options\n");
      exit(1);
      break;
    default :
      errflg = 1;
      exit(1);
    }

  /* check if valid parameters were input */
  if (argv[optind] == NULL || errflg) {
    printf("Usage: %s [-x?] [-d file] [-b file] [-l file] <video (URL, ID, or PATH)>\n", argv[0]);
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
    printf("cmd: %s\n", cmd);
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
  }

  if (vid_isfile || !youtube_flg)
    stream_video(argv[optind]);
  else 
    stream_video(url);
 
  return 0;
}
