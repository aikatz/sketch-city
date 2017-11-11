/*
 *  Reverse a string by using a generic, dynamic stack data structure.
 *
 *  Copyright (C) 2010  Efstathios Chatzikyriakidis (contact@efxa.org)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

// include stack library header.
#include <StackArray.h>
#include <Servo.h>

#define FULL_POWER_CCW   180
#define MID_POWER_CCW    95
#define LOW_POWER_CCW    91

#define FULL_POWER_CW  0
#define MID_POWER_CW   85
#define LOW_POWER_CW   89

#define SERVO_STOP     90

#define LINE_THRESHOLD 800

// Orientation of the robot
typedef enum orientation{
  NORTH,
  SOUTH,
  WEST,
  EAST
} Orientation;

// defining a struct to hold information about each intersection
typedef struct intersect {
  int pos_x; // x-coordinate
  int pos_y; // y-coordinate
  int back_x; // back-coordinate x-coordinate
  int back_y; // back-coordinate y-coordinate
  bool visited; // visited state
} intersect

// Create a matrix with all intersections
intersect[5][4] inters;

// Initializing all the intersections of the maze
for (int i = 0; i < 5; i++) {
  for (int j = 0; j < 4; j++) {
    inters[i][j] = {.pos_x = i; .pos_y = j; .back_x = null; .back_y = null; .visited = false};
  }
}

// create a stack of characters.
StackArray <intersect> stack;

// Declare two Servo objects - one to control each servo
Servo right_servo;
Servo left_servo; 

// Stores the pin # that is connected to the IR sensor
const int right_sensor_pin = 0;
const int left_sensor_pin = 1;
const int center_sensor_pin = 2;
const int left_wall_pin = 3
const int center_wall_pin = 4;
const int right_wall_pin = 5;

// Will use this to store the value from the ADC
int right_sensor_value;
int left_sensor_value;
int center_sensor_value;
int left_wall_sensor;
int center_wall_sensor;
int right_wall_sensor;

typedef enum state{
  STOP,
  STRAIGHT,
  SLIGHT_RIGHT,
  SLIGHT_LEFT,
  INTERSECTION,
  RIGHT,
  LEFT,
  TURN_AROUND
} Direction;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;

// HELPER FUNCTIONS TO HELP SMOOTH THE PROCESS

// Code to update the value of the sensors
void updateSensors() {
  right_sensor_value = analogRead(right_sensor_pin);
  left_sensor_value = analogRead(left_sensor_pin);
  center_sensor_value = analogRead(center_sensor_pin); 
  left_wall_sensor = analogRead(left_wall_pin); 
  center_wall_sensor = analogRead(center_wall_pin); 
  right_wall_sensor = analogRead(right_wall_pin); 
}

// Determining wall presence using surroundings (robot orientation)
bool* wallRobot(){
  bool* walls = (bool*)malloc(sizeof(bool) * 3);

  // The output of this function is described as left-center-right
  // with reference to the robot
  walls[0] = (left_wall_sensor > 400);
  walls[1] = (center_wall_sensor > 400);
  walls[2] = (right_wall_sensor > 400);

  return walls;
}

// Determining wall presence using the actual maze as orientation
bool* wallMaze (Orientation orient, bool* wallsRobot) {
  
  bool* actualWalls = (bool*)malloc(sizeof(bool) * 4);
  
  // Initializing array to all false
  for (int i = 0; i < 4; i++) {
    actualWalls[i] = false;
  }
  
  // The output of this function is described as left-up-right-down 
  // with reference to the maze being outputted to the FPGA
  if (wallsRobot[0] == true) {
   switch(orient) {
      case NORTH: 
        actualWalls[0] = true;
        break;
      case EAST: 
        actualWalls[1] = true;
        break;
      case SOUTH: 
        actualWalls[2] = true;
        break;
      case WEST: 
        actualWalls[3] = true;
        break;
   }
  }
  
  if (wallsRobot[1] == true) {
    switch(orient) {
      case WEST: 
        actualWalls[0] = true;
        break;
      case NORTH: 
        actualWalls[1] = true;
        break;
      case EAST: 
        actualWalls[2] = true;
        break;
      case SOUTH: 
        actualWalls[3] = true;
        break;
     }
  }
  
  if (wallsRobot[2] == true) {
    switch(orient) {
      case SOUTH: 
        actualWalls[0] = true;
        break;
      case WEST: 
        actualWalls[1] = true;
        break;
      case NORTH: 
        actualWalls[2] = true;
        break;
      case SOUTH: 
        actualWalls[3] = true;
        break;
     }
  }

  return actualWalls;
}

// Reading from sensors and 

void setup() {  
  pinMode(LED_BUILTIN, OUTPUT);
  
  // Connect right servo to pin 9, left servo to pin 10
  right_servo.attach(10);
  left_servo.attach(9);
  
  right_servo.write(SERVO_STOP);
  left_servo.write(SERVO_STOP);
  
  Serial.begin(9600);

  // -------------- DEVELOPING DFS HERE --------------

  // Updating initial intersection of the program
  inters[0][0].visited = true;
  
  // Pushing first element into the stack
  stack.push(inters[0][0]);

  // Additional requirement for initial intersection
  bool init = true;

  // Required variables to be updated for the information process
  int current_pos_x, current_pos_y, go_pos_x, go_pos_y, treasure;
  bool visited, wall1, wall2, wall3, wall4; // wall1 -> west; wall2 -> north; wall3 -> east; wall4 -> south

  // Updating value sensors right before starting 
  updateSensors();
  
  // Starting the while loop to interact with the stack
  while(!stack.isEmpty()){

    // Popping the top of the stack
    intersect current_intersect = stack.pop();
    
    if (init) { // if initial intersection is the current intersection

      // Initialize the variables accordingly
      current_pos_x = 0;
      current_pos_y = 0;
      
      // Read from sensors and update variables to be sent in next iteration
      bool* possiblePaths = wallRobot();
      bool* realWalls = wallMaze(current_intersect.orient);
      
      // Update stack for possible intersections to go
      if (possiblePaths[0] == false) {
        stack.push(inters[current_pos_x--][current_pos_y];
      }
      if (possiblePaths[1] == false) {
        stack.push(inters[current_pos_x][current_pos_y++];
      } 
      if (possiblePaths[2] == false) {
        stack.push(inters[current_pos_x++][current_pos_y];
      } 
   
      init = false; // it will never see the first intersection again
    }
    // Mark this element visited if it is not visited already. If it is, continue to the next iteration of DFS
    else if (!current_intersect.visited) {

      // Setting this intersection as visited
      current_intersect.visited = true;

      // Update variables of next destination
      go_pos_x = current_intersect.pos_x;
      go_pos_y = current_intersect.pos_y;

      // Perform back-pointer algorithm if neccesary
      if (abs(current_pos_x - go_pos_x) > 1 || abs(current_pos_y - go_pos_y) > 1) {
        // Back_pointer algorithm
      }
      
      // Send information to the other Arduino
      
      // Perform movement to intersection specified by the popped element from the stack
      
      // Read from sensors and update variables to be sent in next iteration
      bool* possiblePaths = wallRobot();
      bool* realWalls = wallMaze(current_intersect.orient);
      
      // Update stack for possible intersections to go
      if (possiblePaths[0] == false) {
        stack.push(inters[current_pos_x--][current_pos_y];
      }
      if (possiblePaths[1] == false) {
        stack.push(inters[current_pos_x][current_pos_y++];
      } 
      if (possiblePaths[2] == false) {
        stack.push(inters[current_pos_x++][current_pos_y];
      } 
      
      // Update matrix of intersections with the new changes to popped intersection
      inters[current_pos_x][current_pos_y] = current_intersect;
      
      // Update variables for next iteration
      current_pos_x = go_pos_x;
      current_pos_y = go_pos_y;
      
    }
  } 
}

void loop() {

  updateSensors();
  
  // State Outputs
  switch(current_state) {
    case STOP:
      right_servo.write(SERVO_STOP);
      left_servo.write(SERVO_STOP);
      delay(5000);
      break;
      
    case STRAIGHT:
      if(wall_sensor_value > 400)                                                   // Wall ahead, turn around
        next_state = TURN_AROUND;
      else if(right_sensor_value > 850 && left_sensor_value > 850)                  // At intersection, do next turn
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
        next_state = moves[move_idx];
        move_idx += 1;
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
      
      if(currentMillis - previousMillis > 700) next_state = STRAIGHT;
      else next_state = TURN_AROUND;
      break;

    default:
      right_servo.write(SERVO_STOP);
      left_servo.write(SERVO_STOP);
  }
  delay(15);
}


