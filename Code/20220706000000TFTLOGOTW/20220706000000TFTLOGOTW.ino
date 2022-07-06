/*---------------------------------------ILI9341_DRIVER 320x240 2.2' TFT-----------------------------------------*/
// https://blog.csdn.net/weixin_45488643/article/details/113702365

/*
   用u8g2函式庫建字庫，OLED顯中文: https://blog.jmaker.com.tw/chinese_oled/
   List of all U8g2 fonts:    https://github.com/olikraus/u8g2/wiki/fntlistall
   TFT_eSPI library:          https://github.com/Bodmer/TFT_eSPI
   U8g2_for_TFT_eSPI library: https://github.com/Bodmer/U8g2_for_TFT_eSPI
   U8g2_Arduino library:      https://github.com/olikraus/U8g2_Arduino
   https://downloads.arduino.cc/libraries/github.com/olikraus/U8g2-2.27.6.zip
*/

/***
   CMD
  // cd ./\Documents\Arduino\libraries\U8g2-2.27.6\tools\font\bdfconv\
  // bdfconv.exe -v ../bdf/unifont.bdf -b 0 -f 1 -M ../build/chinese1.map -d ../bdf/7x13.bdf -n u8g2_font_unifont -o u8g2_font_unifont.c
  // C:\Users\Guan\Documents\Arduino\libraries\U8g2-2.27.6\tools\font\bdfconv
  // C:\Users\Guan\Documents\Arduino\libraries\U8g2-2.27.6\tools\font\build
***/

/*
 * https://www.ifreesite.com/unicode-ascii-ansi.htm
*/

#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library
#include <U8g2_for_TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();       // Invoke custom library
U8g2_for_TFT_eSPI u8f;       // U8g2 font instance

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
  Serial.begin(115200);   // GPIO1, GPIO3 (TX/RX pin on ESP-12E Development Board)

  // setting PWM properties
#define freq 5000
#define ledChannel 0
#define resolution 8
  int TFT_BLBrightness = 200;
  //
  ledcSetup(ledChannel, freq, resolution); //宣告ledChannel特性
  ledcAttachPin(TFT_BL, ledChannel); //將上述ledChannel附加到ledPin腳位上
  ledcWrite(ledChannel, TFT_BLBrightness);

  tft.begin();
  u8f.begin(tft);                     // connect u8g2 procedures to TFT_eSPI
  tft.setRotation(RotationDisplay);

  // 這裡擺LOGO
  tft.fillScreen(TFT_WHITE);
  tft.setSwapBytes(true); // pushImage is used for 16-bit sprites, the effect of swap bytes is reversed
  tft.pushImage(0, 0, 320, 240, SeaOtter);
  delay(2000);
  //
  //  tft.fillScreen(TFT_BLACK); // try to commit and see the result.
  tft.setCursor(5, 25, 2);
  tft.setTextColor(TFT_WHITE);
  tft.setTextFont(1);
  tft.println("System Ini...");
  //
  //
  u8f.setFontDirection(0); // left to right (this is default)
  u8f.setForegroundColor(TFT_VIOLET); // apply color
  //
  u8f.setFont(u8g2_font_siji_t_6x10); // icon font
  u8f.setFontMode(1); // use u8g2 transparent mode (this is default)
  u8f.drawGlyph( 0, 10, 0x0e200); // Power Supply
  u8f.drawGlyph(12, 10, 0x0e201); // Charging
  u8f.drawGlyph(24, 10, 0x0e10a); // Right Arrow
  u8f.drawGlyph(36, 10, 0x0e24b); // full Battery
  //
  //
  // UTF-8 string: "歡迎使用伴伴登肚小車"
  u8f.setFont(u8g2_font_unifont_t_chinese1); // extended font
  u8f.setFontMode(1); // use u8g2 transparent mode (this is default)
  u8f.setCursor(95, 225); // start writing at this position
  String TW = "歡迎使用伴伴登肚小車";
  for (int i = 0; i < TW.length(); i++)
  {
    u8f.print(TW[i]);
    delay(100);
  }
}

void loop()
{
  // put your main code here, to run repeatedly:

}
