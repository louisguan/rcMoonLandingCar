// https://openweathermap.org/widgets-constructor
// http://opendata2.epa.gov.tw/AQI.json
// https://blog.kylemanna.com/hardware/sniffer-air-quality-monitor-aqi-using-esp32-pmsa003-bme680/
// https://www.yiboard.com/thread-1561-1-1.html
// https://tw.piliapp.com/aqi/tw/
// https://airtw.epa.gov.tw/cht/Information/Standard/AirQualityIndicator.aspx
/*---------------------------------------ESP32 WIFI-----------------------------------------*/
#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
WiFiClient espClient;
//
String DeviceName = "";
/*---------------------------------------WiFiManager-----------------------------------------*/
#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager

WiFiManager wm;

/*---------------------------------------MQTT-----------------------------------------*/
#include "PubSubClient.h"
const char* mqttServer = "broker.emqx.io"; // mqtt server address
const int mqttPort = 1883; // mqtt port
const char* mqttUser = ""; // your mqtt user
const char* mqttPassword = ""; // your mqtt password
//
String clientId = "ESP32Client-"; // Create a random client ID
// #include <WiFi.h>
PubSubClient client(espClient);
//
#define MQTT_PUB_HandShake        "ESP32/ACS/Device001/Init"
#define MQTT_SUB_ACK              "ESP32/ACS/Device001/Ack"

#define MQTT_SUB_ACT_STATE        "ESP32/ACS/DeviceABC/STATE"
#define MQTT_PUB_PMS5003T_STATE   "ESP32/ACS/DeviceABC/PMS5003T"

/*---------------------------------------WS2812b-----------------------------------------*/
#include <FastLED.h>
#define LedNum 1
#define DataIn 15
#define ColorOrder GRB
#define ChipSet WS2812B
int Brightness = 50;
CRGB leds[LedNum];
/*---------------------------------------ILI9341_DRIVER 320x240 2.2' TFT-----------------------------------------*/
// https://blog.csdn.net/weixin_45488643/article/details/113702365

#include <TFT_eSPI.h> // Hardware-specific library
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library

#define TFT_BL    25
int RotationDisplay = 1;

// http://www.rinkydinkelectronics.com/t_imageconverter565.php
#include "TainanLogo.h"

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

/*---------------------------------------PMS5003T-----------------------------------------*/
#include "PMS.h"
//
PMS pms(Serial2);
PMS::DATA data;

/*---------------------------------------ArduinoJson-----------------------------------------*/
// https://www.itread01.com/content/1550251984.html
#include <ArduinoJson.h> // ArduinoJson library v5.9.0
//
// Standard Particles, CF=1
String A = ""; // PM_SP_UG_1_0
String B = ""; // PM_SP_UG_2_5
String C = ""; // PM_SP_UG_10_0
// Atmospheric environment
String D = ""; // PM_AE_UG_1_0
String E = ""; // PM_AE_UG_2_5
String F = ""; // PM_AE_UG_10_0
// Particles Number
String G = ""; // PM_PN_UG_0_3
String H = ""; // PM_PN_UG_0_5
String I = ""; // PM_PN_UG_1_0
String J = ""; // PM_PN_UG_2_5
// Tmperature & Humidity
String K = ""; // Tmperature
String L = ""; // Humidity

/*---------------------------------------Other-----------------------------------------*/
long lastMsg = 0;
//
#define TRIGGER_PIN 0

#define DevicePSWD "0989839679"

