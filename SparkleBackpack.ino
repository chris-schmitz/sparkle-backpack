#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>
#include <Adafruit_NeoPixel.h>

#ifndef __AVR__
  #include <avr/power.h>
#endif

#define sparkleTrigger 0.50
#define PIN 6

Adafruit_NeoPixel strip = Adafruit_NeoPixel(4, PIN, NEO_GRB + NEO_KHZ800);

Adafruit_LSM303_Accel_Unified accelerometer = Adafruit_LSM303_Accel_Unified(54321);

float currentZValue = 0;
float previousZValue = 0;
int start = 0;

void setup () {
  Serial.begin(9600);
  Serial.println("Starting up the sparkler!");

  strip.begin();
  strip.show();

  if (!accelerometer.begin()) {
    Serial.println("Could not detect accelerometer :/");
    while(1); // I'm assuming this is to lock up output.
  }
}


void loop () {
  // Serial.println("hello");
  // strip.setPixelColor(0, 255, 0, 0);
  // strip.show();
  // delay(500);
  // strip.setPixelColor(0, 0, 0, 0);
  // strip.show();
  // delay(500);


  sensors_event_t event;
  accelerometer.getEvent(&event);

  currentZValue = abs(event.acceleration.z);

  if (start == 0) {
    previousZValue = currentZValue;
    start = 1;
  }

  int i = currentZValue - previousZValue;
  // Serial.println("==================");
  // Serial.print("current z: "); Serial.println(currentZValue);
  // Serial.print("previous z: "); Serial.println(previousZValue);
  // Serial.print("i: "); Serial.println(i);
  // Serial.print("number of pixels: "); Serial.println(strip.numPixels());

  if ( i > sparkleTrigger) {
    // rainbow(1);
    sparkle(30);
  }
  else {
    clearStrip();
  }
  delay(500);
}

void sparkle (uint8_t wait) {
  uint16_t i, j;
  Serial.println("sparkle!");

  // loop through colors
  // for (j = 0 ; j < 256 ; j++) { // all colors
  for (j = 0 ; j < 10 ; j++) {

    // loop through neopixls
    for (i = 0 ; i <= strip.numPixels() ; i ++) {

      // strip.setPixelColor(i, Wheel((i+j) & 127));

      strip.setPixelColor(i, 255, 255, 0, 127);
      strip.show();
      delay(wait);
      strip.setPixelColor(i, 0, 0, 0);
      strip.show();
      delay(wait);
    }
  }
}

// lifted from strand test
// come back and rewrite by hand for better understanding
void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      // great explanation of bitwise operators here: https://www.youtube.com/watch?v=d0AwjSpNXR0
      // here we're saying:
      // for the binary value of the current pixel number and current color value added together,
      // compare it to the binary value of 255 (our color range is 0 through 255, so we're comparing
      // it to all of the bits necessary to make the full range), and only return the bits that are
      // on in both values.
      byte wheelPosition = Wheel((i+j) & 255);
      Serial.print("Wheel position from rainbow"); Serial.println(wheelPosition);
      strip.setPixelColor(i, wheelPosition);
    }
    strip.show();
    delay(wait);
  }
}
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  // I get what's happening here as far as using the wheel position byte
  // to determine the R, G, and B values, but Im' not toally sure why the
  // `... * 3` is in there :/
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void clearStrip () {
  int i;

  for (i = 0 ; i < strip.numPixels() ; i++) {
    strip.setPixelColor(i, 0, 0, 0);
  }
  strip.show();
}