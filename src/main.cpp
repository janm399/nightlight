#include <Arduino.h>
#include <FastLED.h>
#include "pins.h"

static const int LED_COUNT = 8;
static CRGB leds[LED_COUNT];

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);

  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN>(leds, LED_COUNT);
  FastLED.setBrightness(20);
}

void loop() {
  for (int i = 0; i < LED_COUNT; i++) {
    leds[i] = CRGB(0x00ff00);
    FastLED.show();
    delay(200);
  }

  FastLED.show();
}
