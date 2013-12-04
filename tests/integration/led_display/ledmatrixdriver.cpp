 /**
 * Implementation of the LEDMatrixDriver class.
 */

#include "ledmatrixdriver.h"
#ifndef USE_STD_OUT
#include <wiringPi.h>
#endif
#include <QDebug>
#include <QObject>
#include <QThread>
#include <cstdint>
#include <stdio.h>
#define OE      1
#define SCLK    16
#define LAT     4
#define A       2
#define B       6
#define C       15
#define D       5
#define R1      14
#define G1      13
#define B1      11
#define R2      12
#define G2      3
#define B2      10
#define CALC_BUFFER_OFFSET(X,Y,WIDTH) ((((Y) * (WIDTH)) * 3) + ((X) * 3))

#define DEBUG

/**
 * @brief LEDMatrixDriver::LEDMatrixDriver
 */
LEDMatrixDriver::LEDMatrixDriver(QObject *parent) :
    LEDDriver(parent)
{
#ifndef USE_STD_OUT
    #ifdef DEBUG
    printf("Constructing LEDMatrixDriver\n");
    #endif
    wiringPiSetup();


    piHiPri(0); 

    pinMode(OE, OUTPUT);
    pinMode(SCLK, OUTPUT);
    pinMode(LAT, OUTPUT);
    pinMode(A, OUTPUT);
    pinMode(B, OUTPUT);
    pinMode(C, OUTPUT);
    pinMode(D, OUTPUT);

    pinMode(R1, OUTPUT);
    pinMode(G1, OUTPUT);
    pinMode(B1, OUTPUT);
    pinMode(R2, OUTPUT);
    pinMode(G2, OUTPUT);
    pinMode(B2, OUTPUT);

    digitalWrite(SCLK, LOW);
    digitalWrite(LAT, LOW);
    digitalWrite(OE, HIGH);
    digitalWrite(A, LOW);
    digitalWrite(B, LOW);
    digitalWrite(C, LOW);
    digitalWrite(D, LOW);
    digitalWrite(R1, LOW);
    digitalWrite(R2, LOW);
    digitalWrite(G1, LOW);
    digitalWrite(G2, LOW);
    digitalWrite(B1, LOW);
    digitalWrite(B2, LOW);
    digitalWrite(OE, LOW);
#endif

    this->matrixCount = 1;
    this->buffer = new uint8_t[32 * 32 * 1 * 3]();
    this->nextbuffer = this->buffer;
    this->timer = new QTimer(this);
    connect(this->timer, SIGNAL(timeout()), this, SLOT(output()));
    this->currentRow = 0;
    this->globalMask = 1;

    this->redLookupTable[0] = 0x00;
    this->redLookupTable[1] = 0x00;
    this->redLookupTable[2] = 0x00;
    this->redLookupTable[3] = 0x88;
    this->redLookupTable[4] = 0x88;
    this->redLookupTable[5] = 0x88;
    this->redLookupTable[6] = 0xcc;
    this->redLookupTable[7] = 0xcc;
    this->redLookupTable[8] = 0xcc;
    this->redLookupTable[9] = 0xcc;
    this->redLookupTable[10] = 0xee;
    this->redLookupTable[11] = 0xee;
    this->redLookupTable[12] = 0xee;
    this->redLookupTable[13] = 0xff;
    this->redLookupTable[14] = 0xff;
    this->redLookupTable[15] = 0xff;

    this->greenLookupTable[0] = 0x00;
    this->greenLookupTable[1] = 0x00;
    this->greenLookupTable[2] = 0x00;
    this->greenLookupTable[3] = 0x44;
    this->greenLookupTable[4] = 0x44;
    this->greenLookupTable[5] = 0x44;
    this->greenLookupTable[6] = 0x33;
    this->greenLookupTable[7] = 0x33;
    this->greenLookupTable[8] = 0x33;
    this->greenLookupTable[9] = 0x33;
    this->greenLookupTable[10] = 0x77;
    this->greenLookupTable[11] = 0x77;
    this->greenLookupTable[12] = 0x77;
    this->greenLookupTable[13] = 0xff;
    this->greenLookupTable[14] = 0xff;
    this->greenLookupTable[15] = 0xff;

    this->blueLookupTable[0] = 0x00;
    this->blueLookupTable[1] = 0x00;
    this->blueLookupTable[2] = 0x00;
    this->blueLookupTable[3] = 0x11;
    this->blueLookupTable[4] = 0x11;
    this->blueLookupTable[5] = 0x11;
    this->blueLookupTable[6] = 0x66;
    this->blueLookupTable[7] = 0x66;
    this->blueLookupTable[8] = 0x66;
    this->blueLookupTable[9] = 0x66;
    this->blueLookupTable[10] = 0xdd;
    this->blueLookupTable[11] = 0xdd;
    this->blueLookupTable[12] = 0xdd;
    this->blueLookupTable[13] = 0xff;
    this->blueLookupTable[14] = 0xff;
    this->blueLookupTable[15] = 0xff;

}

/**
 * @brief Implementation of the OutputFrame function for the 32x32 LED Matrices available
 *        from Adafruit. It works by constantly shifting out data to the matrix (or matrices)
 *        in a seperate thread. The seperate thread is required because this function must
 *        guarantee that the frame is constantly shown and only two rows can actually be
 *        shown at a time. So the rows must be iterated through fast enough for the eye to
 *        not be able to percieve it.
 * @param width The width the output frame.
 * @param height The height of the output frame.
 * @return If an error occured it returns a nonzero integer, otherwise it returns 0.
 */
