#include "HAL.h"
CAN_device_t CAN_cfg;        // CAN Config
const int rx_queue_size = 10;       // Receive Queue size

static unsigned long previousMillis = 0;   // will store last time a CAN Message was send
const int interval = 1000;   
static HAL::ReceiveMotorData motor_feed[DJ_C610_M2006_MOTOR_NUM];//电机的反馈数据
//static uint8_t CAN_Recv_Data[8];
QueueHandle_t Motor_Queue[DJ_C610_M2006_MOTOR_NUM];
void HAL::CAN_Init()
{
    for(int i = 0;i<DJ_C610_M2006_MOTOR_NUM;i++)
    {
        Motor_Queue[i] = xQueueCreate(1,sizeof(HAL::ReceiveMotorData)); 
        if(Motor_Queue[i] == NULL)
        {
            while(1)
            {
                ERR("CAN_Init:xQueueCreate return null...\n");
                sleep(1);
            }
        }
    }
    INFOLN("Init CAN Config...");
    CAN_cfg.speed = CAN_SPEED_1000KBPS;
    CAN_cfg.tx_pin_id = CAN_TX;
    CAN_cfg.rx_pin_id = CAN_RX;
    CAN_cfg.rx_queue = xQueueCreate(rx_queue_size, sizeof(CAN_frame_t));
    // Init CAN Module
    ESP32Can.CANInit();
    //Create thread run Can receive
    xTaskCreate(HAL::CAN_Update,"CAN_Updata",4096,NULL,0,NULL);
}
void HAL::CAN_Receive_Template()
{
    CAN_frame_t rx_frame;
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) {
        if (rx_frame.FIR.B.FF == CAN_frame_std) 
        {
            DEBUG("New standard frame");
        }
             
        else 
        {
            DEBUG("New extended frame");
        }
    if (rx_frame.FIR.B.RTR == CAN_RTR) {//远程帧
      DEBUG(" RTR from 0x%08X, DLC %d\r\n", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
    }
    else {
        DEBUG(" from 0x%08X, DLC %d, Data ", rx_frame.MsgID,  rx_frame.FIR.B.DLC);
        for (int i = 0; i < rx_frame.FIR.B.DLC; i++) {
            DEBUG("0x%02X ", rx_frame.data.u8[i]);
        }
        DEBUG("\n");
    }     

    }
}
void inline Swap_Byte(uint8_t &Byte1,uint8_t &Byte2)
{
    uint8_t temp;
    temp = Byte1;
    Byte1 = Byte2;
    Byte2 = temp;
}
void HAL::CAN_Receive(ReceiveMotorData *recv_data)
{
    CAN_frame_t rx_frame;
    if(recv_data == NULL)
    {
        ERR("CAN_Receive recv_data is null\n");
        return;
    }
    if (xQueueReceive(CAN_cfg.rx_queue, &rx_frame, 3 * portTICK_PERIOD_MS) == pdTRUE) 
    {
        if (rx_frame.FIR.B.FF == CAN_frame_std) 
        {
            const uint32_t motor_id = rx_frame.MsgID - 0x200 - 1;
            recv_data[motor_id].Motor_CAN_ID = rx_frame.MsgID;
            if(motor_id > (DJ_C610_M2006_MOTOR_NUM - 1))
            {
                ERR("Recv motor id is too large(MAX_MOTOR_NUM:%d,motor_id:%d,MsgID:0x%x)...\n ",
                                                DJ_C610_M2006_MOTOR_NUM,motor_id,rx_frame.MsgID);      
                return;
            }
            for (int i = 0; i < rx_frame.FIR.B.DLC; i++)
            {
                recv_data[motor_id].Feed_Data.data[i] = rx_frame.data.u8[i];
            }  
            //Swap
            Swap_Byte(recv_data[motor_id].Feed_Data.data[0],recv_data[motor_id].Feed_Data.data[1]);
            Swap_Byte(recv_data[motor_id].Feed_Data.data[2],recv_data[motor_id].Feed_Data.data[3]);
            Swap_Byte(recv_data[motor_id].Feed_Data.data[4],recv_data[motor_id].Feed_Data.data[5]);
            //Overwrite in queue
            xQueueOverwrite(Motor_Queue[motor_id],recv_data);
        }
    }
}

