#include "videocontroller.h"
#include "ledmatrixdriver.h"
#include "videodisplay.h"
#include <QThread>
#include <QObject>
#include <QDebug>
#include <QVariant>
#include <QCoreApplication>
#include <stdio.h>

#define DEBUG
VideoController::VideoController(char* videoDirectory, QObject *parent) :
  QObject(parent)
{ 
  //initialize components
  this->driver = new LEDMatrixDriver;
  #ifdef DEBUG
  printf("Driver Initialised\n");
  #endif
  this->display = new VideoDisplay(videoDirectory);
  #ifdef DEBUG
  printf("display Initialised\n");
  #endif
  

  //create new driver thread
  QThread *driverThread = new QThread(this);
  //when the thread starts, start the driver
  QObject::connect(driverThread, SIGNAL(started()),this->driver, SLOT(start()));
  //Delete this driver when the thread stops
  QObject::connect(driverThread, SIGNAL(finished()), this->driver, SLOT(deleteLater())); // don't know if I have a delete later...
  //move the driver to the driver thread
  this->driver->moveToThread(driverThread);

  //connect the scrolling text display updateFrame signal to drivers outputFrame slot.
  //connect(this->display, SIGNAL(updateFrame(uint8_t*,ulong,ulong)), this->driver, SLOT(outputFrame(uint8_t*, ulong, ulong)));

  //create the display thread
  QThread* displayThread = new QThread(this);
  QObject::connect(displayThread, SIGNAL(started()), this->display, SLOT(start()));
 // QObject::connect(this->display, SIGNAL(finished()), displayThread, SLOT(quit()));
  QObject::connect(displayThread, SIGNAL(finished()), this->display, SLOT(deleteLater()));
 // QObject::connect(displayThread, SIGNAL(finished()), qApp, SLOT(quit()));
  this->display->moveToThread(displayThread);
  
  //start the display and driver threads, and thus play video
 // driverThread->start();
  #ifdef DEBUG
//  printf("driverThread started from controller\n");
  #endif

  displayThread->start();
  #ifdef DEBUG
  printf("displayThread started from controller\n");
  #endif
  while(i){}


}
