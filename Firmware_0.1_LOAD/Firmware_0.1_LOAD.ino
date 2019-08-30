#include <Servo.h>
#include <ros.h>
#include <joybro/JoyBro.h>

class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};

ros::NodeHandle_<NewHardware>  nh;

Servo servo44; //PbI4Ar
Servo servo45; //hvat


void messageCb( const joybro::JoyBro& data){

  int serv44;
  int serv45;

  serv44 = (data.slider2/6)+10;
  serv45 = (data.slider1/6)+10;

  if(serv44 <= 20)
    {
      serv44 = 20;
    }

  if(serv45 <= 80)
    {
      serv45 = 80;
    }
  if(serv44 >= 130)
  {
    serv44 = 130;
  }
  
    servo45.write(serv45);
    servo44.write(serv44);
}

ros::Subscriber<joybro::JoyBro> sub("joybro", &messageCb );

void setup() 
{
  // put your setup code here, to run once:  
  nh.initNode();
  nh.subscribe(sub);
  //Serial.begin(115200);
  
  servo44.attach(44);servo44.write(90);
  servo45.attach(45);servo45.write(70);
}

void loop() {
  // main code
  nh.spinOnce();
  delay(1);
  }
