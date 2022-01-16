#include <Servo_Hardware_PWM.h>
#include <NewPing.h>
#include<Messenger.h>


#define TRIGGER_PIN  12  
#define ECHO_PIN     11
#define MAX_DISTANCE 100

Servo myServo;
Servo myServo_1;
NewPing dis_sensor(TRIGGER_PIN,ECHO_PIN,MAX_DISTANCE);

float lx = 0.107;
float ly = 0.082;
float R  = 0.04;
long prev_time = 0L;

#define motor1_fr 10 //22,23
#define motor2_fl 9  //24,25
#define motor3_br 5  //26,27
#define motor4_bl 6  //28,29

#define motor1_1 23
#define motor1_2 22

#define motor2_1 24
#define motor2_2 25

#define motor3_1 26
#define motor3_2 27

#define motor4_1 29
#define motor4_2 28

Messenger message = Messenger();

float lin_vel[3]= {0.0,0.0,0.0};
float ang_vel[3]= {0.0,0.0,0.0};
float wheels_vel[4] = {0.0,0.0,0.0,0.0};

void callBack(){
  if(message.checkString("l")){
    for(int i=0;i<3;i++){
      lin_vel[i]=message.readDouble();
    }
  }
  if(message.checkString("a")){
    for(int i=0;i<3;i++){
      ang_vel[i]=message.readDouble();
    }
  }
  if(message.checkString("g")){
    myServo.write(message.readInt());
    delay(270);
    myServo.write(95);
  }
   if(message.checkString("s")){
    myServo_1.write(message.readInt());
  }
  
}
void update_Wheels(){
  wheels_vel[0] = (lin_vel[0]-lin_vel[1]-(lx+ly)*ang_vel[2])/(float) R;
  wheels_vel[1] = (lin_vel[0]+lin_vel[1]+(lx+ly)*ang_vel[2])/(float) R;
  wheels_vel[2] = (lin_vel[0]+lin_vel[1]-(lx+ly)*ang_vel[2])/(float) R;
  wheels_vel[3] = (lin_vel[0]-lin_vel[1]+(lx+ly)*ang_vel[2])/(float) R;
}

void set_Wheel_Speed(int wheel, float Speed, int pin1, int pin2){
  if(Speed == 0){
    analogWrite(wheel,255);
    digitalWrite(pin1,LOW);
    digitalWrite(pin2,LOW);
  }else{
    if(Speed>0){
      float s = Speed*9.5493;
      s = mapf(s,0,150,0,255);
      int x = round(s);
      x = constrain(x,0,255);
      analogWrite(wheel,x);
      digitalWrite(pin1,HIGH);
      digitalWrite(pin2,LOW);
    }else{
      float s = -Speed*9.5493;
      s = mapf(s,0,150,0,255);
      int x = round(s);
      x = constrain(x,0,255);
      analogWrite(wheel,x);
      digitalWrite(pin1,LOW);
      digitalWrite(pin2,HIGH);
    }
  }
}

void MoveRobot(){
  update_Wheels();
  set_Wheel_Speed(motor1_fr,wheels_vel[1],motor1_1,motor1_2);
  set_Wheel_Speed(motor2_fl,wheels_vel[0],motor2_1,motor2_2);
  set_Wheel_Speed(motor3_br,wheels_vel[3],motor3_1,motor3_2);
  set_Wheel_Speed(motor4_bl,wheels_vel[2],motor4_1,motor4_2);
}
void sendDistance(){
 float distance = dis_sensor.ping_cm();
 Serial.print("d "); Serial.print(distance); Serial.print(" \r");
}

float mapf(float x,float min_1,float max_1,float min_2,float max_2){
  float z = (x-min_1)*(max_2-min_2)/(max_1-min_1)+min_2;
  z= z*100;
  int s = z;
  z = s/100.0;
  return z;
}

void setup() {
  // put your setup code here, to run once:
   myServo.attach(44);
   myServo_1.attach(45);
   myServo.write(95);
   myServo_1.write(65);
   
   Serial.begin(115200);
   while(!Serial);
   message.attach(callBack);

   pinMode(5,OUTPUT);
   pinMode(6,OUTPUT);
   pinMode(9,OUTPUT);
   pinMode(10,OUTPUT);
   
   pinMode(22,OUTPUT);
   pinMode(23,OUTPUT);
   pinMode(24,OUTPUT);
   pinMode(25,OUTPUT);
   pinMode(26,OUTPUT);
   pinMode(27,OUTPUT);
   pinMode(28,OUTPUT);
   pinMode(29,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(Serial.available()){
    message.process(Serial.read());
  }
  MoveRobot();
  if(millis()-prev_time>35){
    sendDistance();
    prev_time = millis();
  }
}
