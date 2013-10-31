/* Everett Bloch 10/30/2013
   convert.c

   This file defines the convert function used to convert a color** array
   to a uint8_t** array.
*/

#include "convert.h"
#include "stdlib.h"

/* converts in_matirx with in_width columns and in_height rows and returns 
   a matrix three times larger with 3 rows and (in_width * in_hight) 
   columns.

   wise determines row (ROW_WISE) or column (COL_WISE) wise copy of in_matrix, 
   and scan determines weather to scan from one side to another (SCAN_MONODIR)
   , or back and forth (SCAN_BIDIR).
*/
uint8_t** convert(color** in_matrix, 
		  unsigned int in_width, unsigned int in_height,
                  unsigned int wise, unsigned int scan) 
{
  uint8_t** out_matrix;
  unsigned int in_size = in_width * in_height;
  int pos = 0;
  int i, j;

  // allocate memory  
  out_matrix = (uint8_t**) malloc(sizeof(uint8_t*) * 3);
  for(i=0; i<3; i++)
    out_matrix[i] = (uint8_t*) malloc(sizeof(uint8_t) * in_size);

  switch (scan) {
  case SCAN_MONODIR:
    switch (wise) {
    case ROW_WISE:
      // row wise copy
      for(i=0; i<in_height; i++) 
	for(j=0; j<in_width; j++) {
	  out_matrix[0][pos] = in_matrix[i][j].R;
	  out_matrix[1][pos] = in_matrix[i][j].G;
	  out_matrix[2][pos] = in_matrix[i][j].B;
	  pos++;
	}
      break;

    case COL_WISE:
      // column wise copy
      for(i=0; i<in_width; i++) 
	for(j=0; j<in_height; j++) {
	  out_matrix[0][pos] = in_matrix[j][i].R;
	  out_matrix[1][pos] = in_matrix[j][i].G;
	  out_matrix[2][pos] = in_matrix[j][i].B;
	  pos++;
	}
      break;

    default:
      return NULL;
    } // end switch (wise)

    break; // end SCAN_MONODIR

  case SCAN_BIDIR:
    switch (wise) {
    case ROW_WISE:
      // row wise copy
      for(i=0; i<in_height; i++) {
	for(j=0; j<in_width; j++) {
	  out_matrix[0][pos] = in_matrix[i][j].R;
	  out_matrix[1][pos] = in_matrix[i][j].G;
	  out_matrix[2][pos] = in_matrix[i][j].B;
	  pos++;
	}
	i++;
	if(i<in_height) 
	  // copy next row in other direction
	  for(j=in_width-1; j>=0; j--) {
	    out_matrix[0][pos] = in_matrix[i][j].R;
	    out_matrix[1][pos] = in_matrix[i][j].G;
	    out_matrix[2][pos] = in_matrix[i][j].B;
	    pos++;
	  }
      }
      break;
      
    case COL_WISE:
      // column wise copy
      for(i=0; i<in_width; i++) {
	for(j=0; j<in_height; j++) {
	  out_matrix[0][pos] = in_matrix[j][i].R;
	  out_matrix[1][pos] = in_matrix[j][i].G;
	  out_matrix[2][pos] = in_matrix[j][i].B;
	  pos++;
	}
	i++;
	if(i<in_width) 
	  // copy next col in other direction
	  for(j=in_height-1; j>=0; j--) { 
	    out_matrix[0][pos] = in_matrix[j][i].R;
	    out_matrix[1][pos] = in_matrix[j][i].G;
	    out_matrix[2][pos] = in_matrix[j][i].B;
	    pos++;
	  }
      }
      break;
    
    default: // invalid wise value
      return NULL;
    } // end switch (wise)
    
    break; // end SCAN_BIDIR
    
  default:
    return NULL;
    
  } // end switch (scan)
  
  return out_matrix;
}
