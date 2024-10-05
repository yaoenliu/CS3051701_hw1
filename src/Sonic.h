#pragma once
#include <Arduino.h>

class Sonic
{
private:
    uint8_t trigPin;
    uint8_t echoPin;
    double rawData;

public:
    Sonic(uint8_t trigPin, uint8_t echoPin) : trigPin(trigPin), echoPin(echoPin)
    {
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
    }
    void refresh()
    {
        digitalWrite(trigPin, HIGH);
        delayMicroseconds(10);
        digitalWrite(trigPin, LOW);
        pinMode(echoPin, INPUT);
        rawData = pulseIn(echoPin, HIGH);
    }
    double cm()
    {
        refresh();
        return rawData / 58.2;
    }
    double inch()
    {
        refresh();
        return rawData / 148;
    }
    double raw()
    {
        refresh();
        return rawData;
    }
};