#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
// a rtos tick is 16ms
#include "Sonic.h"

#define ECHO 12
#define TRIG 13

#define LED_R A5
#define LED_G A3
#define BUZZER A0

double distance = 0;

Sonic sonic(ECHO, TRIG);

void update(void *pvParameters);
void updateLED();
void updateBuzzer(void *pvParameters);

void setup()
{
  // Set some digital pins as power supply or ground
  pinMode(11, OUTPUT);
  digitalWrite(11, HIGH);
  pinMode(A2, OUTPUT);
  digitalWrite(A2, LOW);
  pinMode(A4, OUTPUT);
  digitalWrite(A4, LOW);

  // initialize output pins
  pinMode(LED_R, OUTPUT);
  pinMode(LED_G, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  // initialize serial
  Serial.begin(115200);
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
    Serial.println(String(distance) + " cm");
    updateLED();
    vTaskDelay(5);
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