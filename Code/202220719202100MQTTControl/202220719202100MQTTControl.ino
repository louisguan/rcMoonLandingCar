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
const char* mqttServer = "MQTTGO.io"; // mqtt server address
const int mqttPort = 1883; // mqtt port
const char* mqttUser = ""; // your mqtt user
const char* mqttPassword = ""; // your mqtt password
//
String clientId = "ESP32Client-"; // Create a random client ID
// #include <WiFi.h>
PubSubClient client(espClient);
//
#define MQTT_SUB_Direction "esp32/Direction/Car001"
#define MQTT_PUB_HandShake "esp32/init"
#define MQTT_SUB_ACK "esp32/ack"

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

/*---------------------------------------Other-----------------------------------------*/
long lastMsg = 0;
//
#define TRIGGER_PIN 0

/*---------------------------------------MQTT CallBack-----------------------------------------*/
int r = 0, g = 255, b = 0;
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
  if (String(topic) == MQTT_SUB_Direction)
  {
    Serial.println("GOT it");
  }
  else
  {
    // do nothing
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
  tft.setCursor(10, 0, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.println("System Ininitialing......\r\n");
  //
  tft.setTextFont(2);
  tft.print("Device ID: ");
  tft.println(String(WIFI_getChipId(), HEX).c_str());
  //
  tft.setTextFont(1);
  tft.print("- WIFI connecting...Please Wait!!");
  //  tft.print(ssid);
  WIFIreconnect();
  tft.println(" V"); delay(250);
  tft.print("LocalIP:"); tft.println(WiFi.localIP());
  delay(500);
  //
  //  tft.print("- WS2812b.......");
  //  strip.begin();
  //  strip.setBrightness(Brightness);
  //  strip.show(); // Initialize all pixels to 'off'
  //  LEDAllState(0, 1);
  //  tft.println(" V"); delay(250);
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
  client.subscribe(MQTT_SUB_Direction); // option or need with topic
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

  //
  long now = millis();
  if (now - lastMsg > 1000)
  {
    lastMsg = now;
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
  res = wm.autoConnect(String(WIFI_getChipId(), HEX).c_str(), "0989839679"); // password protected ap

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

      if ( digitalRead(TRIGGER_PIN) == LOW )
      {
        Serial.println("Button Held");
        Serial.println("Erasing Config, restarting");
        wm.resetSettings();
        ESP.restart();
      }

      // start portal w delay
      Serial.println("Starting config portal");
      wm.setConfigPortalTimeout(120);

      if (!wm.startConfigPortal(String(WIFI_getChipId(), HEX).c_str(), "0989839679"))
      {
        Serial.println("failed to connect or hit timeout");
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
    }
    else
    {
      Serial.print("failed with state ");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
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
  Serial.print("RSSI: ");
  Serial.print(rssi);
  Xpos += tft.drawString("RSSI: ", Xpos, 220, 2);
  Xpos += tft.drawString(String(rssi), Xpos, 220, 2); // drawFloat(value, precision, x, y, font);
  //  //
  String Mac = WiFi.macAddress();
  DeviceName = Mac;
  Serial.print(", Mac:");
  Serial.println(Mac);
  Xpos += tft.drawString("   XYG:", Xpos, 220, 2);
  Xpos += tft.drawString(Mac, Xpos, 220, 2);
  Xpos += tft.drawString(":FF", Xpos, 220, 2);
}

char* toCharArray(String str)
{
  return &str[0];
}
