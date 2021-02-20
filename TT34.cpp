#include "TT34.h"

TT34::TT34(unsigned int pin, unsigned int timeout)
{
    _pin = pin;
    _timeout = timeout;
    pinMode(pin, INPUT);
}

float TT34::getTemperature()
{
    if (waitForChange(_timeout) < 0)
    {
        return -1.0;
    }

    readValues();

    const int shift = findShift();
    const unsigned int toggle = findToggle(shift);

    const float temperature = parse(shift, toggle);

    if (0 < temperature && temperature < 250)
    {
        return temperature;
    }
    else
    {
        return -2.0;
    }
}

int TT34::waitForChange(unsigned int timeout)
{
    unsigned int start = micros();

    unsigned int counter = 0;
    unsigned int t0 = 0;
    unsigned int duration = 0;
    unsigned int initial = 0;

    const unsigned int requiredCounts = 6;

    while (counter < requiredCounts && start <= micros() && micros() <= start + timeout)
    {
        t0 = micros();
        initial = digitalRead(_pin);
        while (digitalRead(_pin) == initial && start <= micros() && micros() <= start + timeout)
        {
        }

        duration = micros() - t0;

        if ((940 < duration) and (duration < 1140))
        {
            ++counter;
        }
        else
        {
            counter = 0;
        }
    }

    if (counter < requiredCounts)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

void TT34::readValues()
{
    delayMicroseconds(1038 / 2);

    for (int i = 0; i < numValues; ++i)
    {
        _values[i] = digitalRead(_pin);
        delayMicroseconds(1038 - 4);
    }
}

int TT34::findShift()
{
    for (int i = 1; i < numValues; ++i)
    {
        if (_values[i] == _values[i - 1])
        {
            return i - 7;
        }
    }
}

unsigned int TT34::findToggle(int shift)
{
    return _values[shift + 7];
}

float TT34::parse(int shift, unsigned int toggle)
{
    float result = 0.0;

    for (int i = 0; i < 4; ++i)
    {
        result += 0.100 * float((toggle ^ _values[27 + i + shift]) << i);
    }

    for (int i = 0; i < 4; ++i)
    {
        result += 1.000 * float((toggle ^ _values[31 + i + shift]) << i);
    }

    for (int i = 0; i < 4; ++i)
    {
        result += 10.00 * float((toggle ^ _values[37 + i + shift]) << i);
    }

    for (int i = 0; i < 4; ++i)
    {
        result += 100.0 * float((toggle ^ _values[41 + i + shift]) << i);
    }

    return result;
}
