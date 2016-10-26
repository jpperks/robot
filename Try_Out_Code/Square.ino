#include <Servo.h>

Servo servol;
Servo servor;// create servo object to control a servo


int pos = 0;    // variable to store the servo position

void setup() {
  servor.attach(8);
  servol.attach(9);// attaches the servo on pin 9 to the servo object
}

void loop() {
 servor.write(83);
 servol.write(104);
 delay(2000);
 servol.write(94);
 servor.write(83);
 delay(1260);

}k
