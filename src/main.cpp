#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include "Sonic.h"

#define ECHO 12
#define TRIG 13

#define LED_R 2
#define LED_G 3
#define BUZZER 4

double distance = 0;

Sonic sonic(ECHO, TRIG);

void update(void *pvParameters);
void updateLED();
void updateBuzzer(void *pvParameters);

void setup()
{
  // pin 11 is used to power the sensor
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);

  // initialize other pins
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // initialize serial
  Serial.begin(9600);

  // setup tasks
  xTaskCreate(update, "update", 128, NULL, 1, NULL);
  xTaskCreate(updateBuzzer, "updateBuzzer", 128, NULL, 1, NULL);
  // start scheduler
  vTaskStartScheduler();
}

void loop()
{

}

// read distance and update LED (task)
void update(void *pvParameters)
{
  (void)pvParameters;
  while (1)
  {
    distance = sonic.cm();
    Serial.print(distance);
    Serial.println(" cm");
    updateLED();
    vTaskDelay(20);
  }
}

// update buzzer based on distance (task)
void updateBuzzer(void *pvParameters)
{
  (void)pvParameters;
  while (1)
  {
    if (distance < 20)
    {
      digitalWrite(BUZZER, HIGH);
    }
    else if (distance < 50)
    {
      digitalWrite(BUZZER, LOW);
      vTaskDelay(5);
      digitalWrite(BUZZER, HIGH);
      vTaskDelay(3);
    }
    else
    {
      digitalWrite(BUZZER, LOW);
    }
  }
}

// update LED based on distance
void updateLED()
{
  if (distance < 20)
  {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_G, LOW);
  }
  else if (distance < 50)
  {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, HIGH);
  }
  else
  {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_G, LOW);
  }
}