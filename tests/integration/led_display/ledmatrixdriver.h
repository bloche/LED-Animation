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

/**
 * @brief The LEDMatrixDriver class
 */
class LEDMatrixDriver : public LEDDriver
{
    Q_OBJECT
public:
    explicit LEDMatrixDriver(QObject *parent = 0);
    int GetMatrixCount();
    void SetMatrixCount(int count);

public slots:
    int outputFrame(uint8_t *frame, unsigned long width, unsigned long height);
    void start();
    void stop();

private slots:
    void output();

private:
    int matrixCount;
    QTimer *timer;
    uint8_t *buffer;
    uint8_t *nextbuffer;
    unsigned int currentRow;
    uint8_t globalMask;
    uint8_t redLookupTable[16];
    uint8_t blueLookupTable[16];
    uint8_t greenLookupTable[16];
};

#endif // LEDMATRIXDRIVER_H
