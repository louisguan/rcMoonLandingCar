/*---------------------------------------WS2812b-----------------------------------------*/
// https://flaviocopes.com/rgb-color-codes/
#include <FastLED.h>
#define LedNum 11
#define DataIn 15
#define ColorOrder RGB
#define ChipSet WS2812B
int Brightness = 255;
CRGB leds[LedNum];

/*---------------------------------------PCA9685 PWM-----------------------------------------*/
// https://www.itread01.com/content/1550066431.html
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>
//
Adafruit_PWMServoDriver pwm0 = Adafruit_PWMServoDriver(); // called this way, it uses the default address 0x40
//Adafruit_PWMServoDriver pwm1 = Adafruit_PWMServoDriver(0x41); // you can also call it with a different address you want
//Adafruit_PWMServoDriver pwm2 = Adafruit_PWMServoDriver(0x42); // you can also call it with a different address you want
//
#define L9110sFreq 1000
#define pwm1Freq 1000
#define pwm2Freq 1000

/*---------------------------------------Buzzer-----------------------------------------*/
// https://blog.csdn.net/weixin_40973138/article/details/103872327
// https://thecustomizewindows.com/2020/05/esp32-arduino-smartwatch-detector-with-buzzer/
#include <EasyBuzzer.h>
#define BuzzerPin 2

/*---------------------------------------ILI9341_DRIVER 320x240 2.2' TFT-----------------------------------------*/
// https://blog.csdn.net/weixin_45488643/article/details/113702365

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_BL    25
int RotationDisplay = 3;

// http://www.rinkydinkelectronics.com/t_imageconverter565.php
#include "SeaOtterLogo.h"

// https://github.com/Bodmer/TFT_eSPI/blob/master/TFT_eSPI.h
// Default color definitions
#define TFT_BLACK       0x0000      /*   0,   0,   0 */
#define TFT_NAVY        0x000F      /*   0,   0, 128 */
#define TFT_DARKGREEN   0x03E0      /*   0, 128,   0 */
#define TFT_DARKCYAN    0x03EF      /*   0, 128, 128 */
#define TFT_MAROON      0x7800      /* 128,   0,   0 */
#define TFT_PURPLE      0x780F      /* 128,   0, 128 */
#define TFT_OLIVE       0x7BE0      /* 128, 128,   0 */
#define TFT_LIGHTGREY   0xD69A      /* 211, 211, 211 */
#define TFT_DARKGREY    0x7BEF      /* 128, 128, 128 */
#define TFT_BLUE        0x001F      /*   0,   0, 255 */
#define TFT_GREEN       0x07E0      /*   0, 255,   0 */
#define TFT_CYAN        0x07FF      /*   0, 255, 255 */
#define TFT_RED         0xF800      /* 255,   0,   0 */
#define TFT_MAGENTA     0xF81F      /* 255,   0, 255 */
#define TFT_YELLOW      0xFFE0      /* 255, 255,   0 */
#define TFT_WHITE       0xFFFF      /* 255, 255, 255 */
#define TFT_ORANGE      0xFDA0      /* 255, 180,   0 */
#define TFT_GREENYELLOW 0xB7E0      /* 180, 255,   0 */
#define TFT_PINK        0xFE19      /* 255, 192, 203 */ //Lighter pink, was 0xFC9F      
#define TFT_BROWN       0x9A60      /* 150,  75,   0 */
#define TFT_GOLD        0xFEA0      /* 255, 215,   0 */
#define TFT_SILVER      0xC618      /* 192, 192, 192 */
#define TFT_SKYBLUE     0x867D      /* 135, 206, 235 */
#define TFT_VIOLET      0x915C      /* 180,  46, 226 */

unsigned int colour = TFT_SKYBLUE; // Font Color

/*---------------------------------------Ps3Controller-----------------------------------------*/
#include <Ps3Controller.h>

//#define JOYSTICKMAC "38:4f:f0:89:c0:c4"
 #define JOYSTICKMAC "60:f4:94:8f:46:c5"

