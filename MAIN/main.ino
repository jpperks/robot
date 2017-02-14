//#include <Adafruit_TCS34725.h>
#include <Wire.h>
#include <QTRSensors.h>
#include <Servo.h>

// 94 =  LEFT Stopped Counterclockwise.
// 93 =  RIGHT Stopped Clockwise.

// 104 =  LEFT Full Speed Clockwise.
// 83 =  RIGHT Full Speed Counterclockwise.

Servo servol;
Servo servor;// create servo object to control a servo
QTRSensorsRC qtr((char[]) {2, 3, 4, 5, 6, 7, 8, 9}, 8); // {Digital Pin Numbers}

//byte gammatable[256];
//Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);
  servor.attach(0);
  servol.attach(1);// attaches the servo on pin 9 to the servo object

  // IR Sensor Calibration
  int i;
  for (i = 0; i < 250; i++) // make the calibration take about 5 seconds
  {
  qtr.calibrate();
  digitalWrite(LED_BUILTIN, HIGH);
  delay(20);
  digitalWrite(LED_BUILTIN, LOW);
  }

  //if (tcs.begin()) {
   // Serial.println("Found sensor");
  //} else {
  //  Serial.println("No TCS34725 found ... check your connections");
  //  while (1); // halt!
  //}
  
}

void loop() {
  digitalWrite(13, HIGH);
  //
  // uint16_t clear, red, green, blue;
  //
  //  tcs.setInterrupt(false);      // turn on LED
  //
  //  delay(60);  // takes 50ms to read 
  //  
  //  tcs.getRawData(&red, &green, &blue, &clear);
  //
  //  tcs.setInterrupt(true);  // turn off LED
  //  
  //  Serial.print("C:\t"); Serial.print(clear);
  //  Serial.print("\tR:\t"); Serial.print(red);
  //  Serial.print("\tG:\t"); Serial.print(green);
  //  Serial.print("\tB:\t"); Serial.print(blue);

  int turns;
  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  if (sensors[0] > 0)
  {digitalWrite(LED_BUILTIN, HIGH); delay(50); digitalWrite(LED_BUILTIN, LOW);}
  int error = position - 1000;


  // Write all sensor data to the serial monitor. Tools > Serial Monitor (baud 9600)
  //
  Serial.print("Sensor 0 is: ");
  Serial.print(sensors[0]);
  Serial.println();
  Serial.print("Sensor 1 is: ");
  Serial.print(sensors[1]);
  Serial.println();
  Serial.print("Sensor 2 is: ");
  Serial.print(sensors[2]);
  Serial.println();
  Serial.print("Sensor 3 is: ");
  Serial.print(sensors[3]);
  Serial.println();
  Serial.print("Sensor 4 is: ");
  Serial.print(sensors[4]);
  Serial.println();
  Serial.print("Sensor 5 is: ");
  Serial.print(sensors[5]);
  Serial.println();
  Serial.print("Sensor 6 is: ");
  Serial.print(sensors[6]);
  Serial.println();
  Serial.print("Sensor 7 is: ");
  Serial.print(sensors[7]);
  Serial.println();
  
  //
  // START LINE JUMP CODE
  //
  // SENSOR = 0 = OVER WHITE 
  // SENSOR > ~200 = OVER BLACK
  //
  if (sensors[1] < 5 && sensors[2] < 5 && sensors[3] < 5 && sensors[4] < 5 && sensors[5] < 5 && sensors[6] < 5 && sensors[7] < 5)
  {
    if (turns == 0)
    {
      servol.write(104); // Full Speed L
      servor.write(93); // Stopped R
      delay(150);
      turns = 1;
    }
    if (turns == 1)
    {
      servol.write(94); // Stopped L
      servor.write(83); Full Speed R
      delay(150);
      turns = 0;
    }
  }
  //
  // END LINE JUMP CODE
  //

  
  servor.write(83); // Full Speed R
  servol.write(104); // Full Speed L
  if (error < 3000 ){
  servor.write(93); // Stopped R
  turns = 1;
  }
  if (error > 3000){
  servol.write(94); // Stopped L
  turns = 0;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
}
