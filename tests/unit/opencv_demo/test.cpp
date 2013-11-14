/* Everett Bloch   11/13/13
 
   Test the installation of openCV on the computer. This test requires
   windowing, it will not function properly in terminal mode.

   If openCV is installed correctly with all of the requred dependencies, 
   this file shold compile using 'make', given the proper makefile from
   the LED-Animation git repository is present in this folder.
 
   When executed this program should display a video, located in the 
   current directory called test.mp4, in a window titled "video stream".

   test.mp4 needs to be a valid mp4 file copied into this directory. One 
   way to acquire an mp4 file would be to download one via the video_stream
   test program playVideo with the -d option. Be sure libcurl is installed
   on your system.

   To exit the video while it is playing, press any button.
*/

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include <stdio.h>

using namespace cv;
using namespace std;

int main(int, char**)
{
  VideoCapture stream;
  Mat frame;
  const string LOCATION = "./test.mp4";

  if(!stream.open(LOCATION)) {
    cout << "Error opening video stream or file" << endl;
    return -1;
  }

  for(;;) {
    if(!stream.read(frame)) {
      cout << "No frame" << endl;
      waitKey();
    }
    imshow("video stream", frame);
    if(waitKey(1) >= 0) break;
  }

  return 0;
}