int battery = 0;

void notify()
{
  //--- Digital cross/square/triangle/circle button events ---
  if ( Ps3.event.button_down.cross )
    Serial.println("Started pressing the cross button");
  if ( Ps3.event.button_up.cross )
    Serial.println("Released the cross button");

  if ( Ps3.event.button_down.square )
    Serial.println("Started pressing the square button");
  if ( Ps3.event.button_up.square )
    Serial.println("Released the square button");

  if ( Ps3.event.button_down.triangle )
    Serial.println("Started pressing the triangle button");
  if ( Ps3.event.button_up.triangle )
    Serial.println("Released the triangle button");

  if ( Ps3.event.button_down.circle )
    Serial.println("Started pressing the circle button");
  if ( Ps3.event.button_up.circle )
    Serial.println("Released the circle button");

  //--------------- Digital D-pad button events --------------
  if ( Ps3.event.button_down.up )
    Serial.println("Started pressing the up button");
  if ( Ps3.event.button_up.up )
    Serial.println("Released the up button");

  if ( Ps3.event.button_down.right )
    Serial.println("Started pressing the right button");
  if ( Ps3.event.button_up.right )
    Serial.println("Released the right button");

  if ( Ps3.event.button_down.down )
    Serial.println("Started pressing the down button");
  if ( Ps3.event.button_up.down )
    Serial.println("Released the down button");

  if ( Ps3.event.button_down.left )
    Serial.println("Started pressing the left button");
  if ( Ps3.event.button_up.left )
    Serial.println("Released the left button");

  //------------- Digital shoulder button events -------------
  if ( Ps3.event.button_down.l1 )
  {
    Serial.println("Started pressing the left shoulder button");
    LBW(4096);
  }
  if ( Ps3.event.button_up.l1 )
  {
    Serial.println("Released the left shoulder button");
    SW();
  }

  if ( Ps3.event.button_down.r1 )
  {
    Serial.println("Started pressing the right shoulder button");
    RBW(4096);
  }

  if ( Ps3.event.button_up.r1 )
  {
    Serial.println("Released the right shoulder button");
    SW();
  }


  //-------------- Digital trigger button events -------------
  if ( Ps3.event.button_down.l2 )
  {
    Serial.println("Started pressing the left trigger button");
    LFW(4096);
  }

  if ( Ps3.event.button_up.l2 )
  {
    Serial.println("Released the left trigger button");
    SW();
  }

  if ( Ps3.event.button_down.r2 )
  {
    Serial.println("Started pressing the right trigger button");
    RFW(4096);
  }
  if ( Ps3.event.button_up.r2 )
  {
    Serial.println("Released the right trigger button");
    SW();
  }

  //--------------- Digital stick button events --------------
  if ( Ps3.event.button_down.l3 )
  {
    Serial.println("Started pressing the left stick button");
    FWlight(255);
  }
  if ( Ps3.event.button_up.l3 )
  {
    Serial.println("Released the left stick button");
    LightAllOff();
  }

  if ( Ps3.event.button_down.r3 )
  {
    Serial.println("Started pressing the right stick button");
    EasyBuzzer.beep(370, 500, 1, 1, 1, 1); // frequency, onDuration,offDuration,beeps,pauseDuration,cycles
  }

  if ( Ps3.event.button_up.r3 )
  {
    Serial.println("Released the right stick button");
    EasyBuzzer.stopBeep();
  }


  //---------- Digital select/start/ps button events ---------
  if ( Ps3.event.button_down.select )
  {
    Serial.println("Started pressing the select button");

  }
  if ( Ps3.event.button_up.select )
  {
    Serial.println("Released the select button");

  }


  if ( Ps3.event.button_down.start )
  {
    Serial.println("Started pressing the start button");
  }
  if ( Ps3.event.button_up.start )
  {
    Serial.println("Released the start button");
  }
  if ( Ps3.event.button_down.ps )
    Serial.println("Started pressing the Playstation button");
  if ( Ps3.event.button_up.ps )
    Serial.println("Released the Playstation button");



#define JoystickTH 5
  //---------------- Analog stick value events ---------------
  if ( abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2 )
  {
    Serial.print("Moved the left stick:");
    Serial.print(" x="); Serial.print(Ps3.data.analog.stick.lx, DEC);
    Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ly, DEC);
    Serial.println();

    // Left joystick
    if ((abs(Ps3.data.analog.stick.ly) < JoystickTH) && (abs(Ps3.data.analog.stick.lx) < JoystickTH))
    {
      SW();
      // Serial.println("SW-L");
    }
    else if (abs(Ps3.data.analog.stick.ly) > abs(Ps3.data.analog.stick.lx))
    {
      if (Ps3.data.analog.stick.ly > JoystickTH)
      {
        BW(Ps3.data.analog.stick.ly * 32);
        // Serial.print(Ps3.data.analog.stick.ly); Serial.println(", FW");
      }
      else if (Ps3.data.analog.stick.ly < (JoystickTH * -1))
      {
        FW(Ps3.data.analog.stick.ly * 32);
        // Serial.print(Ps3.data.analog.stick.ly); Serial.println(", BW");
      }
    }
    else
    {
      if (Ps3.data.analog.stick.lx > JoystickTH)
      {
        SRW(Ps3.data.analog.stick.lx * 32);
        // Serial.print(Ps3.data.analog.stick.lx); Serial.println(", SRW");
      }
      else if (Ps3.data.analog.stick.lx < (JoystickTH * -1))
      {
        SLW(Ps3.data.analog.stick.lx * 32);
        // Serial.print(Ps3.data.analog.stick.lx); Serial.println(", SLW");
      }
    }
  }

  if ( abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2 )
  {
    Serial.print("Moved the right stick:");
    Serial.print(" x="); Serial.print(Ps3.data.analog.stick.rx, DEC);
    Serial.print(" y="); Serial.print(Ps3.data.analog.stick.ry, DEC);
    Serial.println();

    // Right joystick
    if ((abs(Ps3.data.analog.stick.ry) < JoystickTH) && (abs(Ps3.data.analog.stick.rx) < JoystickTH))
    {
      SW(); // must not used
      // Serial.println("SW-R");
    }
    else if (abs(Ps3.data.analog.stick.ry) > abs(Ps3.data.analog.stick.rx))
    {
      if (Ps3.data.analog.stick.ry > JoystickTH)
      {
        BWlight(abs(Ps3.data.analog.stick.ry * 2));
      }
      else if (Ps3.data.analog.stick.ry < (JoystickTH * -1))
      {
        FWlight(255);
      }
    }
    else
    {
      if (Ps3.data.analog.stick.rx > JoystickTH)
      {
        RW(Ps3.data.analog.stick.rx * 32);
        //      Serial.print(RDX); Serial.println(", RW");
      }
      else if (Ps3.data.analog.stick.rx < (JoystickTH * -1))
      {
        LW(Ps3.data.analog.stick.rx * 32);
        //      Serial.print(RDX); Serial.println(", LW");
      }
    }
  }

  //--------------- Analog D-pad button events ----------------
  if ( abs(Ps3.event.analog_changed.button.up) ) {
    Serial.print("Pressing the up button: ");
    Serial.println(Ps3.data.analog.button.up, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.right) ) {
    Serial.print("Pressing the right button: ");
    Serial.println(Ps3.data.analog.button.right, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.down) ) {
    Serial.print("Pressing the down button: ");
    Serial.println(Ps3.data.analog.button.down, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.left) ) {
    Serial.print("Pressing the left button: ");
    Serial.println(Ps3.data.analog.button.left, DEC);
  }

  //---------- Analog shoulder/trigger button events ----------
  if ( abs(Ps3.event.analog_changed.button.l1))
  {
    Serial.print("Pressing the left shoulder button: ");
    Serial.println(Ps3.data.analog.button.l1, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.r1) )
  {
    Serial.print("Pressing the right shoulder button: ");
    Serial.println(Ps3.data.analog.button.r1, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.l2) )
  {
    Serial.print("Pressing the left trigger button: ");
    Serial.println(Ps3.data.analog.button.l2, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.r2) )
  {
    Serial.print("Pressing the right trigger button: ");
    Serial.println(Ps3.data.analog.button.r2, DEC);
  }

  //---- Analog cross/square/triangle/circle button events ----
  if ( abs(Ps3.event.analog_changed.button.triangle))
  {
    Serial.print("Pressing the triangle button: ");
    Serial.println(Ps3.data.analog.button.triangle, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.circle) )
  {
    Serial.print("Pressing the circle button: ");
    Serial.println(Ps3.data.analog.button.circle, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.cross) )
  {
    Serial.print("Pressing the cross button: ");
    Serial.println(Ps3.data.analog.button.cross, DEC);
  }

  if ( abs(Ps3.event.analog_changed.button.square) )
  {
    Serial.print("Pressing the square button: ");
    Serial.println(Ps3.data.analog.button.square, DEC);
  }

  //---------------------- Battery events ---------------------
  //  if ( battery != Ps3.data.status.battery )
  //  {
  //    battery = Ps3.data.status.battery;
  //    Serial.print("The controller battery is ");
  //    if ( battery == ps3_status_battery_charging )      Serial.println("charging");
  //    else if ( battery == ps3_status_battery_full )     Serial.println("FULL");
  //    else if ( battery == ps3_status_battery_high )     Serial.println("HIGH");
  //    else if ( battery == ps3_status_battery_low)       Serial.println("LOW");
  //    else if ( battery == ps3_status_battery_dying )    Serial.println("DYING");
  //    else if ( battery == ps3_status_battery_shutdown ) Serial.println("SHUTDOWN");
  //    else Serial.println("UNDEFINED");
  //  }
}

