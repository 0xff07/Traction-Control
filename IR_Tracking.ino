#include <Servo.h>

/* pin for TCS3200 */
const int IR_pin[5] = {A0, A1, A2, A3, A4}; 

/* variables for IR tracking sensor */
int IR_val[5] = {0};
int IR_state = 0;

/* Pin for ESC */
byte servoPin = 3;
Servo servo;

/* steering wheel */
const int wheelingPin = 6;
Servo wheeling;
#define SERVO_OFF 40
#define SERVO_ZERO 105
#define SERVO_RIGHT_MAX (SERVO_ZERO + SERVO_OFF)
#define SERVO_LEFT_MAX (SERVO_ZERO - SERVO_OFF)
int ANGLES[2][5] = {{0}};

/* user defined settings */
#define timestep 25
#define NOP_TIME 100
#define NORMAL_POWER 1400
const int ANGLE_OFFSET[2][5] = {{30, 10, 0, -10, -30}, 
                                {20, 5, -5, -20, 0}};
const int POW_OFFSET[2][5] = {{50, 25, 0, 25, 50}, 
                              {45, 20, 20, 45, 0}};
const int TURN_DELAY[2][5] = {  {25, 25, 25, 25, 25},  
                                {25, 25, 25, 25, 0}};

void setup()
{  
    Serial.begin(9600); 

     /* set ESC */
    servo.attach(servoPin);
    servo.writeMicroseconds(1100);
    wheeling.attach(wheelingPin);
    wheeling.write(SERVO_ZERO);

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < 5; j++)
            ANGLES[i][j] = NOTMAL_ANGLE + ANGLE_OFFSET[i][j];
}
  

void loop() 
{    
    track_data();
    IR_state = get_IR_state();
    DEBUG();
    delay(NOP_TIME);

    switch(IR_state){
        case 16 :
            turn(ANGLES[0][0], NORMAL_POWER + POW_OFFSET[0][0],TURN_DELAY[0][0]);
            break;
        case 8 :
            turn(ANGLES[0][1], NORMAL_POWER + POW_OFFSET[0][1], TURN_DELAY[0][1]);
            break;
        case 4 :
            turn(ANGLES[0][2], NORMAL_POWER + POW_OFFSET[0][2], TURN_DELAY[0][2]);
            break;
        case 2 :
            turn(ANGLES[0][3], NORMAL_POWER + POW_OFFSET[0][3], TURN_DELAY[0][3]);
            break;
        case 1 :
            turn(ANGLES[0][4], NORMAL_POWER + POW_OFFSET[0][4], TURN_DELAY[0][4]);
            break;

        case (16 + 8) :
            turn(ANGLES[1][0], NORMAL_POWER + POW_OFFSET[1][0],TURN_DELAY[1][0]);
            break;
        case (8 + 4) :
            turn(ANGLES[1][1], NORMAL_POWER + POW_OFFSET[1][1], TURN_DELAY[1][1]);
            break;
        case (4 + 2) :
            turn(ANGLES[1][2], NORMAL_POWER + POW_OFFSET[1][2], TURN_DELAY[1][2]);
            break;
        case (2 + 1) :
            turn(ANGLES[1][3], NORMAL_POWER + POW_OFFSET[1][3], TURN_DELAY[1][3]);
            break;
 
        default :
            wheeling.write(SERVO_ZERO);
            delay(timestep);
  }
}  


void track_data()
{
  for(int i = 0; i < 5; i++) {
    IR_val[i] = analogRead(IR_pin[i]);
  }
  IR_state = get_IR_state();
}

unsigned int get_IR_state()
{
    unsigned int ret = 0;
    ret += IR_val[4]>500?1:0;
    ret += IR_val[3]>500?2:0; 
    ret += IR_val[2]>500?4:0;
    ret += IR_val[1]>500?8:0;
    ret += IR_val[0]>500?16:0;  
    return ret;
}

void DEBUG()
{
    for(int i = 0; i < 5; i++) {
        Serial.print(i, DEC);
        Serial.print(" ");
        Serial.print(IR_val[i], DEC);
        Serial.print(" ");
    }
    Serial.print(" IR_state : ");
    Serial.println(IR_state, DEC);
}

void turn(int angle, int power, int time_width) {
      wheeling.write(angle);
      servo.writeMicroseconds(power);
      delay(time_width);  
}
