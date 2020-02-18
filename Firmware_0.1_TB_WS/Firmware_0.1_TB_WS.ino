#include "Arduino.h"
#include <Servo.h>
#include <ros.h>
#include "std_msgs/Int16MultiArray.h"

//arduino to ROS connect init
class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};

//ROS node init
ros::NodeHandle_<NewHardware>  nh;

std_msgs::Int16MultiArray buttons_array_msg;

ros::Publisher chatter("tb_ws", &buttons_array_msg);

//Servos and pins init
  int serv44;
  int serv45;
  int serv46;

Servo servo44;
Servo servo45;
Servo servo46;

const byte NUMBER_OF_PINS = 4;
int inputPins[NUMBER_OF_PINS] = {22, 23, 24, 25};

//Servo ROS topic read function

void messageCbServo( const std_msgs::Int16MultiArray& data){

    serv44 = data.data[0];  //servos recieve angles
    serv45 = data.data[1];
    serv46 = data.data[2];

    servo44.write(serv44); // servo commands writes
    servo45.write(serv45);
    servo46.write(serv46);
}

ros::Subscriber<std_msgs::Int16MultiArray> subServo("tb_ws_servos", &messageCbServo );



void setup() {
  buttons_array_msg.data = (int*)malloc(sizeof(int) * 4);
  buttons_array_msg.data_length = 4;
  
  //ROS nodes init
  nh.initNode();
  nh.subscribe(subServo);
  nh.advertise(chatter);
  // Initial servos angles
  servo44.attach(44);servo44.write(40);
  servo45.attach(45);servo45.write(40);
  servo46.attach(46);servo46.write(40);

  for(int i=0; i < NUMBER_OF_PINS; i++){
      pinMode(inputPins[i], INPUT);
  }
  
}

void loop() {
//buttons read
  buttons_array_msg.data[0] = digitalRead(22);
  buttons_array_msg.data[1] = digitalRead(23);
  buttons_array_msg.data[2] = digitalRead(24);
  buttons_array_msg.data[3] = digitalRead(25);

//ROS publish
  chatter.publish(&buttons_array_msg);

//ROS node spin
  nh.spinOnce();
  delay(20);

}
