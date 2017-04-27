#include <Servo.h>

/* pin for TCS3200 */
const int s0 = 8;  
const int s1 = 9;  
const int s2 = 12;  
const int s3 = 11;  
const int out = 10;   

/* RGB values */
int red = 0;  
int green = 0;  
int blue = 0;  

/* Ambient and target(Black) color value */
const int NORMAL[3] = {28, 23, 15};
const int BLACK[3] = {40, 35, 25};

/* Pin for ESC */
byte servoPin = 6;
Servo servo;

/* pin for IR tracking sensor */
int sensorPin = A0;

/* variables for IR tracking sensor */
int sensorValue = 0;

void setup()   
{  
  /* set color sensing */
  Serial.begin(9600); 
  pinMode(s0, OUTPUT);  
  pinMode(s1, OUTPUT);  
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
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
  Serial.print("R:");  
  Serial.print(red, DEC);  
  Serial.print(" G: ");  
  Serial.print(green, DEC);  
  Serial.print(" B: ");  
  Serial.print(blue, DEC);  
  //Serial.println();  

  /*
  int signal = 1200; // Set signal value, which should be between 1100 and 1900
  servo.writeMicroseconds(signal); // Send signal to ESC.
  */
  sensorValue = analogRead (sensorPin);
  Serial.println (sensorValue, DEC); //debug
  
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
  digitalWrite(s0, LOW);  
  digitalWrite(s1, HIGH);
  
  /* set TSC3200 to read RED */
  /* (s2, s3) = (LOW, LOW)   */   
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);  
  /* count OUT, pRed, RED    */  
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  /* set TCS3200 to read BLUE */
  /* (s2, s3) = (LOW, HIGH)   */
  digitalWrite(s3, HIGH);  
  /* count OUT, pBLUE, BLUE   */  
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH); 

  /* set TCS3200 to read GREEN */
  /* (s2, s3) = (HIGH, HIGH)   */
  digitalWrite(s2, HIGH);  
  /* count OUT, pGreen, GREEN  */  
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);  
}

int isBlack(int R, int G, int B) {
    if(R > BLACK[0] && G > BLACK[1] && B > BLACK[2])
      return 1;
    return 0;
}


