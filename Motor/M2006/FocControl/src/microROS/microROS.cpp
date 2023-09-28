#include "microROS.h"
#include "microROS_Msg.h"
#include <std_msgs/msg/int32.h>

#define microROS_Type  Serial1
Micro_ROS * Micro_ROS::cur_class_ptr = NULL;

Micro_ROS::Micro_ROS(std::string Name):Node_Name(Name)
{
    /*NULL*/
}
Micro_ROS::Micro_ROS()
{
    Node_Name = "microROS";
}
void Micro_ROS::init()
{
    INFO("Start Init Mrcro ros\r\n");
    state = WAITING_AGENT;
    Micro_ROS::cur_class_ptr = this;/*Use for static function call this */
    /*Step1: Set communicate interface*/
    microROS_Type.begin(115200);
    set_microros_serial_transports(microROS_Type);
    delay(2000);
}
/*Timer 200HZ callback*/
float test_num = 0.0;
void Micro_ROS::micros_timer_200HZ(rcl_timer_t *timer, int64_t last_call_time)
{
    RCLC_UNUSED(last_call_time);
    //INFO("micros_timer_200HZ...\n");
    if(timer != NULL)
    {
        if(cur_class_ptr == NULL)
        {
            ERR("micros_timer_200HZ this ptr is NULL...return\n");
            return;
        } 
        cur_class_ptr->pub_msg_pid_p.data = (test_num = test_num + 0.1);
        rcl_ret_t ret = rcl_publish(&(cur_class_ptr->Publisher_PID),&(cur_class_ptr->pub_msg_pid_p),NULL);
        if(ret != RCL_RET_OK)
            ERR("[micros_timer_200HZ]: rcl_publish ret:%d\n",ret);
    }else
        ERR("micros_timer_200HZ timer is NUll\n");
}
/*------------------------------------------------------------------------*/
void Micro_ROS::Init_MicroROS_Publish_Subscribe()
{
    INFO("Start create microROS Publisher\n");
    /*Single stage PID*/
    rclc_publisher_init_default(&Publisher_PID,&node,
                                ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
                                "TestCustomMicrosROSMSG"
                                );
    INFO("Start create microROS Subscribe\n");
    /*Single stage PID*/
    rclc_subscription_init_default(&Subscriber_PID,&node,
                                    ROSIDL_GET_MSG_TYPE_SUPPORT(std_msgs, msg, Float32),
                                    "MicroRos_Single_PID");

    INFO("Start create microROS 200HZ Timer\n");
    const unsigned int timer_timeout = 500;
    rcl_ret_t ret = rclc_timer_init_default(&timer_200HZ,&support,RCL_MS_TO_NS(timer_timeout),
                            Micro_ROS::micros_timer_200HZ);
                            // micros_timer_200HZ_callback);
    if(ret != RCL_RET_OK)
        ERR("rclc_timer_init_default error ret =%d\n",ret); 
}