void onConnect()
{
  for (int j = 0; j < 2; j++)
  {
    for (uint16_t i = 6; i < 9; i++)
    {
      leds[i] = CRGB(0, 250, 154);
    }
    FastLED.show();
    delay(125);
    FastLED.clear();
    FastLED.show();
    delay(125);
  }
  //
  for (int i = 0; i < 1; i++)
  {
    EasyBuzzer.beep(392, 1); // Freq, times
    delay(125);
    EasyBuzzer.beep(523.3, 1); // Freq, times
    delay(250);
    EasyBuzzer.beep(329.6, 1); // Freq, times
    delay(125);
    EasyBuzzer.beep(523.3, 1); // Freq, times
    delay(125);
    EasyBuzzer.beep(880.0, 1); // Freq, times
    delay(375);
    EasyBuzzer.beep(440, 1); // Freq, times
    delay(125);
    EasyBuzzer.beep(784, 1); // Freq, times
    delay(750);
    EasyBuzzer.stopBeep();
  }
  //
  Ps3.setRumble(100.0); // Turn rumble on full intensity indefinitely
  delay(1000);
  Ps3.setRumble(0.0); // Turn off rumble
  delay(500);
  //
  tft.println("... Connected !!");
  tft.println("");
  tft.print(" JoyStick MAc: ");
  tft.println(JOYSTICKMAC);
  //
  Serial.println("Connected.");
}

