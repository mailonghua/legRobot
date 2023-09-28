#include "HAL.h"
using namespace HAL;

key_id_t    key_id;  // current
key_state_t keys[KEY_ID_END];
void HAL::key_init(void)
{
    pinMode(KEY_1,INPUT_PULLUP);
    pinMode(KEY_2,INPUT_PULLUP);
    pinMode(KEY_3,INPUT_PULLUP);

    for (uint8_t i = 0; i < ARRAY_SIZE(keys); ++i)
        keys[i].cur = keys[i].last = key_val(i + 1);
}
int HAL::key_scan(void)
{
    for (uint8_t i = 0; i < 3; i++) {
        keys[i].cur = key_val(i + 1);
        if (keys[i].last != keys[i].cur) {
            keys[i].last = keys[i].cur;
            
            if (keys[i].cur)
            {
                INFO("%d\r\n", keys[i].cur);
                return i + 1; // key pressed
            }
        }
    }
    return 0;
}
bool HAL::key_val(uint8_t id)
{
    switch (id) {
        case KEY_ID_OK:   return digitalRead(KEY_1) == 0;    
        case KEY_ID_PREV: return digitalRead(KEY_2) == 0; 
        case KEY_ID_NEXT: return digitalRead(KEY_3) == 0;
        default: break;
    }
    return false;
}