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
const int NORMAL[3] = {2300, 1500, 1270000};
const int BLACK[3] = {900, 800, 600};

/* Pin for ESC */
byte servoPin = 3;
Servo servo;

/* pin for IR tracking sensor */
const int leftIRPin = A0;
const int rightIRPin = A1;
int l_IR_val = 0;
int r_IR_val = 0;

/* variables for IR tracking sensor */
int sensorValue = 0;

/* steering wheel */
const int wheelingPin = 6;
Servo wheeling;

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
  wheeling.attach(wheelingPin);

}
  

void loop() 
{  
  int signal = 0;
  color_data();
  track_data();
  DEBUG();
   
  wheeling.write(180);
  if(isBlack(red, green, blue)){
    Serial.println("- (Black Color)");
    signal = 1200; // Set signal value, which should be between 1100 and 1900
    servo.writeMicroseconds(signal); // Send signal to ESC.
    delay(600);  
  } else {
    Serial.println("- (Not Black)");
    int signal = 1100; // Set signal value, which should be between 1100 and 1900
    servo.writeMicroseconds(signal); // Send signal to ESC.  
  }
   
}  

    
void color_data()  
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

void track_data()
{
  l_IR_val = analogRead(leftIRPin);
  r_IR_val = analogRead(rightIRPin);
}

void DEBUG()
{
  Serial.print("R:");  
  Serial.print(red, DEC);  
  Serial.print(" G: ");  
  Serial.print(green, DEC);  
  Serial.print(" B: ");  
  Serial.print(blue, DEC);

  Serial.print(" Left IR : ");
  Serial.print(l_IR_val, DEC);
  Serial.print(" Right IR : ");
  Serial.print(r_IR_val, DEC);
  
}


