#include <Servo.h>
#include <ros.h>
#include <joybro/JoyBro.h>
#include "std_msgs/Int16MultiArray.h"

//arduino to ROS connect init
class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};

//ROS node init
ros::NodeHandle_<NewHardware>  nh;

//Servos and pins init

#define SPD 10
#define ROT 12

  int serv44;
  int serv45;

Servo servo44;
Servo servo45;
Servo servo46;

//Joystick ROS topic read function

void messageCb( const joybro::JoyBro& data){

    servo46.write(int((data.slider2/6)+3)); //arm up/down

    if(data.btn1==1) //rover head zero pozition call
    {
      servo45.write(100);
      servo44.write(75);
    }

    
    if(data.slider1<=512) //rotation of drill settings
    {
    digitalWrite(ROT,1);
    analogWrite(SPD,(255 - data.slider1/2));
    }
    else
    {
    digitalWrite(ROT,0);
    analogWrite(SPD,(data.slider1/2));
    }
}

ros::Subscriber<joybro::JoyBro> sub("joybro", &messageCb );


//Roverhead ROS topic read function

void messageCbR( const std_msgs::Int16MultiArray& data){

    //servo44.write(int((data.slider1/6)+3));
    serv45 = data.data[0];  //rover head up-down
    serv44 = data.data[1]; // rover head left-rigth
    
    if (serv44 <= 0) //rover head settings
    {
      if (serv44 <= -90)
      {
        serv44 = 180;
      }
      else
      {
      serv44 = 0;
      }
    }
    if (serv44 >= 180)
    {
      serv44 = 180;
    }
 
    if (serv45 <= 0)
    {
      if (serv45 <= -90)
      {
        serv45 = 180;
      }
      else
      {
      serv45 = 0;
      }
    }
    if (serv45 >= 180)
    {
      serv45 = 180;
    }

      servo45.write(serv45); // rover head servo commands
      servo44.write(serv44);


}

ros::Subscriber<std_msgs::Int16MultiArray> subR("roverhead", &messageCbR );


void setup() {
  //ROS nodes init
  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(subR);
  // Initial servos angles
  servo44.attach(44);servo44.write(70);
  servo45.attach(45);servo45.write(70);
  servo46.attach(46);servo46.write(70);
  //power shield pins init
  pinMode(SPD, OUTPUT);
  pinMode(ROT, OUTPUT);

}


void loop() {
//ROS node spin
  nh.spinOnce();
  delay(1);

}
