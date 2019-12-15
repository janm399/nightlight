#include <Arduino.h>
#include <FastLED.h>
#include <avr/interrupt.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include "pins.h"

static const int maxBrightness = 200;
static const int peeDelay = 2000;  // 1000 * 60 * 10;
static const int lightTrigger = 110;

static bool lightUp = false;
static const int LED_COUNT = 28;
static CRGB leds[LED_COUNT];
static CRGB kelvinTable[] = {CRGB(0xff3800), CRGB(0xff4700), CRGB(0xff5300)};

void setColour() {
  for (int i = 0; i < LED_COUNT; i++) leds[i] = kelvinTable[1];
}

ISR(PCINT0_vect) { lightUp = true; }

void setup() {
  clock_prescale_set(clock_div_1);
  pinMode(PIR_SENSOR_PIN, INPUT);
  GIMSK = _BV(PCIE);
  PCMSK = _BV(PIR_SENSOR_PIN);
  MCUCR |= (1 << ISC01) | (1 << ISC00);
  ADCSRA |= (1 << ADEN);
  sei();
  pinMode(3, OUTPUT);
  FastLED.addLeds<NEOPIXEL, LED_STRIP_PIN>(leds, LED_COUNT);
  setColour();
  FastLED.setBrightness(0);
  FastLED.show();
}

void sleep() {
  GIMSK = _BV(PCIE);
  PCMSK = _BV(PIR_SENSOR_PIN);
  ADCSRA &= ~_BV(ADEN);
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);

  sleep_enable();
  sei();
  sleep_cpu();

  cli();
  PCMSK &= ~_BV(PIR_SENSOR_PIN);
  sleep_disable();
  ADCSRA |= _BV(ADEN);

  sei();
}

void loop() {
  sleep();
  const auto light = analogRead(A1);
  // FastLED.setBrightness(100);
  // for (int i = 0; i < LED_COUNT; i++)
  //   if (i * 37 < light)
  //     leds[i] = kelvinTable[1];
  //   else
  //     leds[i] = CRGB(0);
  // FastLED.show();
  // return;
  if (lightUp) {
    if (light < lightTrigger) {
      setColour();
      for (int i = 0; i < maxBrightness; i++) {
        FastLED.setBrightness(i);
        FastLED.show();
        delay(100);
      }
      delay(peeDelay);
      for (int i = maxBrightness; i >= 0; i--) {
        FastLED.setBrightness(i);
        FastLED.show();
        delay(200);
      }
    }
    lightUp = false;
  }
}

// #include <Adafruit_NeoPixel.h>
// #include <Arduino.h>
// #include <avr/power.h>

// static const int maxBrightness = 200;
// static const int peeDelay = 2000;  // 1000 * 60 * 10;
// static const int lightTrigger = 0;

// static bool lightUp = false;
// static const int LED_COUNT = 28;
// Adafruit_NeoPixel strip = Adafruit_NeoPixel(LED_COUNT, 3, NEO_GRB +
// NEO_KHZ800);

// ISR(PCINT0_vect) { lightUp = true; }

// void setup() {
//   clock_prescale_set(clock_div_1);
//   GIMSK = _BV(PCIE);
//   PCMSK = _BV(PCINT4);
//   GIFR = GIFR | 0x40;
//   MCUCR |= (1 << ISC01) | (1 << ISC00);  // Rising edge
//   sei();

//   strip.begin();
//   strip.setBrightness(100);
//   strip.show();
// }

// void loop() {
//   delay(100);
//   if (lightUp) {
//     cli();
//     for (int i = 0; i < LED_COUNT; i++)
//       strip.setPixelColor(i, 0xff, 0x47, 0x00);
//     strip.show();

//     delay(1000);
//     lightUp = false;
//     for (int i = 0; i < LED_COUNT; i++) strip.setPixelColor(i, 0);
//     strip.show();
//     sei();
//   }
//   return;
//   if (lightUp) {
//     const auto light = 100;  // analogRead(2);
//     if (light > lightTrigger) {
//       for (int i = 0; i < maxBrightness; i++) {
//         for (int i = 0; i < LED_COUNT; i++)
//           strip.setPixelColor(i, 0xff, 0x47, 0x00);
//         strip.show();
//         delay(100);
//       }
//       delay(peeDelay);
//       for (int i = maxBrightness; i >= 0; i--) {
//         for (int i = 0; i < LED_COUNT; i++)
//           strip.setPixelColor(i, 0xff, 0x47, 0x00);
//         strip.show();
//         delay(200);
//       }
//     }
//     lightUp = false;
//   }
// }
