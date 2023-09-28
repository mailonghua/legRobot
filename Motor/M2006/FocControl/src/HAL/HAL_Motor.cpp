#include "HAL.h"
static unsigned long previousMillis = 0;   // will store last time a CAN Message was send
static const int interval = 1000;   
static HAL::SendMotorData data;

extern QueueHandle_t Motor_Queue[DJ_C610_M2006_MOTOR_NUM];
void HAL::Motor_Init()
{
    data.Motor_Data.Motor1_Current = 0;
    data.Motor_Data.Motor2_Current = 0;
    data.Motor_Data.Motor3_Current = 0;
    data.Motor_Data.Motor4_Current = 0;
}
/*
    两个电机的转矩控制
*/
/*下发电机控制数据(力矩)*/
void HAL::Motor_Control_Torque(uint16_t Motor1,uint16_t Motor2)
{
    data.Motor_Data.Motor1_Current = Motor1;
    data.Motor_Data.Motor2_Current = Motor2;
    CAN_Send(data,0x200);
}
/*获得电机反馈数据(机械角度，速度，力矩)*/
inline HAL::ReceiveMotorData*  HAL::Motor_Feedback()
{
    static HAL::ReceiveMotorData motor_feed[DJ_C610_M2006_MOTOR_NUM];//电机的反馈数据
    //1.Get new data in queue
    for(int i = 0 ; i<DJ_C610_M2006_MOTOR_NUM ; i++)
    {
        if(pdFALSE == xQueueReceive(Motor_Queue[i],&motor_feed[i],0))
            ERR("Not get data in queue.\n");
    }     
    return &motor_feed[0];
}
void HAL::Motor_Test()
{
    static uint16_t num = 0;
    static uint16_t Motor_Current = 0;
    static int flag = 0;
    unsigned long currentMillis = millis();//millis
    

    //500HZ,2ms
    if (currentMillis - previousMillis >= 2) {
        previousMillis = currentMillis;
        num++;
        if(num >= 500)//1s
        {
            if(Motor_Current >= 1000)
                flag = -1;
            else if(Motor_Current <= 100)
                flag = 1;
            
            Motor_Current = Motor_Current + flag * 100;
            num = 0;

        }
        data.Motor_Data.Motor2_Current = Motor_Current;
        DEBUG("Motor Send Current = %u\r\n",Motor_Current);
        CAN_Send(data,0x200);
        
    }
}