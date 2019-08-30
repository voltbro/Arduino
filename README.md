## Description

These are "firmware" sketches for embedded TurtleBro Arduino compatible controller. LOAD for servos. LED for FASTLed lib and parktronik sample. And JOY is for Arduino compatible joystik JoyBro

## Setup

For uploading this sketches to embedded Arduino you need to make new ROS-library for Arduino onto TurtleBro RaspberyPI:
```
cd 
rosrun rosserial_arduino make_libraries.py
```
This create ros_lib directory on RaspberyPI so you need to copy it to your PC (most common to ~/Arduino/libraries) and recompile your Arduino libs.

## Package install

We provide TurtleBro with preinstalled Turtlebro pakkage, but if you are going to rebuil all, you need to reinstall  "[Turtlebro](https://github.com/voltbro/turtlebro)" and "[Joybro](https://github.com/voltbro/joybro)" packages on your RaspberryPi
