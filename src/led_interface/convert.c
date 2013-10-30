/* Everett Bloch 10/30/2013
   convert.c

   This file defines the convert function used to convert a color** array
   to a uint8_t** array.
*/

#include "color.h"
#include "stdlib.h"

uint8_t** convert(color** in_matrix, int in_width, int in_hight,
                  int out_width, int out_height,
                  int wise, int scan) 
{
  unint8_t** out_matrix;
  int in_size = in_width * in_height;
  int out_size = out_width * out_height;

  int i;

  // resulting matrix must be 3 times larger than in_matrix
  if (out_size != 3 * in_size)
    return NULL;
  
  out_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * in_height);
  for(i=0; i<in_height; i++) {
    out_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * in_width);
  }
}
