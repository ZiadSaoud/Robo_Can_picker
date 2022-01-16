#include <ESP8266WiFi.h>
#define   ROSSERIAL_ARDUINO_TCP
#include <ros.h>
#include <hello_world/gripper_srv.h>
#include <hello_world/servo_srv.h>
#include <std_msgs/Float32.h>
#include <geometry_msgs/Twist.h>
#include <Messenger.h>

std_msgs::Float32 dis;

const char* ssid     = "WIFI_Name";
const char* password = "WIFI_Password";
// Set the rosserial socket server IP address
IPAddress server(10,0,0,18);
// Set the rosserial socket server port
const uint16_t serverPort = 11411;

Messenger message = Messenger();

ros::NodeHandle nh;
ros::Publisher pub("/distance", &dis);
void callBack(const geometry_msgs::Twist& vel){
  //send x component to arduino
  float x = vel.linear.x;
  float y = vel.linear.y;
  float z = vel.linear.z;
  float xa = vel.angular.x;
  float ya = vel.angular.y;
  float za = vel.angular.z;
  Serial.print("l "); Serial.print(x); Serial.print(" "); Serial.print(y); Serial.print(" "); Serial.print(z); Serial.print(" ");
  Serial.print("a "); Serial.print(xa); Serial.print(" "); Serial.print(ya); Serial.print(" "); Serial.print(za); Serial.print(" \r");
  //nh.loginfo("sent");
}

void srv_callBack(const hello_world::gripper_srv::Request &req,hello_world::gripper_srv::Response &res){
  Serial.print("g "); Serial.print(req.angle); Serial.print(" \r");
  if(req.angle == 60){
    res.done = true;
  }
  if(req.angle == 30){
    res.done = false;
  }
}
void servo_callBack(const hello_world::servo_srv::Request &req,hello_world::servo_srv::Response &res){
  Serial.print("s "); Serial.print(req.angle); Serial.print(" \r");
  res.position = req.angle;
}
void msg_callback(){
  if(message.checkString("d")){
    dis.data = message.readDouble();
    pub.publish(&dis);
  }
}

// Ros subscribers and Service servers
ros::Subscriber<geometry_msgs::Twist> sub("cmd_vel", callBack);
ros::ServiceServer<hello_world::gripper_srv::Request, hello_world::gripper_srv::Response> gripper_server("gripper_service",&srv_callBack);
ros::ServiceServer<hello_world::servo_srv::Request, hello_world::servo_srv::Response> servo_server("servo_service",&servo_callBack);
void setup()
{
  Serial.begin(115200);
   while(!Serial);
   Serial.print("l "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" ");
   Serial.print("a "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" \r");
   Serial.print("g "); Serial.print(95); Serial.print(" \r");
   Serial.print("s "); Serial.print(65); Serial.print(" \r");
  // Connect the ESP8266 to the wifi AP
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  message.attach(msg_callback);
  nh.getHardware()->setConnection(server, serverPort);
  nh.initNode();
  nh.subscribe(sub);
  nh.advertiseService(gripper_server);
  nh.advertiseService(servo_server);
  nh.advertise(pub);
}

void loop(){
  if (!nh.connected()) {
   Serial.print("l "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" ");
   Serial.print("a "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" "); Serial.print(0); Serial.print(" \r");
   Serial.print("g "); Serial.print(95); Serial.print(" \r");
   Serial.print("s "); Serial.print(65); Serial.print(" \r");
    
  } else {
  while(Serial.available()) {
    message.process(Serial.read());
  }
  }
  nh.spinOnce();
  delay(1);
}