/*---------------------------------------MQTT CallBack-----------------------------------------*/
int r, g, b;
//
void callback(char* topic, byte* message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off".
  // Changes the output state according to the message
  if (String(topic) == MQTT_SUB_ACT_STATE)
  {
    Serial.println("XD ");
    if (messageTemp == "PMS5003T/ON")
    {
      Serial.println("WakeUp");
      pms.wakeUp();
    }
    else if (messageTemp == "PMS5003T/OFF")
    {
      Serial.println("Sleep");
      pms.sleep();
    }
    else if ((messageTemp.length() == 7)) // search "rgb("
    {
      // if (messageTemp.length() != 7) {
      // Serial.printf("invalid messageTemp: '%s'\n", messageTemp.c_str());
      // return;
      // }
      long rgb = strtol(messageTemp.c_str() + 1, 0, 16); // parse as Hex, skipping the leading '#'
      r = (rgb >> 16) & 0xFF;
      g = (rgb >> 8) & 0xFF;
      b = rgb & 0xFF;
      //      Serial.printf("r=%d, g=%d, b=%d\n", r, g, b);
    }
  }
}

void APCallback (WiFiManager *ptWM)
{
  Serial.print("啟動AP模式，AP的SSID：");
  Serial.println(ptWM->getConfigPortalSSID());
  Serial.print("IP位址：");
  Serial.println(WiFi.softAPIP());
}

String getParam(String name)
{
  //read parameter from server, for customhmtl input
  String value;
  if (wm.server->hasArg(name))
  {
    value = wm.server->arg(name);
  }
  return value;
}

void saveParamCallback()
{
  Serial.println("[CALLBACK] saveParamCallback fired");
  Serial.println("PARAM customfieldid = " + getParam("customfieldid"));
}

void setup()
{
  Serial.begin(115200);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)
  Serial.setDebugOutput(true); // 默認情況下，當調用Serial.begin後，將禁用WiFi庫的診斷輸出。要想再次啓動調試輸出，請調用
  Serial2.begin(9600);  // GPIO2 (D4 pin on ESP-12E Development Board)
  pms.passiveMode();    // Switch to passive mode

  Serial.println("Waking up, wait 30 seconds for stable readings...");
  pms.wakeUp();


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
  tft.pushImage(3, 2, 128, 128, Tainan);
  delay(2000);
  //
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(5, 5, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.println("System Ini...");
  //
  tft.setCursor(5, 25, 2);
  tft.print("Device ID: ");
  tft.println(String(WIFI_getChipId(), HEX).c_str());
  //
  tft.setTextFont(1);
  tft.print("- WIFI connecting...Please Wait!!");
  //  tft.print(ssid);
  WIFIreconnect();
  tft.println(" V"); delay(250);
  tft.print("  LocIP: "); tft.println(WiFi.localIP());
  delay(500);
  //
  tft.print("- WS2812b.......");
  FastLED.addLeds<ChipSet, DataIn, ColorOrder>(leds, LedNum);
  FastLED.setBrightness(Brightness);
  FastLED.clear();
  FastLED.show();
  //  strip.begin();
  //  strip.setBrightness(Brightness);
  //  strip.show(); // Initialize all pixels to 'off'
  //  LEDAllState(0, 1);
  tft.println(" V"); delay(250);
  //
  // 以下順序很重要~錯了少了~都會沒作用歐~
  client.setBufferSize(512); // option or with need
  client.setServer(mqttServer, mqttPort); // need
  client.setCallback(callback); // need
  tft.print("- MQTT connect to ");
  tft.print(mqttServer);
  MQTTreconnect(); // need
  tft.println(" V"); delay(250);
  //
  tft.println("\r\n Has been Initialized ........ Make by XYG"); delay(3000);
  //
  client.subscribe(MQTT_SUB_ACT_STATE); // option or need with topic
  //
  // for digital clock
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(1);
  tft.setTextColor(colour, TFT_BLACK); // Set colour to grey to dim colon 0x39C4
}

