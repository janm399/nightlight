#include <Arduino.h>
#include <FastLED.h>
#include "pins.h"

static const int maxBrightness = 100;
static const int peeDelay = 2000;  // 1000 * 60 * 10;
static bool lightUp = false;
static const int LED_COUNT = 28;
static CRGB leds[LED_COUNT];

void setColour() {
  for (int i = 0; i < LED_COUNT; i++) leds[i] = CRGB(0x00ff00);
}

void irDetected() {
  lightUp = true;
  Serial.println("Intr");
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN>(leds, LED_COUNT);
  attachInterrupt(digitalPinToInterrupt(PIR_SENSOR_PIN), irDetected, RISING);
}

void loop() {
  if (lightUp) {
    setColour();
    for (int i = 0; i < maxBrightness; i++) {
      FastLED.setBrightness(i);
      FastLED.show();
      delay(50);
    }
    delay(peeDelay);
    for (int i = maxBrightness; i >= 0; i--) {
      FastLED.setBrightness(i);
      FastLED.show();
      delay(50);
    }
    lightUp = false;
  }
}
