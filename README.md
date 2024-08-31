# C-programming-for-PIC16F877A

This repository contains different codes developed for programming PIC16F877A used in my Bachelors final year project which was a maximum power point tracking (MPPT) based solar charge controller module and then installing it in an existing locally manufactured uninterrupted power supply (UPS) unit.

Each of these codes were first tested in real time simulations using Proteus software before burning it into our PIC microcontroller. An example of one such simulation is shown below in a screen recording:

https://github.com/user-attachments/assets/bf58d453-6f7e-4655-af93-e8af292d6d57

And here's a picture of another simulation which shows how the code is used to generate the desired pulse width modulation (PWM) to control the MOSFET switching inorder to track the maximum power point as well as displaying the results on the LCD. Moreover, the simulation was used to test the final code which also control the functionality of the UPS unit as to when it should use the solar module via an SPDT relay seen in the figure below:

![Proteus simulation](https://github.com/user-attachments/assets/f78748fd-f456-4b37-ab68-ffaf14090747)