void loop()
{
  wm.process();
  wm.startWebPortal();
  checkButton();
  //
  if (!client.connected())
  {
    MQTTreconnect();
  }
  client.loop();

  long now = millis();
  //  String IR_State = "";
  //  //
  if (now - lastMsg > 2000)
  {
    lastMsg = now;
    //
    PMS5003T();
    //
    PM25TFT();
    //
    // Json物件物件樹的記憶體工具 靜態buffer
    // 300 是大小 如果這個Json物件更加複雜，那麼就需要根據需要去增加這個值.
    StaticJsonBuffer <500> JSONbuffer;
    // StaticJsonBuffer 在棧區分配記憶體   它也可以被 DynamicJsonBuffer（記憶體在堆區分配） 代替
    // DynamicJsonBuffer  jsonBuffer;
    // 建立根，也就是頂節點
    JsonObject& ROOT = JSONbuffer.createObject();
    ROOT["Device"] = "ESP32";
    ROOT["Sensortype"] = "PMS5003T";
    // Tmperature & Humidity
    ROOT["Temperature"] = K;
    ROOT["Humidity"] = L;
    // Standard Particles, CF=1
    JsonArray& SP = ROOT.createNestedArray("Standard_Particles");
    SP.add(A);
    SP.add(B);
    SP.add(C);
    // Atmospheric environment
    JsonArray& AE = ROOT.createNestedArray("Atmospheric_Environment");
    AE.add(D);
    AE.add(E);
    AE.add(F);
    // Particles Number
    JsonArray& PN = ROOT.createNestedArray("Particles_Number");
    PN.add(G);
    PN.add(H);
    PN.add(I);
    PN.add(J);
    //
    ROOT.printTo(Serial);//單行列印
    // ROOT.prettyPrintTo(Serial);//多行列印
    //

    char JSONmessageBuffer[500];
    ROOT.printTo(JSONmessageBuffer, sizeof(JSONmessageBuffer));
    Serial.println("");
    Serial.println("Sending message to MQTT topic..");
    Serial.println(JSONmessageBuffer);

    if (client.publish(MQTT_PUB_PMS5003T_STATE, JSONmessageBuffer) == true)
    {
      Serial.println("Success sending message");
      LEDAllState(6, 100);
      LEDAllState(0, 1);
    }
    else
    {
      Serial.println("Error sending message");
      LEDAllState(3, 5);
    }
    //

    WIFIParameter();
  }
}

void WIFImanager()
{
  //  https://github.com/tzapu/WiFiManager/blob/master/examples/Advanced/Advanced.ino

  //  wm.resetSettings(); // wipe settings
  wm.setClass("invert"); // set dark theme
  wm.setConfigPortalBlocking(true);
  wm.setSaveConfigCallback(saveParamCallback);
  //
  //set static ip
  // wm.setSTAStaticIPConfig(IPAddress(10,0,1,99), IPAddress(10,0,1,1), IPAddress(255,255,255,0)); // set static ip,gw,sn
  wm.setShowStaticFields(true); // force show static ip fields
  wm.setShowDnsFields(true);    // force show dns field always

  wm.setConnectTimeout(20); // how long to try to connect for before continuing

  // wm.setCaptivePortalEnable(false); // disable captive portal redirection
  wm.setAPClientCheck(true); // avoid timeout if client connected to softap
  //
  Serial.println("Starting config portal"); // auto close configportal after n seconds
  bool res;
  //  res = wm.autoConnect(); // auto generated AP name from chipid
  //  res = wm.autoConnect(AP_SSID); // anonymous ap
  res = wm.autoConnect(String(WIFI_getChipId(), HEX).c_str(), DevicePSWD); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect or hit timeout");
    wm.setConfigPortalTimeout(120); // 120sec
  }
  else
  {
    Serial.println("connected...yeey :)"); // if you get here you have connected to the WiFi
  }
}

void WIFIreconnect()
{
  WiFi.mode(WIFI_STA);
  WIFImanager();
}

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~要記得把這塊轉到LCD上顯示歐~
void checkButton()
{
  // check for button press
  if ( digitalRead(TRIGGER_PIN) == LOW )
  {
    // poor mans debounce/press-hold, code not ideal for production
    delay(50);
    if ( digitalRead(TRIGGER_PIN) == LOW )
    {
      Serial.println("Button Pressed");
      // still holding button for 3000 ms, reset settings, code not ideaa for production
      LEDAllState(5, 500);
      LEDAllState(0, 500);
      LEDAllState(5, 500);
      LEDAllState(0, 500);
      LEDAllState(5, 500);
      LEDAllState(0, 500); // total 3 sec

      if ( digitalRead(TRIGGER_PIN) == LOW )
      {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        LEDAllState(2, 1500);
        LEDAllState(0, 0); // total 3 sec
        wm.resetSettings();
        ESP.restart();
      }

      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);

      if (!wm.startConfigPortal(String(WIFI_getChipId(), HEX).c_str(), DevicePSWD))
      {
        Serial.println("failed to connect or hit timeout");
        LEDAllState(3, 1500); // total 1.5 sec
        // ESP.restart();
      } else
      {
        Serial.println("connected...yeey :)"); // if you get here you have connected to the WiFi
      }
    }
  }
}

