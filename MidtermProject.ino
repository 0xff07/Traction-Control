#include <Servo.h>
const int s0 = 8;  
const int s1 = 9;  
const int s2 = 12;  
const int s3 = 11;  
const int out = 10;   
// LED pins connected to Arduino
int redLed = 2;  
int greenLed = 3;  
int blueLed = 4;
// Variables  
int red = 0;  
int green = 0;  
int blue = 0;  
int NORMAL[3] = {28, 23, 15};
int BLACK[3] = {40, 35, 25};

byte servoPin = 6;
Servo servo;

void setup()   
{  
  /* set color sensing */
  Serial.begin(9600); 
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
  pinMode(redLed, OUTPUT);  
  pinMode(greenLed, OUTPUT);  
  pinMode(blueLed, OUTPUT);  
  digitalWrite(s0, HIGH);  
  digitalWrite(s1, HIGH);

  /* set ESC */

  servo.attach(servoPin);
  servo.writeMicroseconds(1200); // send "stop" signal to ESC.
  delay(1000); // delay to allow the ESC to recognize the stopped signal  servo.attach(servoPin);
}
  

void loop() 
{  
  color(); 
  Serial.print("R Intensity:");  
  Serial.print(red, DEC);  
  Serial.print(" G Intensity: ");  
  Serial.print(green, DEC);  
  Serial.print(" B Intensity : ");  
  Serial.print(blue, DEC);  
  //Serial.println();  

  /*
  int signal = 1200; // Set signal value, which should be between 1100 and 1900
  servo.writeMicroseconds(signal); // Send signal to ESC.
  */
  
  if(isBlack(red, green, blue)){
    Serial.println("- (Black Color)");
    int signal = 1200; // Set signal value, which should be between 1100 and 1900
    servo.writeMicroseconds(signal); // Send signal to ESC.    
  } else {
    Serial.println("- (Not Black)");
    int signal = 0; // Set signal value, which should be between 1100 and 1900
    servo.writeMicroseconds(signal); // Send signal to ESC.  
  }
  delay(1000);   
}  

    
void color()  
{    
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  //count OUT, pRed, RED  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s3, HIGH);  
  //count OUT, pBLUE, BLUE  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
  digitalWrite(s2, HIGH);  
  //count OUT, pGreen, GREEN  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}

int isBlack(int R, int G, int B) {
    if(R > BLACK[0] && G > BLACK[1] && B > BLACK[2])
      return 1;
    return 0;
}

int isNormal(int R, int G, int B) {
    
}
