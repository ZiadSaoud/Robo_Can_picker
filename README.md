# Robo_Can_picker
## Description
The goal of the robot is to traverse a maze by reading QR codes, then pick a can using the on-board gripper. The robot will extract direction commands from the QR codes to navigate the maze and using color detection will locate the position of the can relative to the robot in order to pick it.

<img width="455" alt="image" src="https://user-images.githubusercontent.com/69092782/149664366-602b6118-aaf5-4362-93bf-74a9261f2e2d.png">


## Robot circuitry
<img width="904" alt="image" src="https://user-images.githubusercontent.com/69092782/149662376-a022586d-1d95-4979-b1ba-ffc6547cbc8c.png">

## Forward and Inverse Kinematics:
The forward and inverse kinematics model of the robot according to the following [article](https://research.ijcaonline.org/volume113/number3/pxc3901586.pdf):
<img width="520" alt="image" src="https://user-images.githubusercontent.com/69092782/149663166-86554b3f-f9c1-4298-a6c3-c3360a1e6811.png"> 
<img width="454" alt="image" src="https://user-images.githubusercontent.com/69092782/149663441-81356dc5-7b58-42dd-bd90-07a85455258f.png">

## ROS Computing Graph:
![rosgraph](https://user-images.githubusercontent.com/69092782/149664149-1c97d2f2-942f-494d-a6e0-910a96180f45.png)
![TX](https://user-images.githubusercontent.com/69092782/149664268-a489eb90-365f-4991-b92d-25799c160819.png)

The Raspberry pi, PC, and ESP8266 are connected to the same access point.
The raspicam_node Ros node is running on the raspberry pi and publishing `sensor_msgs/CompressedImage` message type on the topic `/raspicam_node/image/compressed`. 
Roscore is running on the pc, in order to connect the Ros node running on the pi to the Ros running on the pc run the following on the pi before launching the camera node `export ROS_MASTER_URI=http://PC Ip address:11311`

The Image_republisher node converts the `sensor_msgs/CompressedImage` message to `sensor_msgs/Image` message type. 

The Vision_node processes the images and distance data from the `/distance` topic and publishes `geometry_msgs/Twist` messages on the `/cmd_vel` topic to order the robot to move. 

The serial_node is running on the ESP8266 it offers the following custom services:
1. `gripper_srv`:service to open and close the gripper.
2. `servo_srv  `:service to set the servo attached to the camera and ultrasonic sensor to a specific angle. 

and publishes the distance read from the ultrasonic sensor over the topic `/distance`.

## Demo video
[![IMAGE](https://img.youtube.com/vi/Nt5qoOX6EoQ/0.jpg)](https://www.youtube.com/watch?v=Nt5qoOX6EoQ)

## Tools used:
Raspberry pi ubuntu 16.04 disk image with Ros already installed from [Ubiquity Robotics](https://www.ubiquityrobotics.com)



