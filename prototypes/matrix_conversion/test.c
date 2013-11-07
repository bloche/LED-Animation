/* Everett Bloch 10/31/2013
   test.c

   This file tests the convert function for converting a color** matrix
   into a matrix that can be sent to the LED display drivers.

   The color** matrix can be of any size, what is returned is a matrix
   3 times larger, having three rows for each color (R,G,B) for each 
   cell of the color** matrix.
*/

#include <stdio.h>
#include <stdlib.h>

#include "../../src/video_stream_interface/convert.h"

void printMatrix(uint8_t* matrix, int size) {
  int i, j;
  printf("   [ ");
  for(i=0; i<size; i++) 
      printf("%2i ", matrix[i]);
  printf("]\n");
}

int main() {
  color** in_matrix;
  uint8_t* out_matrix;
  int const SIZE = 3;
  int i, j, count;
  
  // allocate memeory
  in_matrix = (color**) malloc(sizeof(color*) * SIZE);
  for(i=0; i<SIZE; i++)
    in_matrix[i] = (color*) malloc(sizeof(color) * SIZE);

  count = 0;

  /* fill in matrix */
  for(i=0; i<SIZE; i++)
    for(j=0; j<SIZE; j++) {
      in_matrix[i][j].R = count++;
      in_matrix[i][j].G = count++;
      in_matrix[i][j].B = count++;
    }

  /* print matrix */
  puts("Test matrix:");
  for(i=0; i<SIZE; i++) {
    printf("   [ ");
    for(j=0; j<SIZE; j++) {
      printf("[ %2i, %2i, %2i ] ", 
	     in_matrix[i][j].R, 
	     in_matrix[i][j].G, 
	     in_matrix[i][j].B );
    }
    printf("]\n\n");
  }
  
  printf("Convert - row wise, mono-directional scan:\n");
  out_matrix = convert(in_matrix, SIZE, SIZE, ROW_WISE, SCAN_MONODIR);
  if (out_matrix == NULL)
    printf("Error: convert\n");
  else {
    printMatrix(out_matrix, 3 * SIZE * SIZE);
    free(out_matrix);
  }
  printf("\n");

  printf("Convert - column wise, mono-directional scan:\n");
  out_matrix = convert(in_matrix, SIZE, SIZE, COL_WISE, SCAN_MONODIR);
  if (out_matrix == NULL)
    printf("Error: convert\n");
  else {
    printMatrix(out_matrix, 3 * SIZE * SIZE);
    free(out_matrix);
  }
  printf("\n");

  printf("Convert - row wise, bi-directional scan:\n");
  out_matrix = convert(in_matrix, SIZE, SIZE, ROW_WISE, SCAN_BIDIR);
  if (out_matrix == NULL)
    printf("Error: convert\n");
  else {
    printMatrix(out_matrix, 3 * SIZE * SIZE);
    free(out_matrix);
  }
  printf("\n");

  printf("Convert - column wise, bi-directional scan:\n");
  out_matrix = convert(in_matrix, SIZE, SIZE, COL_WISE, SCAN_BIDIR);
  if (out_matrix == NULL)
    printf("Error: convert\n");
  else {
    printMatrix(out_matrix, 3 * SIZE * SIZE);
    free(out_matrix);
  }
  printf("\n");

  for(i=0; i<SIZE; i++)
    free(in_matrix[i]);
  free(in_matrix);

  return 0;
}
