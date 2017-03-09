// ******************** LINE FOLLOWER ROBOT *********************
// The robot must complete the following tasks:
//
// - Follow a black line.
// - Jump a gap in the black line.
// - If there is a yellow line junction on the black line, the
//   robot must go down it, light up an LED to show the colour
//   at the end of the yellow line. Then turn around and continue
//   the journey on the black line.

#include <QTRSensors.h>
#include <Wire.h>
#include <Servo.h>

// SERVO MOTOR SPEEDS
//
// 94 ==  LEFT Stopped Counterclockwise.
// 93 ==  RIGHT Stopped Clockwise.

// 104 ==  LEFT Full Speed Clockwise.
// 83 ==  RIGHT Full Speed Counterclockwise.

// IR REFLECTANCE SENSOR OUTPUT
//
// 1000 == Sensor[n] is over the black line.
// >0, <1000 == Sensor[n] is over the white surface.
// 0 == Sensor[n] doesn't recognise anything. (Calibration issue). 

// Create servo objects.
Servo servol;
Servo servor;

// Define the pins used by the IR reflectance sensor.
QTRSensorsRC qtr((char[]) {2, 3, 4, 5, 6, 7, 8, 9}, 8); // {Digital Pin Numbers}

// Analogue pins used as digital to read incoming colours from the Nano.
#define BLUE A0
#define RED A1
#define BLACK A2
#define YELLOW A3

void setup() {
  Serial.begin(9600);

  // IR LEDs pin.
  pinMode(10, OUTPUT);
  // Builtin LED
  pinMode(13, OUTPUT);

  pinMode(A4, OUTPUT);
  pinMode(A5, OUTPUT);
  
  // Setup incoming colour pins.
  pinMode(BLUE, INPUT);
  pinMode(RED, INPUT);
  pinMode(YELLOW, INPUT);
  pinMode(BLACK, INPUT);

  // Turn on IR LEDs.
  digitalWrite(10, HIGH);

  // IR Sensor Calibration
  //
  // Move the sensor left and right over the line so that
  // all IR sensors know the difference between the surface
  // and the white line. This only needs to done once.
  //
  int i;
  for (i = 0; i < 400; i++)
  {
  qtr.calibrate();
  qtr.emittersOn();
  //digitalWrite(A5, HIGH);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(20);
  digitalWrite(LED_BUILTIN, LOW);
  //digitalWrite(A5, LOW);
  }

  // Attaches the servo objects to pins.
  servor.attach(11);
  servol.attach(12);


}

