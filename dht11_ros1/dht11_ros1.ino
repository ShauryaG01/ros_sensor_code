#include <dht.h>
#include <ros.h>
#include <std_msgs/Int16.h>
#include <Wire.h> 
 
std_msgs::Int16 rel_humidity;
std_msgs::Int16 temperature;
 
int temp; 
int humi;

#define DHT11_PIN A0
dht DHT;
 
ros::NodeHandle nh;
ros::Publisher chatter1("chatter1", &rel_humidity);
ros::Publisher chatter2("chatter2", &temperature);
 
 
void setup()
{
  nh.initNode();
  nh.advertise(chatter1);
  nh.advertise(chatter2);
}
 
void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  Serial.print("temperature = ");
  int a = DHT.temperature;
  Serial.println(DHT.temperature);
  Serial.print("humidity = ");
  int b = DHT.humidity;
  Serial.println(DHT.humidity);
  delay(2000);
     
   rel_humidity.data = b;
   temperature.data = a;
   
   chatter1.publish( &rel_humidity);
   chatter2.publish( &temperature);
   nh.spinOnce();
}
