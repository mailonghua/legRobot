#ifndef _MICRO_ROS_H__
#define _MICRO_ROS_H__
#include "HAL/HAL.h"
#include <micro_ros_platformio.h>
#include <rcl/rcl.h>
#include <rclc/rclc.h>
#include <rclc/executor.h>
#include <rcl/error_handling.h>
// #include "my_custom_message/msg/my_custom_message.h"
#include <std_msgs/msg/float32.h>

#define RCCHECK(fn) { rcl_ret_t temp_rc = fn; if((temp_rc != RCL_RET_OK)){return false;}}
#define EXECUTE_EVERY_N_MS(MS, X)  do { \
  static volatile int64_t init = -1; \
  if (init == -1) { init = uxr_millis();} \
  if (uxr_millis() - init > MS) { X; init = uxr_millis();} \
} while (0) \

enum states {
  WAITING_AGENT,
  AGENT_AVAILABLE,
  AGENT_CONNECTED,
  AGENT_DISCONNECTED
} ;

class Micro_ROS;
/*
*Thread func use to class member func
* Refer to Website[https://www.cnblogs.com/schips/p/using_cpp_class_member_function_as_normal_callback.html] 
*/
union for_callback
{
    /* data */
    void (*fun_in_c)(void*);
    void (Micro_ROS::*fun_in_class)(void);
};

union for_micro_timer_callback
{
    /* data */
    void (*fun_in_c)(void*,rcl_timer_t *timer, int64_t last_call_time);
    void (Micro_ROS::*fun_in_class)(rcl_timer_t *timer, int64_t last_call_time);
};
/*Micro_ROS暂时和VOFA-PC共用串口1，因此不能同时使能*/
class Micro_ROS 
{
public:

    Micro_ROS(std::string Name);
    Micro_ROS();
    void init();
    void Update(u_int16_t ms);
    void Micro_ROS_Start();
    void micros_task(void);
    /*Use for microRos timer*/
    static void micros_timer_200HZ(rcl_timer_t *timer, int64_t last_call_time);
    static void micros_callback_subscription(const void *msgin);
public:
    const uint8_t micro_ros_callback_num = 2;//执行器callback个数需要显示设置
    rclc_executor_t executor;
    rclc_support_t support;
    rcl_allocator_t allocator;
    rcl_node_t node;
    std::string Node_Name;
    enum states state;
    union for_callback fp;
    union for_micro_timer_callback fp_timer_200hz;
    /*Use for microRos timer*/
    static Micro_ROS * cur_class_ptr;
    
    rcl_subscription_t Subscriber_PID;
    rcl_publisher_t Publisher_PID;
    // my_custom_message__msg__MyCustomMessage test_msg;
    std_msgs__msg__Float32 pub_msg_pid_p;
    // std_msgs__msg__Float32 pub_msg_pid_i;
    // std_msgs__msg__Float32 pub_msg_pid_d;
    rcl_timer_t timer_200HZ;
    //MyMicroRosPkg__msg__PIDSpeed pid_speed_msg;   
private:
    states check_microROS_agency();
    bool create_entities();
    void destroy_entities();
    void Init_MicroROS_Publish_Subscribe();
    void Create_MicroRos_Publish();
    void Destory_MicroRos_Publish();
    void Create_MicroRos_Subscribe();
    void Destory_MicroRos_Subscribe();
    void Create_MicroRos_Timer();
};
#endif