void setup()
{
  Serial.begin(115200);
  //
  // setting PWM properties
#define freq 5000
#define ledChannel 0
#define resolution 8
  int TFT_BLBrightness = 200;
  //
  ledcSetup(ledChannel, freq, resolution); //宣告ledChannel特性
  ledcAttachPin(TFT_BL, ledChannel); //將上述ledChannel附加到ledPin腳位上
  ledcWrite(ledChannel, TFT_BLBrightness);

  tft.init();
  tft.setRotation(RotationDisplay);

  // 這裡擺LOGO
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true); // pushImage is used for 16-bit sprites, the effect of swap bytes is reversed
  tft.pushImage(0, 0, 320, 240, SeaOtter);
  delay(2000);
  //
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 5, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.print("System Ini...");
  //
  //
  pwm0.begin();
  pwm0.setPWMFreq(L9110sFreq);  // This is the maximum PWM frequency
  //  pwm1.begin();
  //  pwm1.setPWMFreq(pwm1Freq);  // This is the maximum PWM frequency
  //  pwm1.begin();
  //  pwm1.setPWMFreq(pwm2Freq);  // This is the maximum PWM frequency

  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode some i2c devices dont like this so much so if you're sharing the bus, watch out for this!
#ifdef TWBR
  // save I2C bitrate
  uint8_t twbrbackup = TWBR;
  // must be changed after calling Wire.begin() (inside pwm.begin())
  TWBR = 12; // upgrade to 400KHz!
