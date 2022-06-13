/*
    Video: https://www.youtube.com/watch?v=oCMOYS71NIU
    Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
    Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE"
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function).
   And txValue is the data to be sent, in this example just a byte incremented every second.
*/

// https://www.youtube.com/watch?v=v0fvBbUQu9o

#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t txValue = 0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

int flagShowControl = 0;

class MyServerCallbacks: public BLEServerCallbacks
{
    void onConnect(BLEServer* pServer)
    {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer)
    {
      deviceConnected = false;
    }
};

// https://forum.arduino.cc/t/how-to-convert-char-array-to-byte/897362  How to convert char array to byte?
// http://tw.gitbook.net/c_standard_library/c_function_strtoul.html  strtoul() - C語言庫函數
int hexConvert2byteint(char highByte, char lowByte)
{
  String data;
  data += "0x";
  data += highByte;
  data += lowByte;
  char Buf[5];
  data.toCharArray(Buf, 5);
  //  Serial.println(strtoul(Buf, NULL, 16));
  return strtoul(Buf, NULL, 16);
}

int datafromV7RC[8] = {150, 150, 150, 150, 150, 150, 150, 150}; // V7RC Code

class MyCallbacks: public BLECharacteristicCallbacks
{
    void onWrite(BLECharacteristic *pCharacteristic)
    {
      std::string rxValue = pCharacteristic->getValue();
      String rxData;
      String data;

      if (rxValue.length() > 0)
      {
        Serial.println("*********");
        Serial.print("Received Value: ");
        for (int i = 0; i < rxValue.length(); i++)
        {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }


      // V7RC Code do Decode Here !! for SRT / SRV  CMD  (4 Servo)---------------------------------------------------------------- >>>
      if (rxValue[1] == 'R')
      {
        for (int i = 0; i < 4; i++)
        {
          data = rxData.substring(i * 4 + 3, i * 4 + 7);
          datafromV7RC[i] = data.toInt();
        }
      }
      else if (rxValue[2] == '8') // 專家模式 for SS8   CMD  (8 Servo)   //SS8 96 96 96 96 96 96 96 96#
      {
        for (int i = 0; i < 8; i++)
        {
          datafromV7RC[i] = hexConvert2byteint(rxValue[2 * i + 3], rxValue[2 * i + 4] ) ;
        }
      }
      else
      {
        SW();
      }

      //      ////debug Only, send to Vrep....
      //      if (flagShowControl == 1)
      //      {
      //        Serial.print(rxValue[2]);  /// should be V / T / 8 (2 ch, 4 ch , 8 ch )
      //        Serial.print(",");
      //
      //        for (int i = 0; i < 8; i++)
      //        {
      //          Serial.print(datafromV7RC[i]);
      //          Serial.print(",");
      //        }
      //        Serial.println(",");
      //      }
      //      else if (flagShowControl == 2)
      //      {
      //        dumpespLoraData();
      //      }
      //      else // flagShowControl == 0
      //      {
      //        // do nothing
      //      }
    }
};

void parseCommand()
{
  char cmd = Serial.read();
  switch (cmd)
  {
    case 'D':
      flagShowControl = 2;
      break;

    case '1':
      flagShowControl = 1;
      break;

    case '0':
      flagShowControl = 0;
      break;
  }
}

void dumpespLoraData()
{
  for (int i = 0; i < 8; i++)
  {
    Serial.print(map(datafromV7RC[i], 100, 200, -4095, 4095));
    Serial.print(",");
  }
  Serial.println("");
}
//
int ConnectingState = 0;

/*---------------------------------------WS2812b-----------------------------------------*/
// https://flaviocopes.com/rgb-color-codes/
#include <FastLED.h>
#define LedNum 11
#define DataIn 15
#define ColorOrder GRB
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
#define L9110sFreq 1000 // 1600 is the maximum PWM frequency
#define pwm1Freq 1000
#define pwm2Freq 1000

/*---------------------------------------Buzzer-----------------------------------------*/
// https://blog.csdn.net/weixin_40973138/article/details/103872327
// https://thecustomizewindows.com/2020/05/esp32-arduino-smartwatch-detector-with-buzzer/
// https://www.apronus.com/music/lessons/unit01.htm
// https://www.apronus.com/music/flashpiano.htm
// https://zh.wikipedia.org/wiki/%E9%8B%BC%E7%90%B4%E9%8D%B5%E9%A0%BB%E7%8E%87
// https://www.backupmp3.com/zh/
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


void setup()
{
  Serial.begin(115200);
  //

  pwm0.begin();
  pwm0.setPWMFreq(L9110sFreq);  // This is the maximum PWM frequency
  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode some i2c devices dont like this so much so if you're sharing the bus, watch out for this!
  Wire.setClock(400000);
  //  pwm1.begin();
  //  pwm1.setPWMFreq(pwm1Freq);  // This is the maximum PWM frequency
  //  pwm1.begin();
  //  pwm1.setPWMFreq(pwm2Freq);  // This is the maximum PWM frequency
  //
  SW(); // stop all of motors
  delay(250);
  //
  // setting PWM properties
#define freq 5000
#define ledChannel 0
#define resolution 8
  int TFT_BLBrightness = 100;
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
  // Create the BLE Device
  BLEDevice::init("LouisGuanCar");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_TX, BLECharacteristic::PROPERTY_NOTIFY);
  pTxCharacteristic->addDescriptor(new BLE2902());
  BLECharacteristic * pRxCharacteristic = pService->createCharacteristic(CHARACTERISTIC_UUID_RX, BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  //  pServer->getAdvertising()->start();
  //  Serial.println("BLE Waiting a client connection to notify...");
  // Start advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();

  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinPreferred(0x06);
  pAdvertising->setMinPreferred(0x12);

  pAdvertising->start();
#ifdef debug
  Serial.println("Start to read!!");
#endif
  //
  tft.println("BLE Waiting a client connection to notify...");
  //
  ConnectingState = 0;
  //
  //
  //


  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode some i2c devices dont like this so much so if you're sharing the bus, watch out for this!
  //#ifdef TWBR
  //  // save I2C bitrate
  //  uint8_t twbrbackup = TWBR;
  //  // must be changed after calling Wire.begin() (inside pwm.begin())
  //  TWBR = 12; // upgrade to 400KHz!
  //#endif

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

  Serial.println("CAR Ready.");

  pwm0.begin();
  pwm0.setPWMFreq(L9110sFreq);  // This is the maximum PWM frequency
  // if you want to really speed stuff up, you can go into 'fast 400khz I2C' mode some i2c devices dont like this so much so if you're sharing the bus, watch out for this!
  Wire.setClock(400000);
  SW(); // stop all of motors
  delay(250);
}

void loop()
{
  RcRemoteControl();
  //

  //  if (Serial.available())
  //    parseCommand();

  if (deviceConnected)
  {
    ConnectingState = 1;
    //
    pTxCharacteristic->setValue(&txValue, 1);
    pTxCharacteristic->notify();
    txValue++;
    delay(10); // bluetooth stack will go into congestion, if too many packets are sent
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected)
  {
    ConnectingState = 0;
    //
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
    //
    tft.println("Disconnecting...Start advertising");
    //
    for (int i = 0; i < 1; i++)
    {
      EasyBuzzer.beep(622.3, 1); // Freq, times
      delay(375);
      EasyBuzzer.beep(587.3, 1); // Freq, times
      delay(250);
      EasyBuzzer.beep(554.4, 1); // Freq, times
      delay(500);
      EasyBuzzer.beep(523.3, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(493.9, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(523.3, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(493.9, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(523.3, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(493.9, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(523.3, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(493.9, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(523.3, 1); // Freq, times
      delay(125);
      EasyBuzzer.beep(493.9, 1); // Freq, times
      delay(625);
      EasyBuzzer.stopBeep();
    }
  }

  // connecting
  if (deviceConnected && !oldDeviceConnected)
  {
    ConnectingState = 1;
    //
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
    //
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
    tft.println("... Connected !!");
    //
    Serial.println("Connected.");
  }

  if (ConnectingState == 0)
  {
    //
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
  else
  {
    // do nothing
  }
}

void RcRemoteControl()
{
#define JoystickTH 100
  //
  int LDX = map(datafromV7RC[0], 100, 200, -4095, 4095); // CH1: 左下 左右X
  int LDY = map(datafromV7RC[1], 100, 200, -4095, 4095); // CH2: 左下 上下Y
  int RDY = map(datafromV7RC[2], 100, 200, -4095, 4095); // CH3: 右下 上下Y
  int RDX = map(datafromV7RC[3], 100, 200, -4095, 4095); // CH4: 右下 左右X

  int LUX = map(datafromV7RC[4], 100, 200, -4095, 4095); // CH5: 左上 左右X
  int LUY = map(datafromV7RC[5], 100, 200, -4095, 4095); // CH6: 左上 上下Y
  int RUY = map(datafromV7RC[6], 100, 200, -4095, 4095); // CH7: 右上 上下Y
  int RUX = map(datafromV7RC[7], 100, 200, -4095, 4095); // CH8: 右上 左右X

  // Left joystick
  if ((abs(LDY) < JoystickTH) && (abs(LDX) < JoystickTH))
  {
    SW();
    // Serial.println("SW-L");
  }
  else if (abs(LDY) > abs(LDX))
  {
    if (LDY > JoystickTH)
    {
      FW(LDY);
      // Serial.print(LDY); Serial.println(", FW");
    }
    else if (LDY < (JoystickTH * -1))
    {
      BW(LDY);
      // Serial.print(LDY); Serial.println(", BW");
    }
  }
  else
  {
    if (LDX > JoystickTH)
    {
      SRW(LDX);
      // Serial.print(LDX); Serial.println(", SRW");
    }
    else if (LDX < (JoystickTH * -1))
    {
      SLW(LDX);
      // Serial.print(LDX); Serial.println(", SLW");
    }
  }

  // Right joystick
  if ((abs(RDY) < JoystickTH) && (abs(RDX) < JoystickTH))
  {
    // SW(); // must not used
    // Serial.println("SW-R");
  }
  else if (abs(RDY) > abs(RDX))
  {
    if (RDY > JoystickTH)
    {
      FWlight();
    }
    else if (RDY < (JoystickTH * -1))
    {
      BWlight();
    }
  }
  else
  {
    if (RDX > JoystickTH)
    {
      RW(RDX);
      //      Serial.print(RDX); Serial.println(", RW");
    }
    else if (RDX < (JoystickTH * -1))
    {
      LW(RDX);
      //      Serial.print(RDX); Serial.println(", LW");
    }
  }
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

  BWlight();
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
}

void FWlight()
{
  leds[0] = CRGB(64, 224, 208);
  leds[1] = CRGB(64, 224, 208);
  leds[9] = CRGB(64, 224, 208);
  leds[10] = CRGB(64, 224, 208);
  FastLED.show();
}

void BWlight()
{
  leds[2] = CRGB(220, 20, 60);
  leds[3] = CRGB(220, 20, 60);
  leds[4] = CRGB(220, 20, 60);
  leds[5] = CRGB(220, 20, 60);
  FastLED.show();
}

void LightAllOff()
{
  FastLED.clear();
  delay(5);
  FastLED.show();
}
