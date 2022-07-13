#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// called this way, it uses the default address 0x40
Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver();
// you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver(0x41);

void setup() {
  Serial.begin(115200);
  Serial.println("16 channel PWM test!");

  pwm0.begin();
  pwm0.setPWMFreq(1600);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode
  // some i2c devices dont like this so much so if you're sharing the bus, watch
  // out for this!
#ifdef TWBR
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
#endif


  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A



}

void loop()
{
  pwm0.setPWM(0, 4095, 0); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  
  pwm0.setPWM(4, 4095, 0); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A
}
