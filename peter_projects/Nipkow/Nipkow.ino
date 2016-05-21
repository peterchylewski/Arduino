/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->	http://www.adafruit.com/products/1438
*/


#include <Wire.h>

// --- prepare motor

#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_MS_PWMServoDriver.h"

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);


// --- prepare light sensor

#include <Adafruit_Sensor.h>
#include "Adafruit_TSL2591.h"
Adafruit_TSL2591 tsl = Adafruit_TSL2591(2591);


// --- prepare div

uint32_t timer = 0;
uint16_t stepCount = 0;
uint8_t  stepDirection = FORWARD;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  //Serial.println("Stepper test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(20);  // 10 rpm


  tsl.setGain(TSL2591_GAIN_HIGH);                 // 428x gain
  tsl.setTiming(TSL2591_INTEGRATIONTIME_600MS);   // longest integration time (dim light)
  
}

void loop() {
 
  if (millis() - timer >= 1000) {
    timer = millis();
    myMotor->onestep(stepDirection, INTERLEAVE);

    stepCount++;
    if (stepCount >= 20) {
      stepDirection = stepDirection == FORWARD ? BACKWARD : FORWARD;
      stepCount = 0;
    }
    
    lightSensorAdvancedRead();
  }
  
  
  //Serial.println(millis() - timer);
  
  /*
  Serial.println("Single coil steps");
  myMotor->step(100, FORWARD, SINGLE); 
  myMotor->step(100, BACKWARD, SINGLE); 
*/
  /*
  Serial.println("Double coil steps");
  myMotor->step(100, FORWARD, DOUBLE); 
  myMotor->step(100, BACKWARD, DOUBLE);
 */
  /*
  Serial.println("Interleave coil steps");
  myMotor->step(20, FORWARD, INTERLEAVE); 
  myMotor->step(20, BACKWARD, INTERLEAVE); 
 */
  /*
  //Serial.println("Microstep steps");
  myMotor->step(20, FORWARD, MICROSTEP); 
  myMotor->step(20, BACKWARD, MICROSTEP);
*/

  
}

void lightSensorAdvancedRead(void)
{
  // More advanced data read example. Read 32 bits with top 16 bits IR, bottom 16 bits full spectrum
  // That way you can do whatever math and comparisons you want!
  uint32_t lum = tsl.getFullLuminosity();
  uint16_t ir, full;
  ir = lum >> 16;
  full = lum & 0xFFFF;
  
  Serial.print("{\"ms\": "); Serial.print(millis()); Serial.print(", ");
  Serial.print("\"ir\": "); Serial.print(ir); Serial.print(", ");
  Serial.print("\"full\": "); Serial.print(full); Serial.print(", ");
  Serial.print("\"visible\": "); Serial.print(full - ir); Serial.print(", ");
  Serial.print("\"lux\": "); Serial.print(tsl.calculateLux(full, ir));
  Serial.println("}");
  
}
