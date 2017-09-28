# Milestone 2

### Trasure Detector

### Wall Detection
In order to detect walls we used a short range proximity sensor. The sensor uses an IR sensor and an LED to detect large obstacles in front of the sensor. It takes 5V as input, and the output varies between 0.3V and 3.3V. We used the ADC on the Arduino to convert the analog output from the sensor to discrete values and through some trial and error, we decided on a threshold of 450 to detect walls. 

Please, refer to the following video to see how the circuit behaved according to the presence of a wooden wall:

<div style="text-align: center">
<iframe width="534" height="300" src="https://www.youtube.com/embed/eG_E8hI7VyI" frameborder="0" allowfullscreen></iframe>
</div>

This worked reasonably well however we noticed that when the sensor was very close to the wall, and ~30cm away, it would give the same value. Our threshold was higher than that value so that did not affect our robot significantly. We added our sensor to the robot and added it into the code by adding a state called TURN_AROUND, where the robot would turn 180 degrees if it sensed a wall in front of it.

Below is a snippet of the Arduino code that performed the turn-around when detecing a wall:

```c
arduino code
```

Please, refer to the following video to see how the robot detected the walls and turned around:

**INSERT VIDEO HERE**

### Conclusions
For future milestones and labs, we are starting to merge all the software and hardware components inside the robot so we can plan ahead on how to structure the code and even designing a new sophisticated model to assemble all the parts. We are also working on integrating a PID system in order to provide efficiency and precision in movement. 