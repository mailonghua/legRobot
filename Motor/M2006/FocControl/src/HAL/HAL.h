#ifndef _HAL__H_
#define _HAL__H_
#include <Arduino.h>
#include <ESP32CAN.h>
#include <CAN_config.h>
#include <WiFi.h>
#include <AsyncUDP.h> 
#include "FreeRTOS/FreeRTOS.h"
#include "FreeRTOS/queue.h"
#include "ezBuzzer.h"
#include "Config/Config.h"
#include "HAL_Def.h"
#include "Log.h"
#include "clib/u8g2.h"
#include "clib/u8x8.h"
#include "clib/mui.h"
#include "clib/mui_u8g2.h"
#include <U8g2lib.h>

DEBUG_SET_LEVEL(DEBUG_LEVEL_DEBUG);
extern String Current_IP_Address;
/*线程等待一定时间,按照一定频率执行线程while(1)*/
#define INIT_TASK_TIME(WAIT_MS) \
        portTickType xLastWakeTime; \
        const portTickType xFrequency = pdMS_TO_TICKS(WAIT_MS); \
        xLastWakeTime = xTaskGetTickCount();   \

#define WAIT_TASK_TIME() \
    vTaskDelayUntil(&xLastWakeTime, xFrequency);

namespace HAL
{
    void HAL_Init();
    void HAL_Update();

    //beep
    void Buzz_Init();
    void Buzz_Update();
    void Buzz_mode_choice(HAL::BEEP_MODE mode);

    //oled
    void OLED_Init();
    void OLED_Init(u8g2_t* u8g2);
    void OLED_Test_Display();

    //CAN
    void CAN_Init();
    void CAN_Update(void*);
    void CAN_Receive(ReceiveMotorData * recv_data);
    void CAN_Send(SendMotorData &send_data,uint32_t MsgID);
    void CAN_Receive_Template();
    void CAN_Send_Template();
    ReceiveMotorData* CAN_Get_Data();

    //Motor
    void Motor_Test();
    void Motor_Init();
    void Motor_Control_Torque(uint16_t Motor1,uint16_t Motor2);
    inline ReceiveMotorData*   Motor_Feedback();

    //OTA
    void OTA_Init();
    void OTA_Update();

    //KEY
    void  key_init(void);
    int   key_scan(void);
    bool  key_val(uint8_t id);

    /*
    *Use for Graph pic interface
    */
    void inline Draw_One_Line(float para1)
    {
        DRAW_LINE("%f\n",para1);
    }
    void inline Draw_Two_Line(float para1,float para2)
    {
        DRAW_LINE("%f,%f\n",para1,para2);
    }
    void inline Draw_Three_Line(float para1,float para2,float para3)
    {
        DRAW_LINE("%f,%f,%f\n",para1,para2,para3);
    }
    void inline Draw_Four_Line(float para1,float para2,float para3,float para4)
    {
        DRAW_LINE("%f,%f,%f,%f\n",para1,para2,para3,para4);
    }
    void inline Draw_Five_Line(float para1,float para2,float para3,float para4,float para5)
    {
        DRAW_LINE("%f,%f,%f,%f,%f\n",para1,para2,para3,para4, para5);
    }
    void inline Draw_Six_Line(float para1,float para2,float para3,
                                float para4,float para5,float para6)
    {
        DRAW_LINE("%f,%f,%f,%f,%f,%f\n",para1,para2,para3,para4,para5,para6);
    }
    void Draw_SIN_Test();

}
#endif