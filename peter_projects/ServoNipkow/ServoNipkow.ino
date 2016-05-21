
#include <Servo.h> 
 
Servo servoX;
Servo servoY;
 
int posX = 90;    // variable to store the servo position 
int posY = 90;


// --- prepare light sensor

#include <Wire.h>

#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);

void setup() 
{

  Serial.begin(115200);
  
  servoX.attach(5);
  servoY.attach(4);

  tsl.setGain(TSL2591_GAIN_MAX);                
  tsl.setTiming(TSL2591_INTEGRATIONTIME_100MS);
} 
 
void loop() 
{ 
  servoX.write(posX);   
  servoY.write(posY);
  delay(500);      
  
  for(posX = posX; posX>=0; posX-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    servoX.write(posX);              // tell servo to go to position in variable 'pos'

    lightSensorAdvancedRead();
    
    //delay(150);                       // waits 15ms for the servo to reach the position 
  } 

  posY--;
  posX = 90;
} 


void lightSensorAdvancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;

  Serial.print("{\"x\": "); Serial.print(posX); Serial.print(", ");
  Serial.print("\"y\": "); Serial.print(posY); Serial.print(", ");
  Serial.print("\"ms\": "); Serial.print(millis()); Serial.print(", ");
  Serial.print("\"ir\": "); Serial.print(ir); Serial.print(", ");
  Serial.print("\"full\": "); Serial.print(full); Serial.print(", ");
  Serial.print("\"visible\": "); Serial.print(full - ir); Serial.print(", ");
  Serial.print("\"lux\": "); Serial.print(tsl.calculateLux(full, ir));
  Serial.println("}");
  
}
