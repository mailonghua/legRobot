#ifndef __MICRO_ROS_MSG_H__
#define __MICRO_ROS_MSG_H__
#include "microROS.h"
#include <std_msgs/msg/int32.h>
#include "std_msgs/msg/detail/header__struct.h"
// #include <fishbot_interfaces/srv/oled_control.h>
/*
*Refer website:https://micro.ros.org/docs/tutorials/advanced/handling_type_memory/
*/
/***********PID*************/
typedef struct MyMicroRosPkg__msg__PIDConfig
{
    std_msgs__msg__Header header;
    double pid_p;
    double pid_i;
    double pid_d;
}MyMicroRosPkg__msg__PIDConfig;
/*单级PID环-MSG*/
typedef struct MyMicroRosPkg__msg__PIDSpeed
{
    std_msgs__msg__Header header;
    double Target_Speed;
    double Current_Speed;
}MyMicroRosPkg__msg__PIDSpeed;
/*串级PID环-MSG*/
typedef struct MyMicroRosPkg__msg__PIDCaseCade
{
    std_msgs__msg__Header header;
    /*Todo*/
}MyMicroRosPkg__msg__PIDCaseCade;
/***********Motor*************/
typedef struct MyMicroRosPkg__msg__Motor
{
    std_msgs__msg__Header header;
    uint16_t Rotor_Machinery_Angle;//转子机械角度
    uint16_t Rotor_Speed;//转子速度
    uint16_t Toque;//转矩
}MyMicroRosPkg__msg__Motor;

#endif