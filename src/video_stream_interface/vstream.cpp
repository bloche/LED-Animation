/* Everett Bloch   11/13/13
   vstream.cpp

   Decode and process video stream using openCV. Video stream is
   processed frame by frame, converted to 32x32 resolution, then 
   converted into a 3072 uint8_t array for use in LED display drivers.
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdint.h>

#include "vstream.hpp"

using namespace cv;
using namespace std;

/* convert MAT to uint8_t */
static uint8_t* extract_pixels(Mat frame); 

int stream_video(char* video_location) {
  VideoCapture stream;
  Mat frame; 
  Mat LEDframe(Size(32,32),CV_8U); // 32x32 frame
  uint8_t* pixmap = NULL;

  if (video_location == NULL)
    return -1;
  
  /* open video from video_location */
  if(!stream.open(video_location)) {
    cout << "Error opening video stream or file" << endl;
    return -2;
  }

  for(;;) {
    /* get frame from stream */
    if(!stream.read(frame)) {
      cout << "No frame" << endl;
      break;
    }

    /* resize to 32x32 resolution */
    resize(frame, LEDframe, LEDframe.size(), 0, 0, INTER_AREA);

    /* convert LEDframe Mat uint8_t array */
    pixmap = extract_pixels(LEDframe);
    if (pixmap == NULL)
      cout << "Error converting to pixel map" << endl;

    /* test frame capture */
    imshow("video stream", LEDframe);
    if(waitKey(1) >= 0) break;
    
    if(pixmap)
      delete pixmap;
  }
  
  if(pixmap)
    delete pixmap;

  return 0;
}

/* convert 2D Mat to a uint8_t* array */
static uint8_t* extract_pixels(Mat in_mat) {
  uint8_t* out_stream;
  uint8_t* data = (uint8_t*) in_mat.data;
  int i, j;
  int pos=0;  // position in out_stream
  int cn = in_mat.channels(); // number of color channels
  int rows = in_mat.rows;
  int cols = in_mat.cols;

  // rows and cols must be positive
  if (rows < 0 || cols < 0)
    return NULL;

  // must have 3 color channels
  if (cn != 3)
    return NULL;

  out_stream = new uint8_t[rows*cols*3];

  /* copy RGB values from in_mat to out_stream */
  if (out_stream != NULL)
    for(i=0; i<rows; i++)
      for(j=0; j<cols; j++) {
	/* Mat data is stored as BGR values */
	out_stream[pos++] = data[i*cols*cn + j*cn + 2]; // R
	out_stream[pos++] = data[i*cols*cn + j*cn + 1]; // G
	out_stream[pos++] = data[i*cols*cn + j*cn + 0]; // B
      }
      
  return out_stream;
}
