#include "HAL.h"
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <U8g2lib.h>
#define U8G2_USAGE
#ifdef ADAFRUIT_SSD1306
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
#else 
static unsigned long previousMillis = 0;
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C     u8g2(U8G2_R2 , /* clock=*/ I2C_SCL_0, /* data=*/ I2C_SDA_0, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
#endif
#ifdef ADAFRUIT_SSD1306
void HAL::OLED_Init()
{
    Wire.begin(/*SDA*/I2C_SDA_0,/*SCL*/I2C_SCL_0);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    //将屏幕进行翻转
    display.ssd1306_command(0xA0);
    display.ssd1306_command(0xC0);
}
void HAL::OLED_Test_Display()
{
    // 清除屏幕
    display.clearDisplay();
    // 设置字体颜色,白色可见
    display.setTextColor(WHITE);
    //设置字体大小
    display.setTextSize(1.5);
    //设置光标位置
    display.setCursor(0, 0);
    display.print("0.91 OLED");
    display.setCursor(0, 10);
    display.print("Run time: ");
    //打印自开发板重置以来的秒数：
    display.print(millis() / 1000);
    display.print(" s");

    display.setCursor(0, 20);
    display.print("IP: ");
    display.print(Current_IP_Address.c_str());

    //display.invertDisplay(true);
    display.display();//显示

 
}
#else
void HAL::OLED_Init()
{
    #ifdef U8G2_USAGE
        INFOLN("Start init u8g2");
        u8g2.begin();
        u8g2.setFont(u8g2_font_t0_11_tf); // choose a suitable font
    #endif
}
String ip_add = "IP:";
void HAL::OLED_Test_Display()
{
    #ifdef U8G2_USAGE
    unsigned long currentMillis = millis();//millis
    if (currentMillis - previousMillis >= 1000) 
    {
        previousMillis = currentMillis;
        u8g2.clearBuffer();         // 清除内部缓冲区
        
        u8g2.drawStr(0,10,"Hello World!");  // write something to the internal memory
        //u8g2.drawStr(0,20,"This is bro cainiao!");  // write something to the internal memory
        ip_add = "IP:" + Current_IP_Address;
        u8g2.drawStr(0,30,ip_add.c_str());  // write something to the internal memory
        u8g2.sendBuffer();          // transfer internal memory to the display
        //delay(1000);
    }
    #endif
}
void HAL::OLED_Init(u8g2_t* u8g2)
{
    // // u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_hw_i2c, u8x8_gpio_and_delay);
    // // U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C     u8g2(U8G2_R2 , /* clock=*/ I2C_SCL_0, /* data=*/ I2C_SDA_0, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
    // u8g2_Setup_ssd1306_i2c_128x32_univision_f(u8g2, U8G2_R0, u8x8_byte_arduino_sw_i2c, u8x8_gpio_and_delay_arduino);
    // u8x8_SetPin_SW_I2C(getU8x8(), I2C_SCL_0,  I2C_SDA_0,  reset);
    // u8g2_InitDisplay(u8g2);
    // u8g2_SetPowerSave(u8g2, 0);  // 使能
    // u8g2_ClearBuffer(u8g2);
}
#endif