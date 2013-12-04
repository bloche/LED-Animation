/**
 * Definition of the LEDDriver abstract base class. This class defines an interface
 * for displaying an image on any LED display.
 */

#ifndef LEDDRIVER_H
#define LEDDRIVER_H

#include <cstdint>
#include <QObject>

/**
 * @brief The LEDDriver class
 */
class LEDDriver : public QObject
{
    Q_OBJECT
public:
    explicit LEDDriver(QObject *parent=0);
public slots:
    virtual void start() =0;
    virtual void stop() =0;
    virtual int outputFrame(uint8_t *frame, unsigned long width, unsigned long height) =0;
};

#endif // LEDDRIVER_H