void MQTTreconnect()
{
  while (!client.connected())
  {
    Serial.println("Connecting to MQTT...");
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str(), mqttUser, mqttPassword))
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish(MQTT_PUB_HandShake, "I'm In~");
      // ... and resubscribe
      Serial.println(client.subscribe(MQTT_SUB_ACK));
      LEDAllState(2, 1000);
      LEDAllState(0, 1000);
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      tft.println(client.state());
      LEDAllState(1, 1250);
      LEDAllState(0, 1250);
      tft.fillScreen(TFT_BLACK);
      // delay(5000); // Wait 5 seconds before retrying
      /*
        client.state() Returns
        -4 : MQTT_CONNECTION_TIMEOUT - the server didn't respond within the keepalive time
        -3 : MQTT_CONNECTION_LOST - the network connection was broken
        -2 : MQTT_CONNECT_FAILED - the network connection failed
        -1 : MQTT_DISCONNECTED - the client is disconnected cleanly
        0 : MQTT_CONNECTED - the client is connected
        1 : MQTT_CONNECT_BAD_PROTOCOL - the server doesn't support the requested version of MQTT
        2 : MQTT_CONNECT_BAD_CLIENT_ID - the server rejected the client identifier
        3 : MQTT_CONNECT_UNAVAILABLE - the server was unable to accept the connection
        4 : MQTT_CONNECT_BAD_CREDENTIALS - the username/password were rejected
        5 : MQTT_CONNECT_UNAUTHORIZED - the client was not authorized to connect
      */
    }
  }
}

void WIFIParameter() // 讀取MAC & 讀取RSSI數值
{
  int Xpos = 0;
  int Ypos = 220;

  int rssi = WiFi.RSSI();
  //  Serial.print("RSSI: ");
  //  Serial.print(rssi);
  Xpos += tft.drawString("RSSI: ", Xpos, 220, 2);
  Xpos += tft.drawString(String(rssi), Xpos, 220, 2); // drawFloat(value, precision, x, y, font);
  //
  String Mac = WiFi.macAddress();
  DeviceName = Mac;
  //  Serial.print(", Mac:");
  //  Serial.println(Mac);
  Xpos += tft.drawString("   XYG:", Xpos, 220, 2);
  Xpos += tft.drawString(Mac, Xpos, 220, 2);
  Xpos += tft.drawString(":FF", Xpos, 220, 2);
}

// https://www.rapidtables.com/web/color/RGB_Color.html
void LEDAllState(int State, long ms) // ms at least 1(ms)
{
  switch (State)
  {
    case 0: // LedAllOff
      FastLED.clear();
      delay(ms);
      FastLED.show();
      break;

    case 1: // Connecting
      for (uint16_t i = 0; i < LedNum; i++)
      {
        leds[0] = CRGB(25, 10, 10);
      }
      FastLED.show();
      delay(ms);
      break;

    case 2: // Connect/sent Succeed
      for (uint16_t i = 0; i < LedNum; i++)
      {
        leds[0] = CRGB(0, 255, 0);
      }
      FastLED.show();
      delay(ms);
      break;

    case 3: // Connect/sent fail
      for (uint16_t i = 0; i < LedNum; i++)
      {
        leds[0] = CRGB(255, 0, 0);
      }
      FastLED.show();
      delay(ms);
      break;

    case 4: // Test
      for (uint16_t i = 0; i < LedNum; i++)
      {
        leds[0] = CRGB(25, 0, 25);
      }
      FastLED.show();
      delay(ms);
      break;

    case 5: // WifiManeger RST all
      for (uint16_t i = 0; i < LedNum; i++)
      {
        leds[0] = CRGB(127, 0, 255);
      }
      FastLED.show();
      delay(ms);
      break;

    case 6: // NodeRed back
      for (uint16_t i = 0; i < LedNum; i++)
      {
        leds[0] = CRGB(r, g, b);
      }
      FastLED.show();
      delay(ms);
      break;
  }
}

