#include <Servo.h>

/* pin for IR sensors */
const int IR_pin[5] = {A0, A1, A2, A3, A4}; 

/* variables for IR tracking sensor */
#define IR_NUM 5
int IR_val[IR_NUM] = {0};
unsigned int IR_state = 0;

/* Pin for ESC */
byte servoPin = 3;
Servo servo;

/* steering wheel */
const int wheelingPin = 6;
Servo wheeling;
#define SERVO_OFF 40
#define SERVO_ZERO 110
#define SERVO_RIGHT_MAX (SERVO_ZERO + SERVO_OFF)
#define SERVO_LEFT_MAX (SERVO_ZERO - SERVO_OFF)
int ANGLES[2][IR_NUM] = {{0}};

/* user defined settings */
#define NOP_TIME 100
#define NORMAL_POWER 1340
const int ANGLE_OFFSET[2][IR_NUM] = {{13, 5, 0, -5, -15}, 
                                {8, 2, -2, -8, 0}};
const int POW_OFFSET[2][IR_NUM] = {{100, 10, -20, 10, 150}, //25
                              {0, 0, 3, 3, 0}};
const int TURN_DELAY[2][IR_NUM] = {{600, 20, 20, 200, 800},  
                              {20, 20, 20, 20, 0}};
#define IR_THREASHOLD 500
#define SAMPLE_INTERVAL 5
#define SAMPLE_TIMES 1

void setup()
{  
    Serial.begin(9600); 

     /* set ESC */
    servo.attach(servoPin);
    wheeling.attach(wheelingPin);
    wheeling.write(SERVO_ZERO);

    for(int i = 0; i < 2; i++)
        for(int j = 0; j < IR_NUM; j++)
            ANGLES[i][j] = SERVO_ZERO + ANGLE_OFFSET[i][j];
}
  

void loop() 
{    
    sample_data();   
    IR_state = get_IR_state();
    DEBUG(); 

    switch(IR_state){
        /* Actions to do if there's only 1 IR senses black line */
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
        //case 17 :
        case 1 :
            turn(ANGLES[0][4], NORMAL_POWER + POW_OFFSET[0][4], TURN_DELAY[0][4]);
            break;

        /* Actions to do if there are 2 neighboring IR senses black line */
        case (16 + 8) :
            turn(ANGLES[1][0], NORMAL_POWER + POW_OFFSET[1][0],TURN_DELAY[1][0]);
            break;
        case (8 + 4) :
            turn(ANGLES[1][1], NORMAL_POWER + POW_OFFSET[1][1], TURN_DELAY[1][1]);
            break;
        case (4 + 2) :
            turn(ANGLES[1][2], NORMAL_POWER + POW_OFFSET[1][2], TURN_DELAY[1][2]);
            break;
        //case 18:
        case (2 + 1) :
            turn(ANGLES[1][3], NORMAL_POWER + POW_OFFSET[1][3], TURN_DELAY[1][3]);
            break;

        /* Keep original direction for other cases (which are "noisy cases") */
        default :
            servo.writeMicroseconds(NORMAL_POWER);
            delay(NOP_TIME);
  }
}  


void sample_data()
{
    /* initialize IR value */
    for(int i = 0; i < IR_NUM; i++)
        IR_val[i] = 0;

    /* sampple SAMPLE_TIMES, with time interval SAMPLE_INTERVAL */
    for(int i = 0; i < SAMPLE_TIMES; i++) {
        delay(SAMPLE_INTERVAL);
        for(int j = 0; j < 5; j++)
            IR_val[j] += analogRead(IR_pin[j]);
    }

    /* get average values */
    for(int i = 0; i < IR_NUM; i++)
        IR_val[i] /= SAMPLE_TIMES;

    IR_state = get_IR_state();
}

unsigned int get_IR_state()
{
    unsigned int ret = 0;
    for(int i = 0; i < IR_NUM; i++)
        ret += IR_val[IR_NUM - i - 1] > IR_THREASHOLD?(1<<i):0;
    return ret;
    /*
    ret += IR_val[4]>IR_THREASHOLD?1:0;
    ret += IR_val[3]>IR_THREASHOLD?2:0; 
    ret += IR_val[2]>IR_THREASHOLD?4:0;
    ret += IR_val[1]>IR_THREASHOLD?8:0;
    ret += IR_val[0]>IR_THREASHOLD?16:0;  
    return ret;
    */
}

void DEBUG()
{
    for(int i = 0; i < IR_NUM; i++) {
        Serial.print(i, DEC);
        Serial.print(" : ");
        Serial.print(IR_val[i], DEC);
        Serial.print(" || ");
    }
    Serial.print(" IR_state : ");
    Serial.println(IR_state, DEC);
}

void turn(int angle, int power, int time_width) {
      wheeling.write(angle);
      servo.writeMicroseconds(power);
      delay(time_width);  
}
