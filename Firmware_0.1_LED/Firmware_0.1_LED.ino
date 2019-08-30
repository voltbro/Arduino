#include <ros.h>
#include <FastLED.h>

#include "std_msgs/ByteMultiArray.h"

#define DATA_PIN 30
#define NUM_LEDS 24
#define BRIGHTNESS 200

CRGB leds[NUM_LEDS];


int pincolorblue = 0;
int pincolorgreen = 0;
class NewHardware : public ArduinoHardware
{
  public:
  NewHardware():ArduinoHardware(&Serial1, 115200){};
};


void messageCb(const std_msgs::ByteMultiArray& arrscan)
{
  int pincolorred;
  int pincologreen;
  int pincolorblue;
  int i = 0;
 for(i=0;i<24;i++)
 {
  pincolorred = arrscan.data[i*3];
  pincolorgreen = arrscan.data[i*3+1];
  pincolorblue = arrscan.data[i*3+2];
  leds[23-i].r = pincolorred*2;
  leds[23-i].g = pincolorgreen*2;
  leds[23-i].b = pincolorblue*2;
  FastLED.show();
 }  
}

ros::NodeHandle_<NewHardware>  nh;
ros::Subscriber<std_msgs::ByteMultiArray> sub("toggle_led", &messageCb);

void setup()
{
  delay(3000); // sanity delay
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
  FastLED.setBrightness( BRIGHTNESS );
  nh.initNode();
  nh.subscribe(sub);
}

void loop()
{
  nh.spinOnce();
  delay(1);
}
