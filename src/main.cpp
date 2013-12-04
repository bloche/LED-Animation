/* Everett Bloch  12/4/13
   main.cpp

   The main file the aggregates tha LED Animation project interfaces
   into a single program.
*/

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
  char c;
  int buffer_flg = 0;
  int dwnld_flg = 0;
  int stream_flg = 0;
  char* buffer_file = NULL;
  char* download_file = NULL;
  char* stream_file = NULL;

  FILE* ofile = NULL;
  FILE* bfile = NULL;
  FILE* infile = NULL;

  while ((c = getopt(argc, argv, "b:d:s:?")) != EOF)
    switch (c) {
    case 'b':
      buffer_file = optarg;
      buffer_flg = 1;
      break;
    case 'd':
      download_file = optarg;
      dwnld_flg = 1;
      break;
    case 's':
      stream_file = optarg;
      stream_flg = 1;
      break;
    case '?':
      printf("Options description:\n");
      printf("\tb file - Buffer LED display pixel map to specified file\n");
      printf("\td file - Download YouTube video to specified file\n");
      printf("\ts file - Stream from file containtin LED pixel map, use after -b option\n");
      printf("\t?      - Print options\n");
      exit(1);
      break;
    default :
      printf("Usage: %s [-d] [-b <buffer_file>] <video URL, ID, or PATH>", argv[0]);
      exit(1);
    }

  if (argv[optind] == NULL) {
    printf("YouTube video URL or ID, or video file name not present\n");
    exit(1);
  }

  if (buffer_flg) {
    bfile = fopen(buffer_file, "w");
    if (buffer_file == NULL) {
      printf("Error: '%s' could not be opend\n", buffer_file);
      exit(1);
    }
  }

  if (dwnld_flg) {
    ofile = fopen(download_file, "w");
    if (ofile == NULL) {
      printf("Error: '%s' could not be opend\n", download_file);
      exit(1);
    }
  }

  if (stream_flg) {
    infile = fopen(stream_file, "r");
    if (infile == NULL) {
      printf("Error: '%s' could not be opend\n", stream_file);
      exit(1);
    }
  }
 
  return 0;
}
