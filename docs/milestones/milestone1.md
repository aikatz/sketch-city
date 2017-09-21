# Milestone 1

### Line Following
Our first goal is to make the robot follow a black line on a white background. We implemented an IR sensor at the front of the robot which detects the existence of he black tape. We want to keep the robot on the right edge between the black tape and white ground. When the IR sensor is above the white background, we turn the robot to the left by setting the right servo to full power clockwise and the left servo to low power counter-clockwise. When the IR sensor is above the black tape, we turn the robot to the right by setting the left servo to full power clockwise and the right servo to low power counter-clockwise. This not only allowed us to follow a stright line, but lines with relatively large curvatures as well.
<div style="text-align: center">
<iframe width="534" height="300" src="https://www.youtube.com/embed/kUykOyhvOOE" frameborder="0" allowfullscreen></iframe>
</div>


### Grid Traversal (figure eight)
In order to get our robot to tranverse grids, we implemented two more sensor. Two sensor are alighed on the front of the robot, and one sensor is implemented behind the front right sensor. The two sensor on the front allow straight line following and intersection detection. The sensor behind the right sensor make sure the robot returns to straight line following after turning at an intersection.

### Advantages of using an FSM
One advantage of using a FSM is that we can replace most the while and for loops which can cause infinite loops that are hard to debug. The delay function also causes many problems, but we can get around the delay function with FSM using the Arduino timer and millis(). In the future, we are going to have much more complex code and FSM can be very helpful in keeping our code organized and concise.

### Possible Software Improvements


### Possible Hardware Improvements
