/* Everett Bloch 10/30/2013
   convert.h

   This header file contains the declaration of the convert function.
   This function converts an arbitrary color** array to a matrix that is
   compatible with the LED display drivers created by team RPLD.
   The enum is for valid values for wise and scan values for the 
   convert function.
   The color structure is defined in color.h.
*/

#include "color.h"
#include <stdint.h>

// enum used for wise and scan values
enum {ROW_WISE, COL_WISE, SCAN_MONODIR, SCAN_BIDIR};

/* convert in_matrix with in_width and in_height demintions, to a
   matrix three times larger (in_width * in_height * 3)
   
   wise determins row wise or column wise; scan determines single 
   deirection scan or back and forth scan of in_matrix.
*/
uint8_t* convert(color** in_matrix, 
		  unsigned int in_width, 
		  unsigned int in_hight, 
		  unsigned int wise, 
		  unsigned int scan); 
