#ifndef VIDEO_CONTROLLER_H
#define VIDEO_CONTROLLER_H

#include <QObject>
#include <QVariant> //don't know if I need this guy
#include "videodisplay.h"
#include "leddriver.h"

class VideoController : public QObject
{
  Q_OBJECT
public:
  explicit VideoController(char*,QObject* parent = 0);

private:
  VideoDisplay* display;
  LEDDriver* driver;
};

#endif //VIDEO_CONTROLLER_H