int LEDMatrixDriver::outputFrame(uint8_t *frame, unsigned long width, unsigned long height)
{
    #ifdef DEBUG
    printf("-- ENTER LEDMatrixDriver OutputFrame --\n");
    #endif
    if (height != 32 || width % 32 != 0) {
        qDebug() << "only horizontals";
        throw "Only horizontal configurations are supported.";
    }
    if (width != (unsigned long)this->matrixCount * 32) {
        qDebug() << "frame size does not match";
        throw "Frame size does not match matrix configuration.";
    }

    memcpy(this->nextbuffer, frame, sizeof(uint8_t) * width * height * 3);

    return 0;
}

void LEDMatrixDriver::start()
{
    #ifdef DEBUG
    printf("LEDMatrixDriver started\n");
    #endif
    // Start the timer
    this->timer->start(1);
}

void LEDMatrixDriver::stop()
{
    #ifdef DEBUG
    printf("Stopping thread normally\n");
    #endif
    // Stop the timer
    this->timer->stop();
}

/**
 * @brief Writes one new row of information from the buffer to the led matrices.
 */
void LEDMatrixDriver::output()
{

    #ifdef DEBUG
    printf("--LEDMatrixDriver::output\n");
    #endif
#ifndef USE_STD_OUT
//int i = 0;
//for(;i< 1;i++){
    digitalWrite(OE, HIGH);
    // select the row
    digitalWrite(A, this->currentRow & 0x01);
    digitalWrite(B, this->currentRow & 0x02);
    digitalWrite(C, this->currentRow & 0x04);
    digitalWrite(D, this->currentRow & 0x08);

    // unlatch
    digitalWrite(LAT, LOW);

    // Clock in some data
    int column;
    int width = this->matrixCount * 32;
    for (column = 0; column < width; column++) {
        uint8_t pixel1r = this->buffer[CALC_BUFFER_OFFSET(column, this->currentRow, width) + 0];
        uint8_t pixel1g = this->buffer[CALC_BUFFER_OFFSET(column, this->currentRow, width) + 1];
        uint8_t pixel1b = this->buffer[CALC_BUFFER_OFFSET(column, this->currentRow, width) + 2];
        uint8_t pixel2r = this->buffer[CALC_BUFFER_OFFSET(column, this->currentRow + 16, width) + 0];
        uint8_t pixel2g = this->buffer[CALC_BUFFER_OFFSET(column, this->currentRow + 16, width) + 1];
        uint8_t pixel2b = this->buffer[CALC_BUFFER_OFFSET(column, this->currentRow + 16, width) + 2];

        pixel1r = (this->redLookupTable[ pixel1r >> 4] & globalMask) == 0 ? 0 : 1;
        pixel1b = (this->blueLookupTable[ pixel1b >> 4] & globalMask) == 0 ? 0 : 1;
        pixel1g = (this->greenLookupTable[ pixel1g >> 4] & globalMask) == 0 ? 0 : 1;
        pixel2r = (this->redLookupTable[ pixel2r >> 4] & globalMask) == 0 ? 0 : 1;
        pixel2b = (this->blueLookupTable[ pixel2b >> 4] & globalMask) == 0 ? 0 : 1;
        pixel2g = (this->greenLookupTable[ pixel2g >> 4] & globalMask) == 0 ? 0 : 1;

	digitalWrite(R1, pixel1r);
	digitalWrite(G1, pixel1g);
	digitalWrite(B1, pixel1b);
	digitalWrite(R2, pixel2r);
	digitalWrite(G2, pixel2g);
	digitalWrite(B2, pixel2b);

   //     pixel1r = pixel1r < 128 ? 0 : 0xff;
  //      pixel1g = pixel1g < 128 ? 0 : 0xff;
  //      pixel1b = pixel1b < 128 ? 0 : 0x00;
  //      pixel2r = pixel2r < 128 ? 0 : 0xff;
  //      pixel2g = pixel2g < 128 ? 0 : 0xff;
  //      pixel2b = pixel2b < 128 ? 0 : 0x00;

        // Pulse the clock
        digitalWrite(SCLK, LOW);
        digitalWrite(SCLK, HIGH);
    }

    this->currentRow++;
    if (this->currentRow == 16) {
        this->currentRow = 0;
        this->globalMask = this->globalMask << 1;
        if(this->globalMask > 8){
          uint8_t *temp = this->buffer;
          this->buffer = this->nextbuffer;
          this->nextbuffer = temp;
	  this->globalMask = 1;
          //this->currentRow = 0;
        }
	}
    // Increment row
//    }//the for statement for the pulse-width modulation

    //increment globalMask
//    this->globalMask = this->globalMask << 1;
//    if(this->globalMask == 0){
//        this->globalMask = 1;
//    }
// I think this is the wrong place for this...

    // latchKTEC For The Win!!
    digitalWrite(LAT, HIGH);
    //digitalWrite(LAT, LOW);
    digitalWrite(OE, LOW);

#endif
}

void LEDMatrixDriver::SetMatrixCount(int count)
{
    #ifdef DEBUG
    printf("--LEDMatrixDriver::SetMatrixCount--\n");
    #endif
    qDebug() << "Setting new matrix count: " << count;
    this->buffer = new uint8_t[32 * 32 * count * 3]();
    this->matrixCount = count;
}

int LEDMatrixDriver::GetMatrixCount()
{
    #ifdef DEBUG
    printf("--LEDMatrixDriver::GetMatrixCount--\n");
    #endif
    return this->matrixCount;
}
