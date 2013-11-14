#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../../src/youtube_interface/yt-surl.h"

int main(int argc, char* argv[]) 
{
  char* str;

  if(argc < 2) {
    printf("Usage: %s [<youtube url> | <youtube vid id>]\n", argv[0]);
    return 1;
  }

  str = getStreamURL(argv[1], VF_MP4, VQ_LOW);

  if (str == NULL )
    printf("Error: A problem occured while processing video data, please try again\n");
  else {
    printf("%s\n\n", str);
    if (strlen(str) > 150)
      free(str);
  }

  return 0;
}
