/* Sweep

*/

#define Analog1y A0
#define Analog1x A1

#define Analog2y A2
#define Analog2x A3

#define ButtonPIN1 4
#define ButtonPIN2 7

#define AXIS_X1_MIDDLE 510
#define AXIS_X1_LEFT   265
#define AXIS_X1_RIGHT  755

#define AXIS_Y1_MIDDLE 510
#define AXIS_Y1_LEFT   260
#define AXIS_Y1_RIGHT  760

#define AXIS_X2_MIDDLE 510
#define AXIS_X2_LEFT   260
#define AXIS_X2_RIGHT  760

#define AXIS_Y2_MIDDLE 510
#define AXIS_Y2_LEFT   265
#define AXIS_Y2_RIGHT  755


#define HEAD  0xAA
#define END   0x55


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

uint8_t Robot[5][1] = {
    
};
uint8_t AnalogPort[4]={Analog1y,Analog1x,Analog2y,Analog2x};
uint8_t data[4];

uint8_t SensorNew[4];
uint8_t SensorOld[4];

// The data format is 0x

void setup() {
    Serial.begin(115200);
    data[0] = HEAD;
    data[3] = END;
}

void loop() {
    
    int sensorValue1 = analogRead(A0);
    int sensorValue2 = analogRead(A1);    
    int sensorValue3 = analogRead(A2);
    int sensorValue4 = analogRead(A3);
    if(HIGH == digitalRead(ButtonPIN1)) {
        int sensor = analogRead(AnalogPort[0]);
        data[1] = 4;
        SensorNew[0] = map(sensor,AXIS_X1_LEFT,AXIS_X1_RIGHT,0,100);
        data[2] = SensorNew[0];
        Serial.write(data,4);
    } else{
        for(uint8_t i=0; i<4; i++){
            int sensor = analogRead(AnalogPort[i]);
            if (sensor> AXIS_X1_RIGHT) {
                sensor = AXIS_X1_RIGHT;
            }
            if (sensor < AXIS_X1_LEFT) {
                sensor = AXIS_X1_LEFT;
            }
            SensorNew[i] = map(sensor,AXIS_X1_LEFT,AXIS_X1_RIGHT,0,100);
            data[1] = i;
            data[2] = SensorNew[i];
            
            if(abs(SensorNew[i] - SensorOld[i]) > 0) {
                Serial.write(data,4);
            } 
            SensorOld[i] = SensorNew[i];
        }
    }
    // Serial.print("The X and Y coordinate is:");
    // Serial.print(sensorValue1, DEC);
    // Serial.print(",");
    // Serial.print(sensorValue2, DEC);
    // Serial.print("----");
    // Serial.print(sensorValue3, DEC);
    // Serial.print(",");
    // Serial.println(sensorValue4, DEC);
    // Serial.println(" ");
    delay(20);
}


