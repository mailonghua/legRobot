#ifndef __PROTOCOL_PARSER_H__
#define __PROTOCOL_PARSER_H__
#include "HAL/HAL.h"
#include "Tools/Vofa/Vofa.h"
#include "APP/Algorithm/Algorithm_Main.h"
#include "APP/Algorithm/PID.h"
/*该类有以下功能：
    1.数据解析
    2.更新数据保存&并提供获取数据接口
    [这里要注意线程安全性]
*/
#define  DIS_EN_PID 0x00
#define  ID_PID_P  0x01
#define  ID_PID_I  0x02
#define  ID_PID_D  0x03
#define  ID_Motor_Target_Speed  0x04
#define  ID_Motor_Target_Angle  0x05


#define JUDGE_LENGTH(LENGTH,NUM_BYTE) \
 do{ \
    if(LENGTH < (3+NUM_BYTE)) \
    {\
        ERR("Recv data length too small[%d]\n",LENGTH);\
        return -1;\
    }\
 }while(0)

class ProtocolParser
{
public:
    ProtocolParser(/* args */);
    ~ProtocolParser();
    int16_t Parser_Update(void *data,uint32_t length);
    float GetInfo_Inner_pid_p();
    float GetInfo_Inner_pid_i();
    float GetInfo_Inner_pid_d();
    float GetInfo_motor_target_speed();
    float GetInfo_motor_target_angle();
    //get api data
private:    
    typedef struct 
    {
        float pid_p;
        float pid_i;
        float pid_d;
        float motor_target_speed;
        float motor_target_angle;
    }PARA_INFO;

    PARA_INFO Para_Info;
};



#endif