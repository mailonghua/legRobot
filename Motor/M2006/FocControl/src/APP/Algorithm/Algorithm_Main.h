#ifndef __ALGORITHM_MAIN_H__
#define __ALGORITHM_MAIN_H__
#include "Tools/Tools.h"
typedef struct
{
    float TargetSpeed;
    float CurrentSpeed;
    float Error;
} SpeedInner_data_t_;
namespace ALGORITHM
{
void Algorithm_Update(void*);
void Algorithm_Init();
void Algorithm_Send_UI_Display(void * data,int32_t channel);
void Algorithm_InitData_UI_Display();
float Get_Target_Speed();
float Get_Target_Angle();
void Set_Target_Speed(float target);
void Set_Target_Angle(float target);
void* GetCurrentPidObject();
void EnAndDisPidFunc();
}
#endif