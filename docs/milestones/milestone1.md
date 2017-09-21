# Milestone 1

### Line Following
Our first goal is to make the robot follow a black line on a white background. We implemented an IR sensor at the front of the robot which detects the existence of he black tape. We want to keep the robot on the right edge between the black tape and white ground. When the IR sensor is above the white background, we turn the robot to the left by setting the right servo to full power clockwise and the left servo to low power counter-clockwise. When the IR sensor is above the black tape, we turn the robot to the right by setting the left servo to full power clockwise and the right servo to low power counter-clockwise. This not only allowed us to follow a straight line, but lines with relatively large curvatures as well.
 
 <div style="text-align: center">
 <iframe width="534" height="300" src="https://www.youtube.com/embed/IGdBMjl0flY" frameborder="0" allowfullscreen></iframe>
 </div>


### Grid Traversal (figure eight)
In order to get our robot to transverse grids, we implemented two more sensor. Two sensor are aligned on the front of the robot, and one sensor is implemented behind the front right sensor. The two sensor on the front allow straight line following and intersection detection. The sensor behind the right sensor make sure the robot returns to straight line following after turning at an intersection. We also had to do a few major modifications of our code for this section. Instead of using while and for loops, we created a finite state machine and used switch statements instead.

 <div style="text-align: center">
 <iframe width="534" height="300" src="https://www.youtube.com/embed/2xJRc-dZXlw" frameborder="0" allowfullscreen></iframe>
 </div>

Here's our full code for the Grid Traversal:

```/* 
 *  GridTraverse.ino 
 *  
 *  Uses a very basic algorithm to have the robot follow 
 *  a black line on a white background
 */

#include <Servo.h>

#define FULL_POWER_CCW   180
#define MID_POWER_CCW    135
#define LOW_POWER_CCW    91

#define FULL_POWER_CW  0
#define MID_POWER_CW   45
#define LOW_POWER_CW   89

// Declare two Servo objects - one to control each servo
Servo right_servo;
Servo left_servo; 

// Stores the pin # that is connected to the IR sensor
const int right_sensor_pin = 0;
const int left_sensor_pin = 1;

// Will use this to store the value from the ADC
int right_sensor_value;
int left_sensor_value;

void setup() {
  // Connect right servo to pin 9, left servo to pin 10
  right_servo.attach(10);
  left_servo.attach(9);
}  

void updateSensors() {
  right_sensor_value = analogRead(right_sensor_pin);
  left_sensor_value = analogRead(left_sensor_pin);
}

void loop() {
  // Read the value from the ADC connected to the IR sensor
  updateSensors();

  if(right_sensor_value > 850 && left_sensor_value > 850) {
      delay(50);

      // Full left turn
      right_servo.write(FULL_POWER_CW);
      left_servo.write(FULL_POWER_CW);
      
      // Full right turn
      //right_servo.write(FULL_POWER_CCW);
      //left_servo.write(FULL_POWER_CCW);
      
      delay(380);
      
  } else if(right_sensor_value > 850) {
      // Small correction right
      right_servo.write(LOW_POWER_CW);
      left_servo.write(FULL_POWER_CCW);
    
  } else if(left_sensor_value > 850) {
      // Small correct left
      right_servo.write(FULL_POWER_CW);
      left_servo.write(LOW_POWER_CCW);
      
  } else {
      right_servo.write(MID_POWER_CW);
      left_servo.write(MID_POWER_CCW);
  }

  // wait 20ms
  delay(10);
}

```

### Advantages of using an FSM
One advantage of using a FSM is that we can replace most the while and for loops which can cause infinite loops that are hard to debug. The delay function also causes many problems, but we can get around the delay function with FSM using the Arduino timer and millis(). In the future, we are going to have much more complex code and FSM can be very helpful in keeping our code organized and concise. We would include a FSM diagram, but due to time constraints, we have not had time to draw one out. We will upload this soon.

### Possible Improvements
One thing we tried when we were implementing grid transversal was to change our power supply from a 9V battery to a power bank. This was because the robot sometimes freeze after crossing an intersection for a few seconds and then resume to normal. We suspect that this problem was caused by the power supply since no where in the code do we allow the robot to stop completely. We believe that the servos are causing inductive spikes which are overloading the Arduino and causing it to reset. As a result, the robot could never complete the figure eight. However, the new power supply give us many other new problems and due to time limit, we decide to change our power back to the Arduino power supply. But we probably will try to implement the power bank power supply in the future to improve robustness.
