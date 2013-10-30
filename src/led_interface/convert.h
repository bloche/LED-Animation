/* Everett Bloch 10/30/2013
   convert.h

   This header file contains the declaration of the convert function.
   This function converts an arbitrary color** array to a matrix that is
   compatible with the LED display drivers created by team RPLD.
   The color structure is defined in color.h.
*/

#include "color.h"

/* convert in_matrix with in_width and in_height demintions, to a
   matrix of out_width and out_height demintions.
   
   wise determins row wise or column wise; scan determines single 
   deirection scan or back and forth scan of in_matrix.
*/
uint8_t** convert(color** in_matrix, 
		  int in_width, 
		  int in_hight, 
		  int out_width, 
		  int out_height, 
		  int wise, 
		  int scan); 
