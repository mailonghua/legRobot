#include "HAL.h"

uint16_t test_level = 0;
void HAL::Draw_SIN_Test()
{
    #define NUM_STEP 100
    #define AMPLITUDE 2000
    float x = 0.0;
    float y = 0.0;
    const float x_step = (2*PI)/NUM_STEP;
    for(int i=0;i<NUM_STEP;i++)
    {
        x = -PI + x_step*i;
        y = (sin(x)+1)*AMPLITUDE + (test_level++);
        Draw_One_Line(y);
    }
}