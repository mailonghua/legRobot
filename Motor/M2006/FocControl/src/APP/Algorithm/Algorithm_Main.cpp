#include "PID.h"
#include "HAL/HAL.h"
#include "Algorithm_Main.h"
using namespace HAL;
//#define CASECADE_PID//串级PID
bool PIDEnableFlag = 1;
//速度环（单级）
SpeedInner_data_t_ speedInnerData;//速度目标位置（单级）
//角度环（串级）
static float angleTarget = 180;//角度目标位置（串级）
/*算法初始化*/
const float P_I_D[3]={1.7,0.1,0};
PID SpeedPID(P_I_D[0],P_I_D[1],P_I_D[2],30000,30000);//速度
// PID AnglePID(0, 0, 0, 30000, 30000);//角度
void ALGORITHM::Algorithm_Init()
{
#ifndef CASECADE_PID
    // PID SpeedPID(P_I_D[0],P_I_D[1],P_I_D[2],30000,30000);
    // ALGORITHM::Algorithm_InitData_UI_Display();//将初始的PID值发送到UI上
    // sleep(1);
#else
    PID SpeedPID(15,3,0,30000,30000);
    PID angleTarget(0, 0, 0, 30000, 30000);
#endif 
    /*Create thread run Algorithm_Update*/
    if(pdPASS !=xTaskCreate(ALGORITHM::Algorithm_Update,
                "Algorithm_thread",
                2048,
                NULL,
                0,
                NULL
                ))
    {
        while(1)
        {
            ERR("xTaskCreate[%s] error\n","Algorithm_thread");
            sleep(1);
        } 
    }
}

void ALGORITHM::Algorithm_Update(void*)
{
    HAL::SendMotorData send_data;
    speedInnerData.TargetSpeed = 0;
    INIT_TASK_TIME(ALGORITHM_UPDATE);//20ms--50HZ
    INFO("Start Algorithm_Update thread...\n");
    INFO("Init PID(%f,%f,%f) MotorSpeed(%f)\n",SpeedPID.get_pid_para_kp(),SpeedPID.get_pid_para_ki(),SpeedPID.get_pid_para_kd(),speedInnerData.TargetSpeed);
    while(1)
    {
#ifndef CASECADE_PID
        if(PIDEnableFlag == 1){
        /*速度环*/
            speedInnerData.CurrentSpeed = static_cast<float>(HAL::CAN_Get_Data()->Feed_Data.Motor_Data.Rotor_Speed);
            send_data.Motor_Data.Motor1_Current = static_cast<int16_t>(SpeedPID.PID_Calculate(speedInnerData.TargetSpeed,speedInnerData.CurrentSpeed));
            speedInnerData.Error = speedInnerData.TargetSpeed - speedInnerData.CurrentSpeed;
            HAL::CAN_Send(send_data,MOTOR1_CAN_ID);
            ALGORITHM::Algorithm_Send_UI_Display(reinterpret_cast<void*>(&speedInnerData),sizeof(speedInnerData)/sizeof(float));
        }else{
            send_data.Motor_Data.Motor1_Current = 0x0;
            HAL::CAN_Send(send_data,MOTOR1_CAN_ID);
        }
#else
    //todo
#endif
        // TOOL::Example_Show();
        WAIT_TASK_TIME();
    }
    INFO("Exit Algorithm_Update thread\n");
}
/*将数据通过串口发送给上位机*/
void ALGORITHM::Algorithm_Send_UI_Display(void * data,int32_t channel)
{
    float *vofaShowData = reinterpret_cast<float*>(data);
    // INFO("TargetSpeed[%f],CurrentSpeed[%f],PIDOutCurrent[%f]\n",vofaShowData[0],vofaShowData[1],vofaShowData[2]);
    TOOL::SendUIData(vofaShowData,channel);
}
/*将初始值发送给上位机*/
void ALGORITHM::Algorithm_InitData_UI_Display()
{
    //set_computer_value(SEND_P_I_D_CMD, CURVES_CH1, (void *)P_I_D, 3);
}

float ALGORITHM::Get_Target_Speed()
{
    return speedInnerData.TargetSpeed;
}
float ALGORITHM::Get_Target_Angle()
{
    return angleTarget;
}
void ALGORITHM::Set_Target_Speed(float target)
{
    INFO("[ALGORITHM]Set_Target_Speed:%f\n",target);
    speedInnerData.TargetSpeed = target;
}
void ALGORITHM::Set_Target_Angle(float target)
{
    INFO("[ALGORITHM]Set_Target_Angle:%f\n",target);
    angleTarget = target;
}
void* ALGORITHM::GetCurrentPidObject()
{
    return reinterpret_cast<void*>(&SpeedPID);
}
void ALGORITHM::EnAndDisPidFunc()
{
    INFO("EnAndDisPidFunc:%d\n",PIDEnableFlag);
    PIDEnableFlag = !PIDEnableFlag;
}