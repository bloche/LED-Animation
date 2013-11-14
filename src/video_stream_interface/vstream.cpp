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

#include "vstream.hpp"

using namespace cv;
using namespace std;

int stream_video(char* video_location) {
  VideoCapture stream;
  Mat frame;
  const string LOCATION = video_location;

  if (video_location == NULL)
    return -1;
  
  if(!stream.open(LOCATION)) {
    cout << "Error opening video stream or file" << endl;
    return -2;
  }

  for(;;) {
    if(!stream.read(frame)) {
      cout << "No frame" << endl;
      break;
    }
    imshow("video stream", frame);
    if(waitKey(1) >= 0) break;
  }

  return 0;
}
