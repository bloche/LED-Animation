/* Everett Bloch   11/13/13
 
   Test the installation of openCV on the computer. This test requires
   windowing, it will not function properly in terminal mode.

   If compiled correctly, this file should display the test.mp4 video
   in a video titles "video stream".

   Press any button while video is playing to exit.
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