#endif

  //
  SW(); // stop all of motors
  //

  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  Ps3.begin(JOYSTICKMAC);

  FastLED.addLeds<ChipSet, DataIn, ColorOrder>(leds, LedNum);
  FastLED.setBrightness(Brightness);
  FastLED.clear();
  FastLED.show();

  int RainBowLed[11][3] =
  {
    {220, 20, 60},
    {220, 20, 60},
    {255, 140, 0},
    {255, 140, 0},
    {255, 255, 0},
    {255, 255, 0},
    {34, 139, 34},
    {64, 224, 208},
    {25, 25, 112},
    {138, 43, 226},
    {138, 43, 226},
  };

  for (uint16_t i = 0; i < LedNum; i++)
  {
    leds[i] = CRGB(RainBowLed[i][0], RainBowLed[i][1], RainBowLed[i][2]);
    delay(250);
    FastLED.show();
  }
  delay(1000);

  FastLED.clear();
  FastLED.show();

  EasyBuzzer.setPin(BuzzerPin);

  Serial.println("Ready.");
}

void loop()
{
  EasyBuzzer.update(); /* Always call this function in the loop for EasyBuzzer to work. */

  //  pwm0.setPWM(6, 4095, 0); // M1B
  //  pwm0.setPWM(7, 0, 4095); // M1A

  if (!Ps3.isConnected())
  {
    for (int j = 0; j < 2; j++)
    {
      for (uint16_t i = 6; i < 9; i++)
      {
        leds[i] = CRGB(220, 20, 60);
      }
      FastLED.show();
      delay(100);
      FastLED.clear();
      FastLED.show();
      delay(100);
    }
  }
  return;

  //------ Digital cross/square/triangle/circle buttons ------
  //  if ( Ps3.data.button.cross && Ps3.data.button.down )
  //    Serial.println("Pressing both the down and cross buttons");
  //  if ( Ps3.data.button.square && Ps3.data.button.left )
  //    Serial.println("Pressing both the square and left buttons");
  //  if ( Ps3.data.button.triangle && Ps3.data.button.up )
  //    Serial.println("Pressing both the triangle and up buttons");
  //  if ( Ps3.data.button.circle && Ps3.data.button.right )
  //    Serial.println("Pressing both the circle and right buttons");
  //
  //  if ( Ps3.data.button.l1 && Ps3.data.button.r1 )
  //    Serial.println("Pressing both the left and right bumper buttons");
  //  if ( Ps3.data.button.l2 && Ps3.data.button.r2 )
  //    Serial.println("Pressing both the left and right trigger buttons");
  //  if ( Ps3.data.button.l3 && Ps3.data.button.r3 )
  //    Serial.println("Pressing both the left and right stick buttons");
  //  if ( Ps3.data.button.select && Ps3.data.button.start )
  //    Serial.println("Pressing both the select and start buttons");
}

void FW(int SPEED)
{
  // 前
  pwm0.setPWM(0, abs(SPEED), 4095 - abs(SPEED)); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, abs(SPEED), 4095 - abs(SPEED)); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, abs(SPEED), 4095 - abs(SPEED)); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, abs(SPEED), 4095 - abs(SPEED)); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A
  //  Serial.println("FW");
  delay(5);
}