void Micro_ROS::Create_MicroRos_Timer()
{
    INFO("Start create microROS Timer\n");
    rcl_ret_t ret = rclc_executor_add_timer(&executor, &timer_200HZ);
    if(ret != RCL_RET_OK)
        ERR("Create_MicroRos_Timer error ret = %d\n",ret);
}
void Micro_ROS::Create_MicroRos_Publish()
{
    //No need
}
void Micro_ROS::Create_MicroRos_Subscribe()
{
    rcl_ret_t ret = rclc_executor_add_subscription(&executor,&Subscriber_PID,&pub_msg_pid_p,
                                    Micro_ROS::micros_callback_subscription,
                                    ON_NEW_DATA);
    if(ret != RCL_RET_OK)
        ERR("Create_MicroRos_Subscribe error ret = %d\n",ret);
}
void Micro_ROS::Destory_MicroRos_Publish()
{
    INFO("Start destory microROS Publisher\n");
    rcl_ret_t ret = rcl_publisher_fini(&Publisher_PID, &node);
    //ret += rcl_publisher_fini(...);
    if(ret != RCL_RET_OK )
        ERR("Destory_MicroRos_Publish return error(%d)...\n",ret);
    
    ret = rclc_executor_remove_timer(&executor,&timer_200HZ);
    if(ret != RCL_RET_OK )
        ERR("Destory_MicroRos_Timer return error(%d)...\n",ret);
}
void Micro_ROS::Destory_MicroRos_Subscribe()
{
    INFO("Start destory microROS Subscribe\n");

    rcl_ret_t ret = rcl_subscription_fini(&Subscriber_PID,&node);

    if(ret != RCL_RET_OK )
        ERR("Destory_MicroRos_Subscribe return error...\n");
}
bool Micro_ROS::create_entities()
{
    INFO("Start create_entities\r\n");
    allocator = rcl_get_default_allocator();// 初始化内存分配器
    rclc_support_init(&support,0,NULL,&allocator);//创建初始化选项
    rclc_node_init_default(&node,Node_Name.c_str(),"",&support);// 创建节点 hello_microros
    Init_MicroROS_Publish_Subscribe();
    rclc_executor_init(&executor,&support.context,micro_ros_callback_num,&allocator);// 创建执行器
    
    Create_MicroRos_Timer();//创建定时器
    Create_MicroRos_Subscribe();//创建订阅
    return true;
}
void Micro_ROS::destroy_entities()
{
    rcl_ret_t ret = 0;
    ERR("Start destroy_entities\r\n");
    rmw_context_t * rmw_context = rcl_context_get_rmw_context(&support.context);
    (void) rmw_uros_set_context_entity_destroy_session_timeout(rmw_context, 0);
    //rcl_publisher_fini(&publisher, &node);
    Destory_MicroRos_Publish();
    Destory_MicroRos_Subscribe();
    ret += rclc_executor_fini(&executor);
    ret += rcl_node_fini(&node);
    ret += rclc_support_fini(&support);
    if(RCL_RET_OK != ret)
    {
        ERR("rclc_executor_fini return error..,\n");
    }
}
/*
*Reconnection function refer websit:
*[https://github.com/micro-ROS/micro_ros_arduino/tree/humble/examples/micro-ros_reconnection_example]
*/
states Micro_ROS::check_microROS_agency()
{
    switch (state) {
        case WAITING_AGENT:
            INFO("[Micro_ROS State]:WAITING_AGENT Ret:%d\n",rmw_uros_ping_agent(100, 1));
            EXECUTE_EVERY_N_MS(500, state = ((RMW_RET_OK == rmw_uros_ping_agent(1000, 5)) ? AGENT_AVAILABLE : WAITING_AGENT););
            rclc_sleep_ms(500);
            break;
        case AGENT_AVAILABLE:
            INFO("[Micro_ROS State]:AGENT_AVAILABLE\n");
            state = (true == create_entities()) ? AGENT_CONNECTED : WAITING_AGENT;
            if (state == WAITING_AGENT) {
                destroy_entities();
            }else
            {
                INFO("[Micro_ROS State]:Success Create Micro Ros Node\n");
            }
            break;
        case AGENT_CONNECTED:
            EXECUTE_EVERY_N_MS(200, state = (RMW_RET_OK == rmw_uros_ping_agent(100, 1)) ? AGENT_CONNECTED : AGENT_DISCONNECTED;);
            if (state == AGENT_CONNECTED) {
                rclc_executor_spin_some(&executor, RCL_MS_TO_NS(100));
            }
            break;
        case AGENT_DISCONNECTED:
            ERR("[Micro_ROS State]:AGENT_DISCONNECTED\n");
            destroy_entities();
            state = WAITING_AGENT;
            break;
        default:
            break;
    }
  return state;
}

void Micro_ROS::Update(u_int16_t ms)
{
    //rclc_executor_spin_some(&executor,RCL_MS_TO_NS(ms));
     /*Step1:Check and wait agncy exist + Step2 Create entities + Step3:Ros spin*/
    check_microROS_agency();
}
void Micro_ROS::micros_task(void)
{
    INFO("Success create microROS thread on core:%d\n",xPortGetCoreID());
    INIT_TASK_TIME(MICRO_ROS_UPDATE);
    this->init();
    while(1)
    {
        this->Update(100);
        WAIT_TASK_TIME();
    }
}
void Micro_ROS::Micro_ROS_Start()
{
    
    fp.fun_in_class = &Micro_ROS::micros_task;
    //xTaskCreatePinnedToCore(fp.fun_in_c,"microROS_task",4096,this,0,NULL,0);
    xTaskCreate(fp.fun_in_c,"microROS_task",4096,this,0,NULL);
}
/*Static function*/
void Micro_ROS::micros_callback_subscription(const void *msgin)
{
    const std_msgs__msg__Float32 *msg = (const std_msgs__msg__Float32 *)msgin;
    INFO("Recv microROS msg:%f\n",msg->data);
}



