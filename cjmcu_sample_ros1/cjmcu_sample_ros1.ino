#include <ros.h>
#include <std_msgs/Float32.h>
#include <cjmcu/cjmcu8128.h>

#include <geometry_msgs/Twist.h>

#include <Wire.h>
#include <SparkFunCCS811.h>
#include <SparkFunBME280.h>
#include <ClosedCube_HDC1080.h>

std_msgs::Float32 value;
geometry_msgs::Twist sensval;

#define CCS811_ADDR 0x5A    //Alternate I2C Address

CCS811 myCCS811(CCS811_ADDR);
ClosedCube_HDC1080 myHDC1080;
BME280 myBME280;

ros::NodeHandle nh;

ros::Publisher chatter("abs", &sensval);

void setup()
{
  nh.initNode();
 
  nh.advertise(chatter);


  myBME280.settings.commInterface = I2C_MODE;
  myBME280.settings.I2CAddress = 0x76;
  myBME280.settings.runMode = 3; //Normal mode
  myBME280.settings.tStandby = 0;
  myBME280.settings.filter = 4;
  myBME280.settings.tempOverSample = 5;
  myBME280.settings.pressOverSample = 5;
  myBME280.settings.humidOverSample = 5;

  myBME280.begin();
  myHDC1080.begin(0x40);

  //It is recommended to check return status on .begin(), but it is not required.
  CCS811Core::status returnCode = myCCS811.begin();
  

  if (returnCode != CCS811Core::SENSOR_SUCCESS)
  {
    //Serial.println(".begin() returned with an error.");
    while (1);      //No reason to go further
  }
  
}


void loop() 
{
  if (myCCS811.dataAvailable())
  {
     //If so, have the sensor read and calculate the results.
    //Get them later
    myCCS811.readAlgorithmResults();

   // Serial.print("P[");
    float a = myBME280.readFloatPressure() * 0.00750062 ;
   // Serial.print("mmHg] ");

   // Serial.print("Alt[");
    float b = myBME280.readFloatAltitudeMeters();
   // Serial.print("m ");

   // Serial.print("] Temp[");
    float c = myHDC1080.readTemperature();
   // Serial.print("C] RH[");
    float d = myHDC1080.readHumidity();

  //  Serial.print("%] CO2[");
    float e = myCCS811.getCO2();  //Returns calculated CO2 reading
  //  Serial.print("] tVOC[");
    float f = myCCS811.getTVOC(); //Returns calculated TVOC reading
  //  Serial.print("] sec[");
    
  //  Serial.print(millis()/1000); //seconds since start
  //  Serial.print("]");
  //  Serial.println();
    
    //compensating the CCS811 with humidity and temperature readings from the HDC1080
    myCCS811.setEnvironmentalData(myHDC1080.readHumidity(), myHDC1080.readTemperature()); 

    sensval.linear.x = a;
    sensval.linear.y = b;
    sensval.linear.z = c;

    sensval.angular.x = d;
    sensval.angular.y = e;
    sensval.angular.z = f;
    
  

  delay(2000); //Don't spam the I2C bus

  float x[7]={a,b,c,d,e,f};
  for(int i=0 ; i<7 ; i++)
  { value.data = x[i];
  }

    chatter.publish(&sensval);
   
   nh.spinOnce();
   
  }
  
}
