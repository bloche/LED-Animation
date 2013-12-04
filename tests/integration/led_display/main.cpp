/**
 * The Raspberry Pi server application.
 *
 * This application listens for messages from the desktop client application
 * as well as driving the LED matrices.
 */

#include "videocontroller.h"
#include <QCoreApplication>
#include <stdio.h>

#define DEBUG
/**
 * @brief The entry point for the application.
 * @param argc The number of command line arguments
 * @param argv The array of command line argument strings.
 * @return The exit status of the application.
 */
int main(int argc, char *argv[])
{   
    printf("application start\n");
    QCoreApplication *a = new QCoreApplication(argc, argv);
    #ifdef DEBUG
    printf("New QCoreApplication created\n");
    #endif
    // Create the server instance. The consuctor of it will set up
    // the driver, scrolling text display, and start listening for
    // incoming settings.
    //Server *server = new Server(a);
    if(argc == 2){  
      #ifdef DEBUG
      printf("argc = %i, argv[0] = %s\n",argc, argv[1]);
      #endif
    	VideoController videoController(argv[1], a);
    } else {
      printf("Usage: video [Path to .mp4 video]\n");
      return -1;
    }
    return a->exec();
}
