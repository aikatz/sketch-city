## LAB 2: Analog Circuitry and FFTs

### Objective

In this lab, we added different sensors to our robot, and made analog circuits
and a digital filter to interface with the Arduino. One is a microphone circuit
that detects a 660Hz whistle blow signifying the beginning of your maze mapping.
The other captures inputs from an IR sensor to detect an IR treasure blinking
at 7kHz.

## Acoustic Team

<div style="text-align:center"> <img src="../pictures/lab2/mic.jpeg"/> </div>

### Materials

  * Arduino Uno
  * Electret Microphone Amplifier - MAX4466 with Adjustable Gain
  * ~3 kΩ resistor

### Analysis of the microphone in the built-in system

When we started assembling our circuit, we made sure to test its different
components in order to make debugging easier in the future. The Electret Microphone was working perfectly according to the oscilloscope, since we could appreciate the difference in certain tones and noises. We obtained the following feedback after setting up an effective low-pass filter with a cut off frequency of 796 Hz using a 1µF capacitor and a ~200Ω resistor:

**Snapping video to be placed here**

For the purpose of the competition, we needed to amplify the signal since there is going to be a lot of background noise in Phillips Hall. We assembled a non-inverting amplifier with 10kΩ and 100kΩ, therefore having a gain of 10. However, we were not getting a signal at all, and the faculty even checked and validated our circuit before reaching any false conclusions. It was in fact a defectuous op amp, and many of the other teams were having the same issue. That is why we switched to a fully functional Electret Microphone Amplifier - MAX4466. This device already contained the desired analog filters, and an adjustable gain ranging from 4 to 27 -measured by our team using the function generator and the oscilloscope-. From this lab, until the competition, we will work with such device to solve the acoustic challenges.

### Testing the Fast-Fourier-Transform algorithm provided by the Arduino

First of all, we started testing the function generator by hooking it up directly to the oscilloscope and make sure it was working properly. We tested it with a 660Hz frequency, 2.55Vpp amplitude, and 1.25V offset. Here is the feedback of the experiment:

Function Generator         |  Oscilloscope
:-------------------------:|:-------------------------:
![](../pictures/lab2/functGenTest.jpg)  |  ![](../pictures/lab2/oscilloscopeTest.jpg)

With the function generator working properly, it was time to test the FFT algorithm provided by the Arduino Library ```c <FFT.h> ```. Before assembling the circuit together, we had to do some work to figure out where our 660Hz frequency would fall inside the output from the FFT. 

### Testing the circuit with a 660 Hz tone



### Conclusions



## IR Sensor Team



### Materials
