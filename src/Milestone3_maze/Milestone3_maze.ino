#include <Servo.h>

#define FULL_POWER_CCW   180
#define MID_POWER_CCW    95
#define LOW_POWER_CCW    93

#define FULL_POWER_CW  0
#define MID_POWER_CW   85
#define LOW_POWER_CW   87

#define SERVO_STOP     90

#define LINE_THRESHOLD 800

// Declare two Servo objects - one to control each servo
Servo right_servo;
Servo left_servo; 

// Stores the pin # that is connected to the IR sensor
const int right_sensor_pin = 0;
const int left_sensor_pin = 1;
const int center_sensor_pin = 2;
const int leftwall_sensor_pin = 3;
const int forwardwall_sensor_pin = 4;
const int rightwall_sensor_pin = 5;


// Will use this to store the value from the ADC
int right_sensor_value;
int left_sensor_value;
int center_sensor_value;

int lw = 0;
int fw = 0;
int rw = 0;
int walls[3];

enum state{
  STOP,
  STRAIGHT,
  SLIGHT_RIGHT,
  SLIGHT_LEFT,
  INTERSECTION,
  RIGHT,
  LEFT,
  TURN_AROUND
};

volatile state current_state = STRAIGHT;
volatile state next_state = STRAIGHT;
const state moves[] = {STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT, STRAIGHT};     
volatile int n_moves = 9;
volatile int move_idx = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

void setup() {  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Connect right servo to pin 9, left servo to pin 10
  right_servo.attach(10);
  left_servo.attach(9);
  
  right_servo.write(SERVO_STOP);
  left_servo.write(SERVO_STOP);
  
  Serial.begin(9600);
}

void updateSensors() {
  right_sensor_value = analogRead(right_sensor_pin);
  left_sensor_value = analogRead(left_sensor_pin);
  center_sensor_value = analogRead(center_sensor_pin); 
}

void detectWalls() {
  int lw_raw = analogRead(leftwall_sensor_pin);
  int fw_raw = analogRead(forwardwall_sensor_pin);
  int rw_raw = analogRead(rightwall_sensor_pin);
  if (lw_raw > 400 && lw_raw < 600){
    lw = 1;
  } else{
    lw = 0;
  }
   if (fw_raw > 500 && fw_raw < 600){
    fw = 1;
  } else{
    fw = 0;
  }
   if (rw_raw > 400 && rw_raw <600){
    rw = 1;
  } else{
    rw = 0;
  }
  walls[0] = lw;
  walls[1] = fw;
  walls[2] = rw;
}

void loop() {
  if(move_idx > n_moves - 1) move_idx = 0;
  current_state = next_state;

  updateSensors();
  
  // State Outputs
  switch(current_state) {
    case STOP:
      right_servo.write(SERVO_STOP);
      left_servo.write(SERVO_STOP);
      delay(5000);
      break;
      
    case STRAIGHT:
      if(right_sensor_value > LINE_THRESHOLD && left_sensor_value > LINE_THRESHOLD)                  // At intersection, do next turn
        next_state = INTERSECTION;
      else if(right_sensor_value > LINE_THRESHOLD)  next_state = SLIGHT_RIGHT;      // Drifting left, correct right
      else if(left_sensor_value > LINE_THRESHOLD)   next_state = SLIGHT_LEFT;       // Drifting right, correct left
      else {                                                                        // Go striaght
        right_servo.write(MID_POWER_CW);
        left_servo.write(MID_POWER_CCW);
        next_state = STRAIGHT;
      }
      break;
      
    case SLIGHT_RIGHT:                                                              // Drifting left, correct right
      right_servo.write(LOW_POWER_CW);
      left_servo.write(MID_POWER_CCW);
      
      if(right_sensor_value > LINE_THRESHOLD && left_sensor_value > LINE_THRESHOLD) // At intersection, do next turn
        next_state = INTERSECTION;
      else if(right_sensor_value > LINE_THRESHOLD) next_state = SLIGHT_RIGHT;
      else next_state = STRAIGHT;
      break;
      
    case SLIGHT_LEFT:                                                               // Drifting right, correct left
      right_servo.write(MID_POWER_CW);
      left_servo.write(LOW_POWER_CCW);
      
      if(right_sensor_value > LINE_THRESHOLD && left_sensor_value > LINE_THRESHOLD) // At intersection, do next turn
        next_state = INTERSECTION;
      else if(left_sensor_value > LINE_THRESHOLD) next_state = SLIGHT_LEFT;
      else next_state = STRAIGHT;
      break;
      
    case INTERSECTION:
      if(center_sensor_value > LINE_THRESHOLD) {
        detectWalls();
        if (walls[1] == 0){
          next_state = STRAIGHT; 
        } else{
           if(walls[0] == 1 && walls[2] == 1){
              next_state = TURN_AROUND; 
           }
           else if(walls[0] == 1){
              next_state = RIGHT;
           }
           else if(walls[2] == 1){
              next_state = LEFT;
           }
        }
        previousMillis = 0;
      }
      else next_state = INTERSECTION;
      break;
      
    case RIGHT:
      right_servo.write(FULL_POWER_CCW);
      left_servo.write(FULL_POWER_CCW);

      if(previousMillis == 0) {
         previousMillis = millis();
      }
      
      currentMillis = millis();
      
      if(currentMillis - previousMillis > 380) next_state = STRAIGHT;
      else next_state = RIGHT;
      break;
      
    case LEFT:
      right_servo.write(FULL_POWER_CW);
      left_servo.write(FULL_POWER_CW);

      if(previousMillis == 0) {
         previousMillis = millis();
      }
      
      currentMillis = millis();
      
      if(currentMillis - previousMillis > 380) next_state = STRAIGHT;
      else next_state = LEFT;
      break;

    case TURN_AROUND:
      right_servo.write(FULL_POWER_CW);
      left_servo.write(FULL_POWER_CW);
      
      if(previousMillis == 0) {
         previousMillis = millis();
      }
      
      currentMillis = millis();
      
      if(currentMillis - previousMillis > 1000) next_state = STRAIGHT;
      else next_state = TURN_AROUND;
      break;

    default:
      right_servo.write(SERVO_STOP);
      left_servo.write(SERVO_STOP);
  }
  delay(15);
}


