#include <Servo.h>

byte servoPin = 3;
Servo servo;

/*  Callibration Steps : 
    1. Disconnect battery.
    2. Send this program to arduino via USB.
    3. Reset arduino then connect battery.
    4. Wait for proper beep sound then disconnect then reconnect the battery.
    5. Wait for finishing sound, then send your code to it.
*/

void setup() {
  servo.attach(servoPin);
}

void loop() {
  servo.writeMicroseconds(2000);
  delay(15000);
  servo.writeMicroseconds(1200);
  delay(15000); 
}

