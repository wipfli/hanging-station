#ifndef TT34_H
#define TT34_H

#include "Arduino.h"

const unsigned int numValues = 70;

class TT34
{
public:
    // digital pin which connects to 433 MHz receiver
    // timeout in microseconds
    TT34(unsigned int pin, unsigned int timeout);

    // returns temperature in degree C
    // returns -1 if timeout
    // returns -2 if temperature out of range
    float getTemperature();

private:
    unsigned int _pin;
    unsigned int _timeout;

    unsigned int _values[numValues];

    // returns -1 if timeout, 0 otherwise
    int waitForChange(unsigned int timeout);

    void readValues();

    int findShift();

    unsigned int findToggle(int shift);

    float parse(int shift, unsigned int toggle);
};

#endif