void loop() {

  // Define turn variable.
  // Left = 0;
  // Right = 1;
  int turns;
  
  digitalWrite(LED_BUILTIN, LOW);

  // Read from the IR sensor. Then determine the error (how far away from the line)
  // the robot is.
  digitalWrite(10, HIGH);   
  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  int error = position - 1000;

  // Print sensor and/or Colour values to the serial for debugging.
  //SensorPrint();
  ColourPrint();
  

  // Blink the builtin LED. Shows something is running!
  if (sensors[0] >= 0)
  {digitalWrite(LED_BUILTIN, HIGH); delay(50); digitalWrite(LED_BUILTIN, LOW);}


  // Let the robot travel straight intialally or is perfectly over the line (unlikely). 
  servor.write(86);
  servol.write(100);

  // Turn the robot LEFT if the Error is greater than 3000.
  if (error > 3000){
       servol.write(94); // Stopped L, Turning Left
       turns = 0;
  }
  
  // Turn the robot RIGHT if the Error is greater than 3000.
  if (error<3000){
       servor.write(93); // Stopped R, Turning Right
       turns = 1;
  }

  delay(50);
  // Check if the incoming colour from the Nano is Yellow.
  // If this is true the Colour(turn_state, jumped_gap) function
  // is run.
  if (digitalRead(YELLOW) == HIGH)
  {
    delay(100);
    if (digitalRead(YELLOW) == HIGH)
    {
      Serial.println("Reading Yellow");
      Colour(turns, 0);
    }
  }


// INITIAL GAP CHECKING
//
// If the robot stops seeing the line, it will travel forward for a small amount of time. If the line is still not to be found after this time,
// the Gap() function is run. The delay stops the robot from instantly going into the gap function due to bad sections on the black line.
//
// After the gap function has finished and has found the black line (returns true), the robot continues it's journey using the Drive() function.
//
if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
{
  delay(200);
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

// An endless loop of following the black line and checking for a yellow line. This function
// is only run after the gap function has returned true.
//
void Drive()
{
   // 94 ==  LEFT Stopped Counterclockwise.
      // 93 ==  RIGHT Stopped Clockwise.

      // 104 ==  LEFT Full Speed Clockwise.
      // 83 ==  RIGHT Full Speed Counterclockwise.
  while(1)
  {
    ColourPrint();
    int turns;
    unsigned int sensors[8];
    int position = qtr.readLine(sensors);
    int error = position - 1000;
    digitalWrite(10, HIGH);
    servor.write(86);       //goes straight if it doesnt match anything
    servol.write(100);
    if (error > 3000){
     servol.write(94); // Stopped L, Turning Left
     turns = 0;
    }
    if (error < 3000){
       servor.write(93); // Stopped R, Turning Right
       turns = 1;
    }
    delay(90);
    if (digitalRead(YELLOW) == HIGH)
    {
    Serial.println("Reading Yellow");
    Colour(turns, 1);
    }
 
    delay(100);
  }
}

bool Gap()
{
  // Read the IR values again.
  unsigned int sensors[8];
  int position = qtr.readLine(sensors);
  int error = position - 1000;
  digitalWrite(10, HIGH);

  // Check if over the line and turn LEFT.
  //
  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  {
    servol.write(94); // Stopped L
    servor.write(83); // Full Speed R
    delay(2000);
  } else {return false;}

  // Check if over the line and turn RIGHT.
  //
  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  {
    servor.write(93);
    servol.write(104);
    delay(1500);
    position = qtr.readLine(sensors);
    error = position - 1000;
  } else {return true;}

  // Check if over the line, while turning ~180 degrees RIGHT gradually.
  //
  if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
  {
        for (unsigned long i=0; i < 1000; i++)
        {
          position = qtr.readLine(sensors);
          error = position - 1000;
          
          if (sensors[1] != 1000 && sensors[2] != 1000 && sensors[3] != 1000 && sensors[4] != 1000 && sensors[5] != 1000 && sensors[6] != 1000 && sensors[7] != 1000)
            {
              servor.write(90);
              servol.write(104);
              delay(2);
            } else {return true;}
        } 
  } else {return true;}

  // Check if over the line and turn LEFT. The robot should have already have found the line, so probably won't get to here.
  //
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


// FOUND YELLOW LINE
//
void Colour(int turn, int gap)
{
  digitalWrite(A4, LOW);
  digitalWrite(A5, LOW);
  ColourPrint();
  Serial.println("Running Colour Routine...");
  servor.write(83);      
  servol.write(104);
  delay(100);
  if (turn == 1) // Yellow LEFT
  {
    servol.write(94); // Stopped L
    servor.write(83); // Full Speed R
    delay(1500);
  }

  if (turn == 0) // Yellow RIGHT
  {
    servol.write(104); // Full Speed L
    servor.write(93); // Stopped R 
    delay(1200);
  }

  digitalWrite(A5, LOW);
  
  while(1)
  {
    ColourPrint();
    if (digitalRead(BLUE) == LOW && digitalRead(RED) == LOW)
    {
     
      servol.attach(12);
      servor.attach(11);
     servor.write(83);       //goes straight if it doesnt match anything
     servol.write(104);
      delay(200);
      servol.detach();
      servor.detach();
      delay(2000);
    } else {
      servol.detach();
      servor.detach();
      if (digitalRead(BLUE) == HIGH)
      {
        delay(500);
        if (digitalRead(BLUE) == HIGH)
        {
        digitalWrite(A5, HIGH);
        } else {digitalWrite(A4, HIGH);}
        delay(1700);
      }

      if (digitalRead(RED) == HIGH)
      {
        delay(500);
        if (digitalRead(RED) == HIGH)
        {
        digitalWrite(A4, HIGH);
        } else {digitalWrite(A5, HIGH);}
        delay(1700);
      }
      
      digitalWrite(A4, LOW);
      digitalWrite(A5, LOW);
      servol.attach(12);
      servor.attach(11);
      break;
    }
  }

  if (turn == 1) // YELLOW LEFT
       {
          servol.write(104); // Full Speed L
          servor.write(93); // Stopped R 
          delay(3700);
          
        } else { // YELLOW RIGHT
          servol.write(94); // Stopped L
          servor.write(83); // Full Speed R
          delay(3700);
        }
  
   while(1)
   {
    ColourPrint();
    unsigned int sensors[8];
    int position = qtr.readLine(sensors);
    if (sensors[0] == 1000 || sensors[1] == 1000 || sensors[2] == 1000 || sensors[3] == 1000 || sensors[4] == 1000 || sensors[5] == 1000 || sensors[6] == 1000 || sensors[7] == 1000)
    {
      if (gap == 0)
      {
        loop();
      }  else {
        Drive();  
    }
    } else {
        servor.write(83);       //goes straight if it doesnt match anything
        servol.write(104);
        
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
//  servol.detach();
//      servor.detach();
//  while(1){
    digitalWrite(A4, LOW);
    digitalWrite(A5, LOW);
    
    if (digitalRead(YELLOW) == HIGH)
    {
    Serial.println("Reading Yellow");
    }
    if (digitalRead(RED) == HIGH)
    {
    Serial.println("Reading Red");
    //digitalWrite(A4, HIGH);
    delay(10);
    }
    if (digitalRead(BLUE) == HIGH)
    {
    Serial.println("Reading Blue");
    //digitalWrite(A5, HIGH);
    delay(10);
    }
    if (digitalRead(BLACK) == HIGH)
    {
    Serial.println("Reading Black");
    }
  
  //}
}
   
