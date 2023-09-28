#ifndef __CONFIG_H__
#define __CONFIG_H__
//Buzz
#define CONFIG_BUZZ_PIN 2

//OLED 128*32
#define SCREEN_WIDTH 128 // 设置OLED宽度,单位:像素
#define SCREEN_HEIGHT 32 // 设置OLED高度,单位:像素
#define OLED_RESET -1  

#define I2C_SDA_0 19
#define I2C_SCL_0 23

#define CAN_TX GPIO_NUM_18
#define CAN_RX GPIO_NUM_5

#define STASSID "Xiaomi_Mailonghua"
#define STAPSK "mlh8823727"

//KEY
#define KEY_1   4
#define KEY_2   22
#define KEY_3   21

#define CONFIG_SCREEN_HEIGHT 32
#define CONFIG_SCREEN_WIDTH 128

//CAN电机数量定义
#define DJ_C610_M2006_MOTOR_NUM 1
#define MOTOR1_CAN_ID (0x200)//还需确认
#define MOTOR2_CAN_ID (0x201)//还需确认
/*Function thread loop frequency*/
#define ALGORITHM_UPDATE 20 //20ms-50hz 
#define MICRO_ROS_UPDATE 20
#define MAIN_LOOP_UPDATE 20

/*
    Uncomment this define choice graph view function
    Comment this define choice normal uart debug
*/
#define ENABLE_CONFIG_GRAPH_UART
/*
    if Enable ENABLE_CONFIG_GRAPH_UART and choose display type
    0:Serial uart[http://www.starlino.com/data/imu_kalman_arduino/SerialChart_01.zip]
    1:FireUart  [https://www.firebbs.cn/forum.php?mod=viewthread&tid=29923&extra=page%3D1]
    2:VOFA[https://github.com/skjsnb/VOFA-Protocol-Driver]
*/
#define DEBUG_DISPLAY_UI_TYPE (2)

/*
    Define VOFA trasnport type
    0:UART
    1:UDP
    2:TCP[not use]
*/
#define VOFA_TRANSMISSION_TYPE (1)

#define VOFA_UDP_LOCAL_PORT 2333
#define VOFA_UDP_REMOTE_PORT 2334
#endif