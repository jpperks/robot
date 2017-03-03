#include <QTRSensors.h>
#include <Wire.h>
#include <Servo.h>

Servo servol;
Servo servor;// create servo object to control a servo

QTRSensorsRC qtr((char[]) {2, 3, 4, 5, 6, 7, 8, 9}, 8); // {Digital Pin Numbers}

#define BLUE A0
#define RED A1
#define BLACK A2
#define YELLOW A3

void setup() {
  Serial.begin(9600);

  pinMode(10, OUTPUT);
  pinMode(13, OUTPUT);
  
  pinMode(BLUE, INPUT);
  pinMode(RED, INPUT);
  pinMode(YELLOW, INPUT);
  pinMode(BLACK, INPUT);
  
  digitalWrite(10, HIGH);

  // IR Sensor Calibration
  int i;
  for (i = 0; i < 250; i++) // make the calibration take about 5 seconds
  {
  qtr.calibrate();
  qtr.emittersOn();
  //digitalWrite(A5, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(20);
  digitalWrite(LED_BUILTIN, LOW);
  //digitalWrite(A5, LOW);
  }
  
  servor.attach(11);
  servol.attach(12);// attaches the servo on pin 9 to the servo object


}

void loop() {

  digitalWrite(LED_BUILTIN, LOW);
  //digitalWrite(A5, LOW);
  int turns;
  int byteRead;
  int incomingByte = 0;
  digitalWrite(10, HIGH);   
  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  int error = position - 1000;
  
  //SensorPrint();
  ColourPrint();
  
  
  if (sensors[0] >= 0)
  {digitalWrite(LED_BUILTIN, HIGH); delay(50); digitalWrite(LED_BUILTIN, LOW);}

  servor.write(83);       //goes straight if it doesnt match anything
  servol.write(104);

// 94 =  LEFT Stopped Counterclockwise.
// 93 =  RIGHT Stopped Clockwise.

// 104 =  LEFT Full Speed Clockwise.
// 83 =  RIGHT Full Speed Counterclockwise.

  if (error > 3000){
       servol.write(94); // Stopped L, Turning Left
       turns = 0;
  }
  if (error<3000){
       servor.write(93); // Stopped R, Turning Right
       turns = 1;
  }

  if (digitalRead(YELLOW) == HIGH)
  {
    Serial.println("Reading Yellow");
    Colour(turns, 0);
  }

if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
{
  for (unsigned int i = 0; i < 20; i++)
  {
    //SensorPrint();
    servor.write(83);       //goes straight if it doesnt match anything
    servol.write(104);
    int turns;
    unsigned int sensors[8];
    int position = qtr.readLine(sensors);
    int error = position - 1000;
    digitalWrite(10, HIGH);

    if (sensors[1] == 1000 || sensors[2] == 1000 || sensors[3] == 1000 || sensors[4] == 1000 || sensors[5] == 1000 || sensors[6] == 1000 || sensors[7] == 1000)
    { 
      break;
    }
    delay(1);
  }
  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  { 
    if (Gap() == true)
    {
      Drive();
    }
  }
}
}

void Drive()
{
  //digitalWrite(A0, LOW);
  while(1)
  {
    ColourPrint();
    int turns;
    unsigned int sensors[8];
    int position = qtr.readLine(sensors);
    int error = position - 1000;
    digitalWrite(10, HIGH);
    servor.write(83);       //goes straight if it doesnt match anything
    servol.write(104);
    if (error > 3000){
     servol.write(94); // Stopped L, Turning Left
     turns = 0;
    }
    if (error < 3000){
       servor.write(93); // Stopped R, Turning Right
       turns = 1;
    }

    if (digitalRead(YELLOW) == HIGH)
    {
    Serial.println("Reading Yellow");
    Colour(turns, 0);
    }
 
    delay(70);
  }
}

bool Gap()
{
  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  int error = position - 1000;
  digitalWrite(10, HIGH);
  //digitalWrite(A0, HIGH);
  
  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  {
    servol.write(94); // Stopped L
    servor.write(83); // Full Speed R
    delay(2000);
  } else {return false;}

  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  {
    servor.write(93);
    servol.write(104);
    delay(1500);
    position = qtr.readLine(sensors);
    error = position - 1000;
  } else {return true;}

  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  {
        for (unsigned long i=0; i < 1000; i++)
        {
          position = qtr.readLine(sensors);
          error = position - 1000;
          //SensorPrint();
          if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
            {
              servor.write(90);
              servol.write(104);
              delay(2);
            } else {return true;}
        } 
  } else {return true;}

  if (sensors[1] == 0 && sensors[2] == 0 && sensors[3] == 0 && sensors[4] == 0 && sensors[5] == 0 && sensors[6] == 0 && sensors[7] == 0)
  {
    servor.write(93);
    servol.write(104);
    delay(2000);
    
        for (unsigned long i=0; i < 20; i++)
        {
          position = qtr.readLine(sensors);
          error = position - 1000;
          //SensorPrint();
          if (sensors[1] == 0 && sensors[2] == 0 && sensors[3] == 0 && sensors[4] == 0 && sensors[5] == 0 && sensors[6] == 0 && sensors[7] == 0)
            {
              servor.write(83);      
              servol.write(104);
              delay(2);
            } else {return true;}
        }
    } else {return true;}
} 

void Colour(int turn, int drive)
{
  ColourPrint();
  Serial.println("Running Colour Routine...");
  if (turn == 0) // Yellow LEFT
  {
    servol.write(94); // Stopped L
    servor.write(83); // Full Speed R
    delay(2000);
  }

  if (turn == 1) // Yellow RIGHT
  {
    servol.write(104); // Full Speed L
    servor.write(93); // Stopped R 
    delay(2000);
  }

  while(1)
  {
    ColourPrint();
    if (digitalRead(BLUE) == LOW || digitalRead(RED) == LOW)
    {
      servor.write(83);       //goes straight if it doesnt match anything
      servol.write(104);
    } else {
      servol.write(94); // Stopped L
      servor.write(93); // Stopped R
      delay(2000);
      servol.write(104); // Full Speed L
      servor.write(93); // Stopped R
      delay(4000);
      break;
    }
  }

   while(1)
   {
    ColourPrint();
    if (digitalRead(BLACK) == LOW)
    {
      servor.write(83);       //goes straight if it doesnt match anything
      servol.write(104);
    } else if (drive == 0) {
      if (turn == 1)
      {
        servol.write(104); // Full Speed L
        servor.write(93); // Stopped R 
        delay(1000);
      } else {
        servol.write(94); // Stopped L
        servor.write(83); // Full Speed R
        delay(1000);
      }
      loop();
    } else {
      if (turn == 1)
      {
        servol.write(104); // Full Speed L
        servor.write(93); // Stopped R 
        delay(1000);
      } else {
        servol.write(94); // Stopped L
        servor.write(83); // Full Speed R
        delay(1000);
      }
      Drive();
    }
   }
   
    

  
}


void SensorPrint()
{
  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  int error = position - 1000;
  digitalWrite(10, HIGH);
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
  Serial.println("error:");
  Serial.println(error);
  //analogWrite(5, 255);
}

void ColourPrint()
{
    if (digitalRead(YELLOW) == HIGH)
    {
    Serial.println("Reading Yellow");
    }
    if (digitalRead(RED) == HIGH)
    {
    Serial.println("Reading Red");
    }
    if (digitalRead(BLUE) == HIGH)
    {
    Serial.println("Reading Blue");
    }
    if (digitalRead(BLACK) == HIGH)
    {
    Serial.println("Reading Black");
    }
}
   
