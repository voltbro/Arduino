#include <Servo.h>
#include <ros.h>
#include <joybro/JoyBro.h>
#include "std_msgs/Int16MultiArray.h"

class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};

ros::NodeHandle_<NewHardware>  nh;

#define SPD 10
#define ROT 12

  int serv44;
  int serv45;

Servo servo44;
Servo servo45;
Servo servo46;

void messageCb( const joybro::JoyBro& data){

    //servo44.write(int((data.slider1/6)+3));
    servo46.write(int((data.slider2/6)+3));
    //servo45.write(int(((data.right_y+512)/6)+3));
    //servo44.write(int(((data.right_x+512)/6)+3));
    if(data.slider1<=512)
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


void messageCbR( const std_msgs::Int16MultiArray& data){

    //servo44.write(int((data.slider1/6)+3));
    serv45 = data.data[0];  //vverh-vniz
    serv44 = data.data[1]; // vlevo-vpravo
    
    if (serv44 <= 0)
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

      servo45.write(serv45);
      servo44.write(serv44);


}

ros::Subscriber<std_msgs::Int16MultiArray> subR("roverhead", &messageCbR );


void setup() {

  nh.initNode();
  nh.subscribe(sub);
  nh.subscribe(subR);
  Serial.begin(115200);
  // put your setup code here, to run once:
  servo44.attach(44);servo44.write(70);
  servo45.attach(45);servo45.write(70);
  servo46.attach(46);servo46.write(70);
  pinMode(SPD, OUTPUT);
  pinMode(ROT, OUTPUT);

}


void loop() {
  // put your main code here, to run repeatedly:
  nh.spinOnce();
  //servo_test.write(180);
  delay(1);

}
