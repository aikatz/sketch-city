# Milestone 4: Display of Maze Information

### Milestone 3 Update
We have worked on finishing the tasks in Milestone 3 and made some significant progress. Our DFS code is complete but stills needs debugging. We tested extensively and believe that the back-pointer is working.
The picture below shows what we get when we tested the alogrithm on a small 2x2 maze.
<div style="text-align:center"><img src ="../pictures/dfsposition.jpg" /></div>

### Wall Detection and Treasure Detection
We had upgraded to new wall detection sensor that are much are accurate and reliable. We were also able to read from multiple I2C sensors on the same bus. So that now we do not have to use a MUX or schmist trigger because we have 5 analog inputs left: 3 for line sensors, 1 for IR, and 1 for the microphone. We tuned the range thresholds and updated our code according to the new sensors. 

We copied the circuit from Lab 2 to a soldered board and mounted it on the top of our robot. 
The picture below shows what our robot currently looks like with new wall sensors and the tresure sensors.
<div style="text-align:center"><img src ="../pictures/IMG_5643.JPG" /></div>

### Display
We further divided each black we drew in previous labs into a 12 by 12 array and assigned color according to maze information. The background color, wall color, and current position are assigned directly and the tresure colors are assigned with reference colors. 
```C
wire [7:0] treasure_color;
	 assign treasure_color = (treasure_sel == 1) ? 8'b111_000_00 : 
	                         (treasure_sel == 2) ? 8'b000_111_00: 
				 (treasure_sel == 3) ? 8'b000_000_11: 8'b000_000_00;
```
In the follwoing display, the walls are represented by the white stripes. Pink squares represent unexplored regions and green square represents current position. The red, green, and blue T represents treasures.
<div style="text-align:center"><img src ="../pictures/20171121_211721.jpg" /></div>

### Done Signal
Since we were not able to successfully communicate between arduino and FPGA, and maze exploration is not done either, we were not able to implement a done signal. 

