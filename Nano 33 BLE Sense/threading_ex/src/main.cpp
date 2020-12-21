#include <Arduino.h>
#include "mbed.h"
#include "rtos.h"

using namespace rtos;

Thread blinker;

void blink() {
  while (1) {
    digitalWrite(LED_BUILTIN, HIGH);
    thread_sleep_for(1000);
    digitalWrite(LED_BUILTIN, LOW);
    thread_sleep_for(1000);
  }
}

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  blinker.start(mbed::callback(blink));
}

void loop() {
  // put your main code here, to run repeatedly:
}