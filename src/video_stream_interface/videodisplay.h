#ifndef VIDEO_DISPLAY_H
#define VIDEO_DISPLAY_H

#include <QObject>
#include <QTimer>
#include <stdint.h>
#include "videoreader.h"

class VideoDisplay : public QObject
{
  Q_OBJECT
public:
  explicit VideoDisplay(char* fileName, QObject *parent = 0);
  ~VideoDisplay();

signals:
  void finished();
  void outputTest(int input);
  void updateFrame(uint8_t *frame, int width, int height);

public slots:
  void start();
  void stop();

private slots:
  void update();

private:
  VideoReaderDelegate* reader;
  uint8_t* buffer;
  QTimer* timer;
};

#endif //VIDEO_DISPLAY_H
