#include "HAL.h"
static bool Buzz_play_flag = 0;

#define USE_BEEP_ARDUINO_LIB
ezBuzzer buzzer(CONFIG_BUZZ_PIN);
void HAL::Buzz_Init()
{
    buzzer.beep(1000);
}

void HAL::Buzz_Update()
{
    buzzer.loop();
}
void HAL::Buzz_mode_choice(HAL::BEEP_MODE mode)
{
    switch (mode)
    {
    case CONVERT_MODE:
        buzzer.beep(200);
        break;
    
    default:
        Serial.printf("Buzz_mode_choice not match (%d)\r\n",mode);
        break;
    }

}