void HAL::CAN_Update(void*)
{
    INIT_TASK_TIME(1);
    INFO("Start CAN_Update thread\n");
    while(1)
    {
        CAN_Receive(motor_feed);
        // #ifndef  ENABLE_CONFIG_GRAPH_UART
        //     /*串口打印*/
        //     for(int i = 0; i<DJ_C610_M2006_MOTOR_NUM ; i++)
        //     {
        //         DEBUG("Motor:%u:Angle=%u,Speed=%u,Toque=%u\r\n",
        //             motor_feed[i].Motor_CAN_ID,
        //             motor_feed[i].Feed_Data.Motor_Data.Rotor_Machinery_Angle,
        //             motor_feed[i].Feed_Data.Motor_Data.Rotor_Speed,
        //             motor_feed[i].Feed_Data.Motor_Data.Toque);
        //     }
        // #else
        //     #if (DJ_C610_M2006_MOTOR_NUM == 2)
        //         /*图形显示*/
        //         Draw_Six_Line(motor_feed[0].Feed_Data.Motor_Data.Rotor_Machinery_Angle,
        //                         motor_feed[0].Feed_Data.Motor_Data.Rotor_Speed,
        //                         motor_feed[0].Feed_Data.Motor_Data.Toque,
        //                         motor_feed[1].Feed_Data.Motor_Data.Rotor_Machinery_Angle,
        //                         motor_feed[1].Feed_Data.Motor_Data.Rotor_Speed,
        //                         motor_feed[1].Feed_Data.Motor_Data.Toque);
        //     #elif (DJ_C610_M2006_MOTOR_NUM == 1)
        //         Draw_Three_Line(motor_feed[0].Feed_Data.Motor_Data.Rotor_Machinery_Angle,
        //                             motor_feed[0].Feed_Data.Motor_Data.Rotor_Speed,
        //                             motor_feed[0].Feed_Data.Motor_Data.Toque);
        //     #endif
        // #endif
        WAIT_TASK_TIME();
    }
}
/*获得当前已经存储电机反馈数据*/
//Old api not use 新机制已经使用队列传递
HAL::ReceiveMotorData* HAL::CAN_Get_Data()
{
    return &motor_feed[0];
}
/*
    电机CAN ID 0X200和0x1FF各自对应控制4个ID的电调
*/
void HAL::CAN_Send(SendMotorData &send_data,uint32_t MsgID)
{
    CAN_frame_t tx_frame;
    tx_frame.FIR.B.FF = CAN_frame_std;
    tx_frame.FIR.B.RTR = CAN_no_RTR;
    tx_frame.MsgID = MsgID;
    tx_frame.FIR.B.DLC = 8;
    /*Motor1*/
    tx_frame.data.u8[0] = send_data.data[1];
    tx_frame.data.u8[1] = send_data.data[0];
    /*Motor2*/
    tx_frame.data.u8[2] = send_data.data[3];
    tx_frame.data.u8[3] = send_data.data[2];

    tx_frame.data.u8[4] = send_data.data[5];
    tx_frame.data.u8[5] = send_data.data[4];

    tx_frame.data.u8[6] = send_data.data[7];
    tx_frame.data.u8[7] = send_data.data[6];
    if(0 != ESP32Can.CANWriteFrame(&tx_frame))
        ERR("ESP32Can.CANWriteFrame ERROR\r\n");  
}


void HAL::CAN_Send_Template()
{
    CAN_frame_t tx_frame;
    unsigned long currentMillis = millis();//millis
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;
        
        tx_frame.FIR.B.FF = CAN_frame_std;
        tx_frame.FIR.B.RTR = CAN_no_RTR;//用于区分是远程帧还是数据帧，当该位位1是远程帧，当该位为0是数据帧[https://www.cnblogs.com/hira2020/p/14383639.html]
        tx_frame.MsgID = 0x104;
        tx_frame.FIR.B.DLC = 8;
        tx_frame.data.u8[0] = 0x00;
        tx_frame.data.u8[1] = 0x01;
        tx_frame.data.u8[2] = 0x02;
        tx_frame.data.u8[3] = 0x03;
        tx_frame.data.u8[4] = 0x04;
        tx_frame.data.u8[5] = 0x05;
        tx_frame.data.u8[6] = 0x06;
        tx_frame.data.u8[7] = 0x07;
        
        if(0 != ESP32Can.CANWriteFrame(&tx_frame))
            ERR("ESP32Can.CANWriteFrame ERROR\r\n");
    }
}