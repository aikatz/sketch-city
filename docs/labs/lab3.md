## LAB 3: FPGA Video Controller and Sound Generation

---

### Objective
This lab is divided into two. One team will take at least two external inputs to the FPGA and display them on a screen. The other team will react to an external input to the FPGA of their choice and generate a short ‘tune’ consisting of at least three tones to a speaker via an 8-bit DAC. Both of these tasks will help towards the design of the final system. In the final competition, all maze information discovered by the robot must be transmitted from the basestation Arduino to the FPGA, and then drawn on a VGA monitor. Once the maze has been completely mapped, the FPGA must generate a short tune to be played on a speaker to signify that the maze-mapping is done.

### Graphics Team: Juan Joel Albrecht, Christine Ou, Yazhi Fan

### Materials
* 1 VGA screen
* 1 VGA cable
* 1 VGA connector
* 1 VGA switch
* Various resistors
* 2 external switches

### Logic explanation for the screen display

### Reading external inputs to FPGA
For this part of the lab, we used two external switches in order to interact with the Arduino and the FPGA. We provided both switches with 3.3V from the Arduino and connected their respective outputs to specific GPIO pins in the FPGA.

<div style="text-align:center"><img src ="../pictures/lab3/switch.jpg" /></div>

We used the following assignment operations to be able to read from the GPIO pins:

``` c
assign switch_x = GPIO_1_D[29];
assign switch_y = GPIO_1_D[25];
```

Then, we manipulated our blocks of pixels on the screen based on the readings from the variables *switch_x* and *switch_y*.

### Correctly updating a 4-bit array dependent on the inputs
Based on the reading from such pins, we then modified two selected digital output pins from the Arduino and set them to LOW/HIGH accordingly. Both signals output 5V and the DE0-Nano operates at 3.3V, therefore we need to create two voltage dividers:

### Drawing one box on the screen


### Description of how the DAC on the provided VGA connectors works and how the resistor values were chosen.


### Mapping external inputs to four different outputs on the screen


### Acoustic Team: Eric Berg, Alex Katz
