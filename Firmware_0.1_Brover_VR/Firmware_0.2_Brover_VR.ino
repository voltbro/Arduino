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

  int head_offset_x = 90;
  int head_offset_y = 90;
  int clr_set = 0;

Servo servo44;
Servo servo45;
Servo servo46;

//Joystick ROS topic read function

void messageCb( const joybro::JoyBro& data){

    servo46.write(int((data.slider2/6)+3)); //arm up/down

    clr_set = data.btn1; //rover head zero_pozition_call write to global var

    
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

ros::Subscriber<joybro::JoyBro> sub("joybro", &messageCb ); //ROS node 'Joybro' call


//Roverhead ROS topic read function

void messageCbR( const std_msgs::Int16MultiArray& data){



    serv45 = data.data[0] - head_offset_y + 90;  //rover head up-down
    serv44 = data.data[1] - head_offset_x + 90; // rover head left-rigth

    if (clr_set == 1)
    {
    head_offset_y =  data.data[0];  //rover head up-down global var offset update
    head_offset_x = data.data[1]; // rover head left-rigth global var offset update
    }
    
    if (serv44 <= 0) //rover head settings left-rigth
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
 
    if (serv45 <= 0) //rover head settings up-down
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

ros::Subscriber<std_msgs::Int16MultiArray> subR("roverhead", &messageCbR ); //ROS node 'roverhead' call


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
