#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Example code for the Adafruit TCS34725 breakout library */

/* Connect SCL    to analog 5
   Connect SDA    to analog 4
   Connect VDD    to 3.3V DC
   Connect GROUND to common ground */
   
/* Initialise with default values (int time = 2.4ms, gain = 1x) */
// Adafruit_TCS34725 tcs = Adafruit_TCS34725();

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_700MS, TCS34725_GAIN_1X);

void setup(void) {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
  
  // Now we're ready to get readings!
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;
  
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature(r, g, b);
  lux = tcs.calculateLux(r, g, b);

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);
 
  Serial.println(c);
  
  if (c >= 5500 && c < 8500) // BLUE
  {
    Serial.println("BLUE");
    digitalWrite(9, HIGH);
    delay(10);
  }
   if (c >= 10000 && c < 13000) //RED
  {
    Serial.println("RED");
    digitalWrite(10, HIGH);
    delay(10);
  }
  if (c < 5000) // BLACK
  {
    Serial.println("BLACK");
    digitalWrite(11, HIGH);
    delay(10);
  }
  if (c >= 20000 && c < 40000) // YELLOW
  {
    Serial.println("YELLOW");
    digitalWrite(12, HIGH);
    delay(10);
  }
}
