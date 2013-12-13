#include "videodisplay.h"
#include "ledvideoreader.h"
#include <QThread>
#include <QDebug>
#include <stdint.h>
#include <iostream>
#include <cassert>

#define DEBUG
VideoDisplay::VideoDisplay(char* fileLocation, QObject* parent) : QObject(parent) {
  #ifdef DEBUG
  printf("Constructing VideoDisplay UPDATED!\n");
  #endif
  this->buffer = new uint8_t [32 * 32 * 3]();
  this->reader = new  LEDVideoReader(fileLocation, this->buffer, 3072);
  this->timer = new QTimer(this);
  #ifdef DEBUG
  printf("(VideoDisplay) Timer Creater\n");
  #endif
  connect(this->timer, SIGNAL(timeout()), this, SLOT(update()));
  #ifdef DEBUG
  printf("(VideoDisplay) Timer connected\n");
  #endif
}

VideoDisplay::~VideoDisplay(){
  #ifdef DEBUG
  printf("VideoDisplay:: In Deconstructor\n");
  #endif
//  delete this->buffer;
//  delete this->reader;
//  if(this->timer != NULL){
//    delete this->timer;
//  }
}
void VideoDisplay::start(){
  #ifdef DEBUG
  static int i;
  printf("VideoDisplay::start()\n");
  #endif
  //this->timer = new QTimer(this);
  //connect(timer,SIGNAL(timeout()), this, SLOT(update()));
 // #ifdef DEBUG
 // printf("VideoDisplay - connected to Update\n");
 // #endif
  this->timer->start(33);
//  while(1){
//    reader->getNextFrame();
//    emit updateFrame(this->buffer, 32,32);
//  }

}

void VideoDisplay::update(){
  static int i = 0;
  #ifdef DEBUG
  //printf("-- VideoDisplay::update --\n");
  #endif
  reader->getNextFrame();
//  emit outputTest(this->buffer[499]);
  emit updateFrame(this->buffer, 32, 32);
}

void VideoDisplay::stop()
{
  #ifdef DEBUG
  printf("stoping thread narmally\n");
  #endif
  this->timer->stop();

  for(int i = 0; i < 32 * 32 * 3; i++){
      this->buffer[i] = 0;
  }

//  emit updateFrame(this->buffer, 32, 32);
}
