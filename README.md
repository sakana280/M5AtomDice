# About
An electronic dice for the M5Stack Atom Matrix, 
less likely to roll off the table compared with a regular analog cube dice.
Press the screen or shake the unit to "roll" the dice.

On first start, the dice operates in "demo" mode, counting 1-6 on a loop. Demo mode finishes after the first "roll". During a roll, the display rapidly cycles through numbers 1-6. When rolling stops (screen press released or unit stopped shaking), a random number is displayed. The number rolled remains on screen indefinitely until the next roll.

# Build
Use Visual Studio Code with the PlatformIO extension to build and upload this program.

# Acknowledgements
Code for the IMU (MPU6886) has been modified from [M5Stack's M5Atom library](https://github.com/m5stack/M5Atom) MIT License Copyright (c) 2020 M5Stack