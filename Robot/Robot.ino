/* Sweep
 by BARRAGAN <http://barraganstudio.com>
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#define Analog A0

#define NODHEAD 2
#define NODHEAD_RANGE 20
#define NODHEAD_ZERO  112


#define SHAKEHEAD 3
#define SHAKEHEAD_RANGE 20
#define SHAKEHEAD_ZERO  100

#define LEFTARM  4
#define LEFTARM_RANGE 20
#define LEFTARM_ZERO  110

#define RIGHTARM  5
#define RIGHTARM_RANGE 20
#define RIGHTARM_ZERO  70

#define WAIST  6
#define WAIST_RANGE 10
#define WAIST_ZERO  90

#define HEAD  0xAA
#define END   0x55

#define WAIT_HEAD    0x01
#define WAIT_TARGET  0x02
#define WAIT_VALUE   0x03
#define WAIT_END     0x04


#define TEST 
#define TEST_ZERO SHAKEHEAD_ZERO
Servo myservo[5];  // create servo object to control a servo
// twelve servo objects can be created on most boards
uint8_t Robot[2][5] = {
    {LEFTARM_RANGE,RIGHTARM_RANGE,NODHEAD_RANGE,SHAKEHEAD_RANGE,WAIST_RANGE},
    {LEFTARM_ZERO,RIGHTARM_ZERO,NODHEAD_ZERO,SHAKEHEAD_ZERO,WAIST_ZERO},
};
struct move {
    uint8_t leftarm;
    uint8_t rightarm;
    uint8_t nodhead;
    uint8_t shakehead;
    uint8_t waist;
};
typedef struct move smove_t;
typedef smove_t *psmove_t;

union umove {
    uint8_t ucdata[5];
    smove_t srobot;
};
typedef union umove umove_t;
typedef umove_t *pumove_t;
umove_t urobot;
pumove_t purobot = &urobot;


uint8_t Status,target,tempdata;
int pos = 0;    // variable to store the servo position
int val;

boolean update = false;  // whether the string is complete

// The data format is 0x

void setup() {
    Serial.begin(115200);
    myservo[0].attach(LEFTARM);  // attaches the servo on pin 9 to the servo object
    myservo[1].attach(RIGHTARM);  // attaches the servo on pin 9 to the servo object
    myservo[2].attach(NODHEAD);  // attaches the servo on pin 9 to the servo object
    myservo[3].attach(SHAKEHEAD);  // attaches the servo on pin 9 to the servo object
    myservo[4].attach(WAIST);  // attaches the servo on pin 9 to the servo object
    
    myservo[0].write(LEFTARM_ZERO);
    myservo[1].write(RIGHTARM_ZERO);
    myservo[2].write(NODHEAD_ZERO);
    myservo[3].write(SHAKEHEAD_ZERO);
    myservo[4].write(WAIST_ZERO);
    
    
    Status = WAIT_HEAD;
    
    urobot.ucdata[0] = LEFTARM_ZERO;
    urobot.ucdata[1] = RIGHTARM_ZERO;
    urobot.ucdata[2] = NODHEAD_ZERO;
    urobot.ucdata[3] = SHAKEHEAD_ZERO;
    urobot.ucdata[4] = WAIST_ZERO;
}

void loop() {
    
    if (true == update) {
        // myservo[target].write(urobot.ucdata[target]);
        Serial.println("update");
        myservo[0].write(urobot.ucdata[0]);
        myservo[1].write(urobot.ucdata[1]);
        myservo[2].write(urobot.ucdata[2]);
        myservo[3].write(urobot.ucdata[3]);
        myservo[4].write(urobot.ucdata[4]);
        update = false;
    }
}

void serialEvent() {
    while (Serial.available()) {
    // get the new byte:
        uint8_t inChar = (char)Serial.read();
    // add it to the inputString:
        Serial.print(inChar,HEX);
        switch(Status) {
        case WAIT_HEAD:
            if (HEAD == inChar ) {
                Status = WAIT_TARGET;
            } else {
                Status = WAIT_HEAD;
            }
            break;
        case WAIT_TARGET:
            if (inChar <= 5){
                target = inChar;
                Status = WAIT_VALUE;
            } else{
                Status = WAIT_HEAD;
            }
            break;
        case WAIT_VALUE:
            tempdata = inChar;
            Status = WAIT_END;
            break;
        case WAIT_END:
            if (END == inChar) {
                tempdata = map(tempdata,0,100,Robot[1][target]-Robot[0][target],Robot[1][target]+Robot[0][target]);
                urobot.ucdata[target] = tempdata;
                update = true;
            } else {
            }
            Status = WAIT_HEAD;

        }
    }
}