# Milestone 1

### Line Following
Our first goal is to make the robot follow a black line on a white background. We implemented an IR sensor at the front of the robot which detects the existence of he black tape. We want to keep the robot on the right edge between the black tape and white ground. When the IR sensor is above the white background, we turn the robot to the left by setting the right servo to full power clockwise and the left servo to low power counter-clockwise. When the IR sensor is above the black tape, we turn the robot to the right by setting the left servo to full power clockwise and the right servo to low power counter-clockwise. This not only allowed us to follow a straight line, but lines with relatively large curvatures as well.
 
 <div style="text-align: center">
 <iframe width="534" height="300" src="https://www.youtube.com/embed/kUykOyhvOOE" frameborder="0" allowfullscreen></iframe>
 </div>


### Grid Traversal (figure eight)
In order to get our robot to transverse grids, we implemented two more sensor. Two sensor are aligned on the front of the robot, and one sensor is implemented behind the front right sensor. The two sensor on the front allow straight line following and intersection detection. The sensor behind the right sensor make sure the robot returns to straight line following after turning at an intersection. We also had to do a few major modifications of our code for this section. Instead of using while and for loops, we created a finite state machine and used switch statements instead.

 <div style="text-align: center">
 <iframe width="534" height="300" src="https://www.youtube.com/embed/2xJRc-dZXlw" frameborder="0" allowfullscreen></iframe>
 </div>

Here's our full code for the Grid Traversal:

```C
#include <Servo.h>

#define FULL_POWER_CCW   180
#define MID_POWER_CCW    120
#define LOW_POWER_CCW    91

#define FULL_POWER_CW  0
#define MID_POWER_CW   60
#define LOW_POWER_CW   89

#define SERVO_STOP     90

// Declare two Servo objects - one to control each servo
Servo right_servo;
Servo left_servo; 

// Stores the pin # that is connected to the IR sensor
const int right_sensor_pin = 0;
const int left_sensor_pin = 1;
const int center_sensor_pin = 2;

// Will use this to store the value from the ADC
int right_sensor_value;
int left_sensor_value;
int center_sensor_value;

enum state{
  STOP,
  STRAIGHT,
  SLIGHT_RIGHT,
  SLIGHT_LEFT,
  INTERSECTION,
  RIGHT,
  LEFT
};

volatile state current_state = STRAIGHT;
volatile state next_state = STRAIGHT;
const state moves[] = {RIGHT, LEFT, LEFT, LEFT, RIGHT, RIGHT, RIGHT, RIGHT, STOP};     
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
      if(right_sensor_value > 850 && left_sensor_value > 850)           // At intersection, do next turn
        next_state = INTERSECTION;
      else if(right_sensor_value > 850)  next_state = SLIGHT_RIGHT;    // Drifting left, correct right
      else if(left_sensor_value > 850)   next_state = SLIGHT_LEFT;     // Drifting right, correct left
      else {                                                           // Go striaght
        right_servo.write(FULL_POWER_CW);
        left_servo.write(FULL_POWER_CCW);
        next_state = STRAIGHT;
      }
      break;
    case SLIGHT_RIGHT:                                                  // Drifting left, correct right
      //Serial.println("SLIGHT RIGHT");
      right_servo.write(LOW_POWER_CW);
      left_servo.write(MID_POWER_CCW);
      
      if(right_sensor_value > 850 && left_sensor_value > 850)           // At intersection, do next turn
        next_state = INTERSECTION;
      else if(right_sensor_value > 850) next_state = SLIGHT_RIGHT;
      else next_state = STRAIGHT;
      break;
    case SLIGHT_LEFT:                                                   // Drifting right, correct left
      right_servo.write(FULL_POWER_CW);
      left_servo.write(MID_POWER_CCW);
      
      if(right_sensor_value > 850 && left_sensor_value > 850)           // At intersection, do next turn
        next_state = INTERSECTION;
      else if(left_sensor_value > 850) next_state = SLIGHT_LEFT;
      else next_state = STRAIGHT;
      break;
    case INTERSECTION:
      if(center_sensor_value > 850) {
        next_state = moves[move_idx];
        move_idx += 1;
        previousMillis = 0;
      }
      else next_state = INTERSECTION;
      break;
    case RIGHT:                                                         // Do a 90 degree right turn
      right_servo.write(FULL_POWER_CCW);
      left_servo.write(FULL_POWER_CCW);

      if(previousMillis == 0) {
         previousMillis = millis();
      }
      
      currentMillis = millis();
      
      if(currentMillis - previousMillis > 420) next_state = STRAIGHT;
      else next_state = RIGHT;
      break;
    case LEFT:                                                          // Do a 90 degree left turn
      right_servo.write(FULL_POWER_CW);
      left_servo.write(FULL_POWER_CW);

      if(previousMillis == 0) {
         previousMillis = millis();
      }
      
      currentMillis = millis();
      
      if(currentMillis - previousMillis > 420) next_state = STRAIGHT;
      else next_state = LEFT;
      break;
    default:
      right_servo.write(SERVO_STOP);
      left_servo.write(SERVO_STOP);
  }
  delay(15);
}



```

### Advantages of using an FSM
One advantage of using a FSM is that we can replace most the while and for loops which can cause infinite loops that are hard to debug. The delay function also causes many problems, but we can get around the delay function with FSM using the Arduino timer and millis(). In the future, we are going to have much more complex code and FSM can be very helpful in keeping our code organized and concise. We would include a FSM diagram, but due to time constraints, we have not had time to draw one out. We will upload this soon.

### Possible Improvements
One thing we tried when we were implementing grid transversal was to change our power supply from a 9V battery to a power bank. This was because the robot sometimes freeze after crossing an intersection for a few seconds and then resume to normal. We suspect that this problem was caused by the power supply since no where in the code do we allow the robot to stop completely. We believe that the servos are causing inductive spikes which are overloading the Arduino and causing it to reset. As a result, the robot could never complete the figure eight. However, the new power supply give us many other new problems and due to time limit, we decide to change our power back to the Arduino power supply. But we probably will try to implement the power bank power supply in the future to improve robustness.
