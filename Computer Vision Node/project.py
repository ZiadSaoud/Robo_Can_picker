#!/usr/bin/env python
from __future__ import print_function
import rospy
import cv2
import numpy as np
from pyzbar.pyzbar import decode
from sensor_msgs.msg import Image
from geometry_msgs.msg import Twist
from std_msgs.msg import Float32
from hello_world.srv import servo_srv
from cv_bridge import CvBridge, CvBridgeError
#Run the following command to convert compressed images to Raw images
#rosrun image_transport republish compressed in:=/raspicam_node/image raw out:=/raspicam_node/image_raw
def dis_callback(dist):
    global distance
    distance = dist.data

def pub_vel(x,y,z,xx,yy,zz):
    vel = Twist()
    vel.linear.x = x
    vel.linear.y = y
    vel.linear.z = z
    vel.angular.x= xx
    vel.angular.y= yy
    vel.angular.z= zz
    vel_pub.publish(vel)

def callback(data):
    global servo_angle
    global prev_data
    global result
    global pick_can
    ########################################convert ROS image to openCV image
    try:
      cv_image = bridge.imgmsg_to_cv2(data, "bgr8")
    except CvBridgeError as e:
      print(e)
    ########################################convert ROS image to openCV image
    code = decode(cv_image)
    if len(code) > 0:
      #print(code[0].rect.width)
      Data = code[0].data
      pts = np.array([code[0].polygon],np.int32)
      pts = pts.reshape((-1,1,2))
      cv2.polylines(cv_image,[pts],True,(255,0,0),2)
      pts2 = code[0].rect
      #print((pts2[0],pts2[1]))
      cv2.putText(cv_image,Data,(pts2[0],pts2[1]),cv2.FONT_HERSHEY_SIMPLEX,0.9,(255,0,0),2)
      result.write(cv_image)
      ##########################################
      if prev_data == Data:
        if distance > 25:
          if servo_angle == 65:
            pub_vel(0.3,0,0,0,0,0)
          elif servo_angle == 150:
            pub_vel(0,0.3,0,0,0,0)
          elif servo_angle == 0:
            pub_vel(0,-0.3,0,0,0,0)
        else:
          pub_vel(0,0,0,0,0,0)
          rospy.wait_for_service("/servo_service")
          if Data == "Go left":
            servo_angle = 150
            prev_data = "Go forward"
            try:
              serv = rospy.ServiceProxy("/servo_service",servo_srv)
              resp = serv(150)
            except rospy.ServiceException:
              rospy.WARN("Failed to call Service")
          elif Data == "Go forward":
            servo_angle = 65
            prev_data = "Go right"
            try:
              serv = rospy.ServiceProxy("/servo_service",servo_srv)
              resp = serv(65)
            except rospy.ServiceException:
              rospy.WARN("Failed to call Service")
          elif Data == "Go right":
            servo_angle = 0
            prev_data = "Cans are to the front"
            try:
              serv = rospy.ServiceProxy("/servo_service",servo_srv)
              resp = serv(0)
            except rospy.ServiceException:
              rospy.WARN("Failed to call Service")
          elif Data == "Cans are to the front":
            servo_angle = 65
            prev_data = "Done"
            pick_can = True
            try:
              serv = rospy.ServiceProxy("/servo_service",servo_srv)
              resp = serv(65)
            except rospy.ServiceException:
              rospy.WARN("Failed to call Service")

    else:
      if pick_can:
        print("Implement can picking process here")
      else:
        #print("publish zero speed")
        pub_vel(0,0,0,0,0,0)
    #cv2.imshow("capture",cv_image)
    #cv2.waitKey(1)
    ########################################convert openCV image to ROS image and publish it
    try:
      image_pub.publish(bridge.cv2_to_imgmsg(cv_image, "bgr8"))
    except CvBridgeError as e:
      print(e)
    ########################################convert openCV image to ROS image and publish it


rospy.init_node("Vision_Node",anonymous= True)
pick_can = False
distance = 0
prev_data = "Go left"
servo_angle = 65
image_pub = rospy.Publisher("image_output",Image,queue_size=1)
vel_pub   = rospy.Publisher("cmd_vel",Twist,queue_size=1)
bridge = CvBridge()
image_sub = rospy.Subscriber("raspicam_node/image_raw",Image,callback)
dis_sub = rospy.Subscriber("/distance",Float32,dis_callback)
frame_width = 410
frame_height = 308
size = (frame_width, frame_height)
result = cv2.VideoWriter('filename.avi', cv2.VideoWriter_fourcc(*'MJPG'), 10, size)
try:
  rospy.spin()
except KeyboardInterrupt:
  rospy.loginfo("shutting  down")
result.release()
cv2.destroyAllWindows()
