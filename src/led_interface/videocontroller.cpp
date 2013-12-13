#include "videocontroller.h"
#include "ledmatrixdriver.h"
#include "../video_stream_interface/videodisplay.h"
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QCoreApplication>
#include <stdio.h>

#define DEBUG
VideoController::VideoController(VideoReaderDelegate* reader,char* videoDirectory, QObject *parent) :
  QObject(parent)
{
  printf("in VideoController constructor\n"); 
  //initialize components
  this->driver = new LEDMatrixDriver(reader);
  #ifdef DEBUG
  printf("Driver Initialised\n");
  #endif
  this->display = new VideoDisplay(videoDirectory);
  #ifdef DEBUG
  printf("display Initialised\n");
  #endif
  

//  create new driver thread
  QThread *driverThread = new QThread(this);
  //when the thread starts, start the driver
  QObject::connect(driverThread, SIGNAL(started()),this->driver, SLOT(start()));
  //Delete this driver when the thread stops
  QObject::connect(driverThread, SIGNAL(finished()), this->driver, SLOT(deleteLater())); // don't know if I have a delete later...
  //move the driver to the driver thread
  this->driver->moveToThread(driverThread);

  //connect the scrolling text display updateFrame signal to drivers outputFrame slot.
  //connect(this->display, SIGNAL(outputTest(int)), this->driver, SLOT(inputTest(int)));
  //connect(this->display, SIGNAL(updateFrame(uint8_t*,int,int)), this->driver, SLOT(outputFrame(uint8_t*, int, int)));

  //create the display thread
  QThread* displayThread = new QThread(this);
  #ifdef DEBUG
  printf("Display thread created\n");
  #endif
  QObject::connect(displayThread, SIGNAL(started()), this->display, SLOT(start()));
  #ifdef DEBUG
  printf("Connected Started to Start\n");
  #endif
 // QObject::connect(this->display, SIGNAL(finished()), displayThread, SLOT(quit()));
  QObject::connect(displayThread, SIGNAL(finished()), this->display, SLOT(deleteLater()));
  #ifdef DEBUG
  printf("finished connected to deleteLater\n");
  #endif
 // QObject::connect(displayThread, SIGNAL(finished()), qApp, SLOT(quit()));
  this->display->moveToThread(displayThread);
  #ifdef DEBUG
  printf("Display moveToThread\n");
  #endif
  
  //start the display and driver threads, and thus play video
  driverThread->start();
  #ifdef DEBUG
  printf("driverThread started from controller\n");
  #endif

  //displayThread->start();
  #ifdef DEBUG
  printf("displayThread started from controller\n");
  #endif
  while(1){}


}
