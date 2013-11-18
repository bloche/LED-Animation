/* Everett Bloch  11/13/13
   
   Test the integration of the YouTube interface and Videos Stream interface.

   What this test will do is take the URL retrieved from getStreamUrl()
   and display the video using stream_video().

   The desired video URL or video id should be supplied as a commandline 
   argument. 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern "C" {
#include "../../../src/youtube_interface/yt-surl.h"
}
#include "../../../src/video_stream_interface/vstream.hpp"

int main(int argc, char **argv) 
{
  char *url;
  
  if (argc != 2) {
    printf("usage: %s <youtube-video url>\n", argv[0]);
    exit(1);
  }

  printf("Retrieving video stream... ");
  url = getStreamURL(argv[1], VF_MP4, VQ_LOW);
  if (url == NULL) {
    printf("Error retrieving YouTube video\n");
    exit(1);
  }

  if (strlen(url) < 150) {
    printf("\n%s\n", url);
    exit(1);
  }
  
  printf(" done\nPlaying video\n");
  fflush(stdout);
  if (stream_video(url) < 0)
    printf("Error streaming video\n");

  if(url)
    free(url);
  
  return 0;
}
