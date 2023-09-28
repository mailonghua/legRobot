#include "HAL/HAL.h"

void HAL::HAL_Init()
{
    Buzz_Init();
    OLED_Init();
    CAN_Init();/*init and start recv thread*/
    OTA_Init();
    key_init();
}

void HAL::HAL_Update()
{
    Buzz_Update();
    OLED_Test_Display();
    OTA_Update();
    //Motor_Test();
}