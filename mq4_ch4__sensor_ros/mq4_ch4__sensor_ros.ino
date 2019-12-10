#include <ros.h>
#include <std_msgs/Int16.h>
#include <Wire.h>

std_msgs::Int16 methane;

int meth;

#define pin_d 8
#define pin_a A0

ros::NodeHandle nh;
ros::Publisher chatter("chatter",&methane);

void setup()
{
  nh.initNode();
  nh.advertise(chatter);
}

void loop()
{
  int value;
  int limit;
  
  value= analogRead(pin_a);//reads the analaog value from the methane sensor's AOUT pin
  limit= digitalRead(pin_d);//reads the digital value from the methane sensor's DOUT pin
  Serial.print("Methane value: ");
  Serial.println(value);//prints the methane value
  Serial.print("Limit: ");
  Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
  delay(2000);

  methane.data=value;

  chatter.publish( &methane);
  nh.spinOnce();
}
