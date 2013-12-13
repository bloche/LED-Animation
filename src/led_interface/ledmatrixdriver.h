/**

 * Concrete implementation of the LEDDriver class for the 32x32 LED Matrices available
 * from Adafruit.
 */

#ifndef LEDMATRIXDRIVER_H
#define LEDMATRIXDRIVER_H

#include "leddriver.h"
#include <cstdint>
#include <QObject>
#include <QTimer>
#include "../video_stream_interface/videoreaderdelegate.h"
/**
 * @brief The LEDMatrixDriver class
 */
class LEDMatrixDriver : public LEDDriver
{
    Q_OBJECT
public:
    explicit LEDMatrixDriver(char* filename, int flag, QObject *parent = 0);
    int GetMatrixCount();
    void SetMatrixCount(int count);

public slots:
    void inputTest(int input);
    void outputFrame(uint8_t *frame, int width, int height);
    void start();
    void stop();

private slots:
    void output();

private:
    VideoReaderDelegate* videoReader;
    int matrixCount;
    int frameCount;
    QTimer *timer;
    uint8_t *buffer;
    uint8_t *nextbuffer;
    unsigned int currentRow;
    int globalMask;
    uint8_t globalMaskB;
    int redLookupTable[16];
    //uint8_t blueLookupTable[16];
    // uint8_t greenLookupTable[16];
};

#endif // LEDMATRIXDRIVER_H
