// **************** COLOUR DETECTION AND TRANSMISSION ******************
//
// Reads colour values from the colour sensor. Depending upon the value,
// assign main colours (yellow, red, blue, black). Calibration maybe required.
// Turn on and off digital pins depending upon the colour.

#include <Wire.h>
#include "Adafruit_TCS34725.h"

/* Initialise with specific int time and gain values */
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);


int rMin  = 255;
int rMax = 0;
int gMin  = 255;
int gMax = 0;
int bMin  = 255;
int bMax = 0;

int cal = false;

void setup(void) {
  Serial.begin(9600);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  
  if (tcs.begin()) {
    Serial.println("Found sensor");
  } else {
    Serial.println("No TCS34725 found ... check your connections");
    while (1);
  }
}

void loop(void) {

  if (cal == false)
  {
    for (unsigned long i = 0; i < 300; i++)
    {
      cali();
      digitalWrite(LED_BUILTIN, HIGH);
      delay(1);
      digitalWrite(LED_BUILTIN, LOW);
    }
    cal = true;
  }
  
  uint16_t red, green, blue, c, colorTemp, lux;
  tcs.getRawData(&red, &green, &blue, &c);
  colorTemp = tcs.calculateColorTemperature(red, green, blue);
  lux = tcs.calculateLux(red, green, blue);

  red= map(red,rMin,rMax,0,255);
  red = constrain(red,0,255);

  green= map(green,gMin,gMax,0,255);
  green = constrain(green,0,255);

  blue = map(blue,bMin,bMax,0,255);
  blue = constrain(blue,0,255);

  digitalWrite(9, LOW);
  digitalWrite(10, LOW);
  digitalWrite(11, LOW);
  digitalWrite(12, LOW);

  Serial.print("red = ");
  Serial.println(red);
  Serial.print("blue = ");
  Serial.println(blue);
  Serial.print("green = ");
  Serial.println(green);
  
  if (red < 70 && blue < 100 && blue > 10 && green < 70 && green > 10 ) // BLUE
  {
    Serial.println("BLUE");
    digitalWrite(9, HIGH);
  }
   if (red > 70 && blue < 70 && green < 70) //RED
  {
    Serial.println("RED");
    digitalWrite(10, HIGH);
    delay(10);
  }
//  if (red < 20 && blue < 20) // BLACK
//  {
//    Serial.println("BLACK");
//    digitalWrite(11, HIGH);
//  }
  if (red <= 255 && red > 180 && green <= 255 && green > 140 && blue > 70 && blue < 120) // YELLOW
  {
    Serial.println("YELLOW");
    digitalWrite(12, HIGH);
    delay(10);
  }
}


void cali() {
  uint16_t red, green, blue, c, colorTemp, lux;
  tcs.getRawData(&red, &green, &blue, &c);
  
      // record the maximum sensor value
      if (red > rMax) {
        rMax = red;
      }
      // record the minimum sensor value
      if (red < rMin) {
        rMin = red;
      }
      
      if (green > gMax) {
        gMax = green;
      }
      // record the minimum sensor value
      if (green < gMin) {
        gMin = green;
      }

      if (blue > bMax) {
        bMax = blue;
      }
      // record the minimum sensor value
      if (blue < bMin) {
        bMin = blue;
      }
}
