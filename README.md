# R2D2
Create yourself an R2D2 robot!

# Getting Started
There is a lot to setup with this. Considering this project includes electronics, 3d printing, robotics, software engineering, AI and more, this is both an expensive and complex project. 

## Essentials
To get started, you will need:
 - A 3d printer w/ plenty of filament
 - A soldering iron
 - Good working knowledge of: 
    - Electronics
    - 3d Printing
    - Linux
    - Docker
    - Robotics Operating System 2 (ROS2)
    - Python
    - Machine Learning and AI

## Parts
You will need the following parts to mimic this design exactly:

### Computing Core
1) Raspberry Pi 5 8GB: https://www.canakit.com/raspberry-pi-5-8gb.html 
2) Raspberry Pi AI Kit with Extended Turbine Case, Fan, and Mega Heat Sink: https://www.canakit.com/raspberry-pi-ai-kit-with-case.html
    - You can also order these parts separate. 
    - The AI Kit is a Hailo 8L accelerator with up to 13 TOPS (Trillions of Operations Per Second) inferencing performance.
3) 512 GB MicroSD Card: https://www.amazon.com/dp/B09X7C2GBC 

### Movement
1) TT Motor DC 3-6V Gearbox Motor Dual Shaft: https://www.amazon.com/AEDIKO-Motor-Gearbox-Shaft-200RPM/dp/B099Z85573/
2) 3.3V to 5V Level Shifter: https://www.amazon.com/dp/B07F7W91LC
3) Caster Wheels: https://www.amazon.com/dp/B0CZ9GNXSQ
4) Accelerometer/Gyroscope: https://www.amazon.com/dp/B00LP25V1A

### Power
1) 12V 5200mAh Battery: https://www.amazon.com/dp/B0C242DYT1
2) DC 9V 12V 24V to DC 5V 5A Buck Converter: https://www.amazon.com/dp/B07XXWQ49N

### Imaging
1) USB Camera: https://www.amazon.com/Ximimark-Camera-Raspberry-Require-Drivers/dp/B07GWP2SGV/
2) LIDAR: TBD

## 3-D Prints
For me, it was helpful to have an easy test board before proceeding with the design of R2D2.

### Test Board
I created a circular plastic body with M3-sized holes and two expansion shafts to support multiple layers. For each layer, print:
- (1) roundBody 
- (2-3) shaftExpansion

You can access the stl prints in print/test/
I purchased these M3 screws from Amazon: https://www.amazon.com/dp/B0BMQFHDBH

This 3D print was very helpful to mount the motors to the bottom of the board: https://www.thingiverse.com/thing:1473508 

### R2D2
TBD

## Electronics
Here is the schematic for setting up this system:
TBD

# Installation
1) Load up your Raspberry PI 5 with a 64-bit installation of Raspberry PI. The version I used was Raspberry PI OS 12.7 64-bit.
2) Once the PI is booted and ready, copy the contents of this repo into a directory you prefer.
3) Open a terminal up and cd to the src directory. 
4) Run ```sudo pi/install.sh``` - this will install Docker, ROS, and a number of other dependencies
5) Finally, you should be able to run ```sudo run.sh``` - this will run the robotics engine

# Running It
TBD