char* toCharArray(String str)
{
  return &str[0];
}

void PMS5003T()
{
  //    Serial.println("Send read request...");
  pms.requestRead();

  //  Serial.println("Wait max. 1 second for read...");
  if (pms.readUntil(data))
  {
    // Standard Particles, CF=1
    A = String(data.PM_SP_UG_1_0);
    B = String(data.PM_SP_UG_2_5);
    C = String(data.PM_SP_UG_10_0);

    // Atmospheric environment
    D = String(data.PM_AE_UG_1_0);
    E = String(data.PM_AE_UG_2_5);
    F = String(data.PM_AE_UG_10_0);

    // Particles Number
    G = String(data.PM_PN_UG_0_3);
    H = String(data.PM_PN_UG_0_5);
    I = String(data.PM_PN_UG_1_0);
    J = String(data.PM_PN_UG_2_5);

    // Tmperature & Humidity
    K = String(data.Tmperature).toFloat() / 10 ;
    L = String(data.Humidity).toFloat() / 10 ;

    //
    //    Serial.print("PM 1.0 (ug/m3): ");
    //    Serial.println(data.PM_AE_UG_1_0);
    //
    //    Serial.print("PM 2.5 (ug/m3): ");
    //    Serial.println(data.PM_AE_UG_2_5);
    //
    //    Serial.print("PM 10.0 (ug/m3): ");
    //    Serial.println(data.PM_AE_UG_10_0);
    //
    //    Serial.print("Temperature: ");
    //    Serial.println(String(data.Tmperature).toFloat() / 10);
    //
    //    Serial.print("Humidity: ");
    //    Serial.println(String(data.Humidity).toFloat() / 10);
  }
  else
  {
    Serial.println("No data.");
  }

  //  Serial.println("Going to sleep for 60 seconds.");
  //  pms.sleep();
  //  delay(1000);
}

void PM25TFT()
{
  tft.fillScreen(TFT_BLACK);// refresh the screen by black
  int yInterval = 20;
  //
  int txtXpos = 3; // draw string centred on dY
  int txtYpos = 5; // draw string centred on dY
  int txtFont = 1;
  //
  tft.setTextColor(colour);// font color white
  tft.drawString(" PM 1.0 (ug/m3): ", txtXpos, txtYpos, txtFont);
  tft.drawString(" PM 2.5 (ug/m3): ", txtXpos, txtYpos + yInterval * 1, txtFont);
  tft.drawString("PM 10.0 (ug/m3): ", txtXpos, txtYpos + yInterval * 2, txtFont);
  tft.drawString("    Temperature: ", txtXpos, txtYpos + yInterval * 3, txtFont);
  tft.drawString("       Humidity: ", txtXpos, txtYpos + yInterval * 4, txtFont);
  //
  //
  int valXpos = 100; // draw string centred on dX
  int valYpos = 5; // draw string centred on dY
  int valFont = 1;
  //
  tft.setTextColor(TFT_GREEN, TFT_RED);// font color & BG color
  tft.drawString(D, valXpos, valYpos, txtFont);
  tft.drawString(E, valXpos, valYpos + yInterval * 1, txtFont);
  tft.drawString(F, valXpos, valYpos + yInterval * 2, txtFont);
  tft.drawString(K, valXpos, valYpos + yInterval * 3, txtFont);
  tft.drawString(L, valXpos, valYpos + yInterval * 4, txtFont);

}
