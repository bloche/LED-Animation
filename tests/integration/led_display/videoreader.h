#ifndef VIDEO_READER_H
#define VIDEO_READER_H
/* Everett Bloch   11/13/13
   vstream.hpp

   Header file for vstream.cpp. 
*/
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdint.h> 

using namespace cv;
using namespace std;
/**************************************************************
VideoReader

A class that encapsulates the process of converting a frame
of video to a buffer. To use, create a new instance of reader,
using a location of a file, an existing output buffer, and the size
of that buffer.

Output size is currently locked at 32x32

Note: The color usage is specific to the LED-Animation project.
See documentation for details
***************************************************************/
class VideoReader{
	public:
		//constructor that sets up the buffer and location and size
		VideoReader(char*,uint8_t*,int);
		void getNextFrame();
	private:
		//fields
		char* location;
		uint8_t* buffer;
		int size;
    VideoCapture stream;
    Mat frame;
    //Mat LEDframe;//(Size(32,32),CV_8U); //this might not work, dpends on if its a macro.

		//methods
		void extractPixels(Mat);
};


#endif //VIDEO_READER_H
