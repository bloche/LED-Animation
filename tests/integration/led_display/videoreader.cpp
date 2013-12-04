/* Everett Bloch:   10/13/13
   vstream.cpp

   Decode and process video stream using openCV. Video stream is
   processed frame by frame, converted to 32x32 resolution, then 
   converted into a 3072 uint8_t array for use in LED display drivers.

  Grant Boomer 11/22/13

  Rewrote some code to put Everett's (excellent) code into a noce neet
  reader package
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdint.h>

#include "videoreader.h"

#define DEBUG
using namespace cv;
using namespace std;

/*****************************************************************
Methodes for VideoReader class
******************************************************************/

/*****************************************************************
constructor

Takes a file location string, uint8_t array, and size of that array. 
initialises fields.
*******************************************************************/
VideoReader::VideoReader(char* location, uint8_t* buffer, int size){
    this->location = location;
    #ifdef DEBUG
    printf("Location = %s\n", location);
    #endif
    //this->LEDframe = *(new Mat(Size(32,32),CV_8U)):;
    this->buffer = buffer;
    this->size = size;
    if(!this->stream.open(this->location)){
      cout << "Error initialising Video stream: Can not open file (In constructor)" << endl;
    }
    #ifdef DEBUG
    if(stream.isOpened()){
      cout << "Stream opened video at location" << endl;
    } else {
      cout << "Stream did NOT open file at location" << endl;
    }
    #endif //DEBUG
}

/******************************************************************
public getNextFrame
@param none;
return none;

pulls the next frame out of the video and copies it to the buffer
*******************************************************************/
void VideoReader::getNextFrame(){
  Mat LEDframe(Size(32,32),CV_8U);

  /* get frame from stream */
  if(!stream.read(frame)) {
    cout << "No frame" << endl;
  }

  /* resize to 32x32 resolution */
  resize(frame, LEDframe, LEDframe.size(), 0, 0, INTER_AREA);

	//convert to buffer
   extractPixels(LEDframe);

}
/******************************************************************
private extractPixels
@param a MAT image
retrun none

Copy's the color data (specific to the LED-ANIMATION project) to the
class buffer.

Note: No checking for buffer size. 
Note: Hard-coded for 32x32 LED screen
*******************************************************************/
void VideoReader::extractPixels(Mat in_mat){
  uint8_t* data = (uint8_t*) in_mat.data;
  int i, j;
  int pos=0;  // position in out_stream
  int cn = in_mat.channels(); // number of color channels
  int rows = in_mat.rows;
  int cols = in_mat.cols;

  // rows and cols must be positive
  if (rows < 0 || cols < 0)
    return;

  // must have 3 color channels
  if (cn != 3)
    return;

  /* copy RGB values from in_mat to out_stream */
  if (this->buffer != NULL)
    for(i=0; i<rows; i++)
      for(j=0; j<cols; j++) {
	      /* Mat data is stored as BGR values */
	      this->buffer[pos++] = data[i*cols*cn + j*cn + 2]; // R
	      this->buffer[pos++] = data[i*cols*cn + j*cn + 1]; // G
	      this->buffer[pos++] = data[i*cols*cn + j*cn + 0]; // B
      }

}
