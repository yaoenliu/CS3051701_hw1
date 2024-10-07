#pragma once
#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

class Sonic
{
private:
    uint8_t trigPin;
    uint8_t echoPin;
    double rawData;
    TaskHandle_t taskHandle;

public:
    Sonic(uint8_t trigPin, uint8_t echoPin) : trigPin(trigPin), echoPin(echoPin)
    {
        // initialize pins
        pinMode(trigPin, OUTPUT);
        pinMode(echoPin, INPUT);
        // create a task to refresh the distance
        xTaskCreate([](void *pvParameters)
                    {
            Sonic *sonic = (Sonic *)pvParameters;
            while (1)
            {
                sonic->refresh();
                vTaskDelay(5);
            } }, "sonic", 128, this, 1, &taskHandle);

        // default task is suspended
        vTaskSuspend(taskHandle);
    }
    ~Sonic()
    {
        // delete task
        vTaskDelete(taskHandle);
    }
    void start()
    {
        vTaskResume(taskHandle);
    }
    void stop()
    {
        vTaskSuspend(taskHandle);
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
        return rawData / 58.2;
    }
    double inch()
    {
        return rawData / 148;
    }
    double raw()
    {
        return rawData;
    }
};