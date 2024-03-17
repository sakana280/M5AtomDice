// Digital dice.
// Press the screen or shake the unit to "roll" the dice.

// Board: M5Stack-Core-ESP32
// Baud: 115200

#include <Arduino.h>
#include <Math.h>
#include <FastLED.h> // http://fastled.io/ | http://fastled.io/docs/3.1/index.html
#include "MPU6886.h" // couldn't get the I2C_MPU6886 library working: Wire errors on serial

// Various greyscale fonts are defined, only #include one.
#include "font5x5.h"
//#include "font4x5.h"
//#include "font3x3inv.h"

#define BUTTON_PIN 39
#define LED_PIN 27
#define MIN_ROLLING_ACCEL 2.5
#define MIN_SHAKE_INTERVAL_MS 800
//#define DEBUG_ACCEL
#define NUM_LEDS 25
CRGB _leds[NUM_LEDS];

bool demo = true; // run demo until first roll (click/shake)
bool rolling = false; // fast count when rolling (click/shake)
bool draw = true; // draw on change to prevent flicker
unsigned int current = 1;
unsigned long lastIncrement;
unsigned long incrementInterval = 1000; // intitial demo mode interval
unsigned long shakeEnd;
unsigned long seed = 0;
MPU6886 imu;

void drawDigit(int number) {
  Serial.printf("Display %i\n", number);
  for ( int i = 0; i < NUM_LEDS; i++ ) {
    unsigned char grey = DIGITS[number - 1][i];
    _leds[i] = CRGB(grey, grey, grey);
  }
  FastLED.show();
}

void drawBlank() {
  FastLED.showColor(CRGB::Black);
}

float getAccel() {
  float accX = 0, accY = 0, accZ = 0;
  imu.getAccelData(&accX, &accY, &accZ);
  float accel = sqrt(accX*accX + accY*accY + accZ*accZ);
  #ifdef DEBUG_ACCEL
  Serial.printf("ACCEL x=%.3f y=%.3f z=%.3f abs=%.3f\n", accX, accY, accZ, accel);
  #endif
  return accel;
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting...");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  FastLED.addLeds<WS2812, LED_PIN, GRB>(_leds, NUM_LEDS);
  FastLED.setBrightness(20); // max brightness recommended for M5Atom
  drawBlank();
  lastIncrement = shakeEnd = millis();

  imu.Init();
  imu.SetAccelFsr(MPU6886::AFS_2G);
}

void loop() {
  // Spin loop without delay() so that seed can increment rapidly,
  // and to detect clicks immediately.
  
  unsigned long now = millis();

  bool clicking = digitalRead(BUTTON_PIN) == LOW;

  float accel = getAccel();
  if (accel > MIN_ROLLING_ACCEL) {
    shakeEnd = now + MIN_SHAKE_INTERVAL_MS;
  }
  float shaking = now < shakeEnd;
  
  if (demo && !clicking && !shaking) {
    // Count up slowly in startup/demo mode, until first click.
    if (draw) drawDigit(current);
    draw = false; // until next increment
    seed++; // demo duration gives the seed for the random generator
    
  } else if (clicking || shaking) {
    if (demo) randomSeed(seed); // init seed when demo is finished
    if (!rolling) Serial.println("Rolling");
    demo = false;
    rolling = true;
    incrementInterval = 100; // show numbers changing, otherwise they blur into one
    if (draw) drawDigit(current);
    draw = false; // until next increment
    
  } else if (rolling) {
    // Rolling is finished,
    // show a random number so the number can't be gamed by fast clicks.
    current = random(1,6+1);
    Serial.printf("Rolled %i\n", current);
    drawDigit(current);
    rolling = false;
  }

  if (now > lastIncrement + incrementInterval) {
    current++;
    if (current > 6) current = 1;
    draw = true;
    lastIncrement = now;
  }
}