void BW(int SPEED)
{
  // 後
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, abs(SPEED), 4095 - abs(SPEED)); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, abs(SPEED), 4095 - abs(SPEED)); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, abs(SPEED), 4095 - abs(SPEED)); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, abs(SPEED), 4095 - abs(SPEED)); // M1A
  //  Serial.println("BW");
  BWlight(abs(SPEED / 16));
  delay(5);
}

void LW(int SPEED)
{
  // 左旋
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, abs(SPEED), 4095 - abs(SPEED)); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, abs(SPEED), 4095 - abs(SPEED)); // M4B
  pwm0.setPWM(4, abs(SPEED), 4095 - abs(SPEED)); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, abs(SPEED), 4095 - abs(SPEED)); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A

  delay(5);
}

void RW(int SPEED)
{
  // 右旋
  pwm0.setPWM(0, abs(SPEED), 4095 - abs(SPEED)); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, abs(SPEED), 4095 - abs(SPEED)); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, abs(SPEED), 4095 - abs(SPEED)); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, abs(SPEED), 4095 - abs(SPEED)); // M1A

  delay(5);
}

void SRW(int SPEED)
{
  // 螃蟹右
  pwm0.setPWM(0, abs(SPEED), 4095 - abs(SPEED)); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, abs(SPEED), 4095 - abs(SPEED)); // M4B
  pwm0.setPWM(4, abs(SPEED), 4095 - abs(SPEED)); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, abs(SPEED), 4095 - abs(SPEED)); // M1A

  delay(5);
}

void SLW(int SPEED)
{
  // 螃蟹左
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, abs(SPEED), 4095 - abs(SPEED)); // M2A
  pwm0.setPWM(2, abs(SPEED), 4095 - abs(SPEED)); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, abs(SPEED), 4095 - abs(SPEED)); // M3B
  pwm0.setPWM(6, abs(SPEED), 4095 - abs(SPEED)); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A

  delay(5);
}

void LFW(int SPEED)
{
  // 螃蟹斜左前
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, abs(SPEED), 4095 - abs(SPEED)); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, abs(SPEED), 4095 - abs(SPEED)); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A

  delay(5);
}

void LBW(int SPEED)
{
  // 螃蟹斜左後
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, abs(SPEED), 4095 - abs(SPEED)); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, abs(SPEED), 4095 - abs(SPEED)); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A

  delay(5);
}

void RFW(int SPEED)
{
  // 螃蟹斜右前
  pwm0.setPWM(0, abs(SPEED), 4095 - abs(SPEED)); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, abs(SPEED), 4095 - abs(SPEED)); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A

  delay(5);
}

void RBW(int SPEED)
{
  // 螃蟹斜右後
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, abs(SPEED), 4095 - abs(SPEED)); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, abs(SPEED), 4095 - abs(SPEED)); // M1A

  delay(5);
}

void SW()
{
  // 停
  pwm0.setPWM(0, 0, 4095); // M2B
  pwm0.setPWM(1, 0, 4095); // M2A
  pwm0.setPWM(2, 0, 4095); // M4A
  pwm0.setPWM(3, 0, 4095); // M4B
  pwm0.setPWM(4, 0, 4095); // M3A
  pwm0.setPWM(5, 0, 4095); // M3B
  pwm0.setPWM(6, 0, 4095); // M1B
  pwm0.setPWM(7, 0, 4095); // M1A
  LightAllOff();

  delay(5);
  Serial.println("STOP");
}

void FWlight(int SPEED)
{
  FastLED.setBrightness(SPEED);
  leds[0] = CRGB(64, 224, 208);
  leds[1] = CRGB(64, 224, 208);
  leds[9] = CRGB(64, 224, 208);
  leds[10] = CRGB(64, 224, 208);
  FastLED.show();
}

void BWlight(int SPEED)
{
  FastLED.setBrightness(SPEED);
  leds[2] = CRGB(220, 20, 60);
  leds[3] = CRGB(220, 20, 60);
  leds[4] = CRGB(220, 20, 60);
  leds[5] = CRGB(220, 20, 60);
  FastLED.show();
}

void LightAllOff()
{
  FastLED.clear();
  FastLED.show();
}
