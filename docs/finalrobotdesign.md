# Final Robot Design
### Physical Design and Custom CAD Work
### Line Following
### Wall Detection
### Treasure Detection
### Maze-Solving Algorithm
### Radio Communication

### FPGA: FPGA + Arduino Communication
The FPGA takes a 7-bit signal as a message from the Arduino and outputs to a VGA display and a DAC connected to an audio jack. The 7-bit message can represent either the current position of the robot or the presence of walls and treasures. Each transmission from the Arduino is a set of two messages, the first containing the position and the second containing walls and treasures. The MSB of the message determines which of the two messages is being sent. When it is 0, the position is being sent, with the x-position as bits 0 to 1 and the y-position as bits 2 to 4. The the MSB is 1, the walls and treasures are being sent, with the treasures as bits 0 to 1 and the walls as bits 2 to 5. The treasures are represented as 00 for the 7kHz, 01 for 12 kHz, and 17 kHz for 17kHz. The walls are each represented as 1 bit in the order north, east, south, west from the MSB to the LSB. When the robot is done mapping the maze, it transmits a message of all 1’s, which is not normally possible.

The FPGA stores three hard-coded arrays of data containing the outputs to the DAC for playing the done signal, the color values for the treasure image, and the color values for the done image. A fourth array is updated based on the Arduino messages and stores whether the position has been visited, the walls for the given position, and the treasures for the position. The first message determines which array index to update and the new data consists of the second message’s contents with a 1 appended as the MSB to indicate the position is visited. This data array is initialized on reset to contain no treasures and the outer walls of the maze.


### FPGA: VGA
To display the maze, we use a series of select signals representing the north, east, south, and west walls, the presence of treasures, whether the position is visited, and whether the robot is done. The colors for visited, current position, unreachable positions, and walls are hard-coded variables and treasures are set from red, green, and blue based on the treasure bits of the position data. We calculate whether each pixel of the screen is part of the walls, the treasure image, outside of the maze area, or part of the done image. We also calculate which square of the maze the pixel is in. If the pixel is outside of the maze, it is set to black. If it is part of the done image, it will be set to the colors specified in that array if done signal is high or black otherwise. For the actual maze data, the code is best explained with an example. If the pixel maps to one of the walls and the select signal for that wall is high, the color will be set to the wall color. Otherwise, if the robot is done and the position is not visited, it is set to the unreachable color. If the robot is not done and the the maze position is the same as the current position, the color is set to the current position color. If it is not the current position and it is visited, the color is set to the visited color. If none of the conditions are met, it is set to white. Similar logic is used for the other parts of the image.

For the done audio signal, if the done signal is high, the code will run through the audio signal data array, updating the signal on the clock edge. If the robot is not done, it should not output anything. Below is a video of the FPGA displaying a maze on a screen, the messages are simulated, and not coming from a real robot, which is why the robots position simply scrolls from left to right. 

# put video of fpga here
