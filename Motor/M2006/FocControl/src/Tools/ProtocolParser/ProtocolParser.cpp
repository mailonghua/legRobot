#include "ProtocolParser.h"
/*
    数据格式：
    FLOAT 01 AF FA
    其中
    01:指令
    AF FA：代表指令尾部标识
*/
static const uint8_t cmdTail[] = VOFA_CMD_TAIL;

ProtocolParser::ProtocolParser(/* args */):Para_Info({0})
{
}

ProtocolParser::~ProtocolParser()
{
}
int16_t ProtocolParser::Parser_Update(void *data,uint32_t length)
{
    const uint8_t * const ptr_data = (uint8_t*)data;
    uint16_t cmd_end_id = 0;

    if((ptr_data == NULL)||(length == 0))
    {
        return -1;
    }
    //查找当前是否有匹配的结尾字符
    for(int i=1;i<length;i++)
    {
        if((ptr_data[i-1] == cmdTail[0])&&(ptr_data[i] == cmdTail[1]))
        {
            cmd_end_id = i-1;
            break;
        }
        else
            cmd_end_id = 0;
    }
    if(cmd_end_id ==0)
    {
        return -1;
    }
    //JUDGE_LENGTH(length,sizeof(float));
    const float  * const Data_f32 =  (float *)&ptr_data[cmd_end_id-1-sizeof(float)];
    PID* const updateParamObj = reinterpret_cast<PID*>(ALGORITHM::GetCurrentPidObject());
    switch (ptr_data[cmd_end_id-1])
    {
    case ID_PID_P:
            Para_Info.pid_p = *Data_f32;
            updateParamObj->set_pid_para_kp(Para_Info.pid_p);
            // INFO("PID_P %f\n",*Data_f32);
            break;
    case ID_PID_I:
            Para_Info.pid_i = *Data_f32;
            updateParamObj->set_pid_para_ki(Para_Info.pid_i);
            // INFO("PID_I %f\n",*Data_f32);
            break;
    case ID_PID_D:
            Para_Info.pid_d = *Data_f32;
            updateParamObj->set_pid_para_kd(Para_Info.pid_d);
            // INFO("PID_D %f\n",*Data_f32);
            break;
    case ID_Motor_Target_Speed:
            Para_Info.motor_target_speed = *Data_f32;
            ALGORITHM::Set_Target_Speed(Para_Info.motor_target_speed);
            // INFO("Motor_Target_Speed %f\n",*Data_f32);
            break;
    case ID_Motor_Target_Angle:
            Para_Info.motor_target_angle = *Data_f32;
            ALGORITHM::Set_Target_Angle(Para_Info.motor_target_angle);
            INFO("Motor_Target_Angle %f\n",*Data_f32);
            break;
    case DIS_EN_PID:
            ALGORITHM::EnAndDisPidFunc();
            break;
    
    default:
            ERR("Not match command..please check\n");
            return -1;
        break;
    }
    return 0;

}
float ProtocolParser::GetInfo_Inner_pid_p()
{
    return Para_Info.pid_p;
}
float ProtocolParser::GetInfo_Inner_pid_i()
{
    return Para_Info.pid_i;
}
float ProtocolParser::GetInfo_Inner_pid_d()
{
    return Para_Info.pid_d;
}
float ProtocolParser::GetInfo_motor_target_speed()
{
    return Para_Info.motor_target_speed;
}
float ProtocolParser::GetInfo_motor_target_angle()
{
    return Para_Info.motor_target_angle;
}