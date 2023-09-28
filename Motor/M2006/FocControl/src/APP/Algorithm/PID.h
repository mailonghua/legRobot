#ifndef _PID_H__
#define _PID_H__
#include "HAL/HAL.h"
#define LIMIT_MIN_MAX(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
typedef struct 
{
    float kp;
    float ki; 
    float kd;
    float i_max;
    float out_max;
    float ref;
    float fdb;
    float err[2];
    float p_out;
    float i_out;
    float d_out;
    float output;
}pid_para_t;
typedef struct 
{
    pid_para_t outer;
    pid_para_t inner;
}pid_Cascade_t;
/*
单级PID
*/
class PID
{
public:
    friend class PID_Cascade;
    PID(float kp,float ki,float kd,float i_max,float out_max)
    {
        pid_para.kp = kp;
        pid_para.ki = ki;
        pid_para.kd = kd;
        pid_para.i_max = i_max;
        pid_para.out_max = out_max;
    }
    float PID_Calculate(float ref, float fdb);
    void set_pid_para_kp(float kp)
    {
        INFO("[PID]set_pid_para_kp:%f\n",kp);
        pid_para.kp = kp;
    }
    void set_pid_para_ki(float ki)
    {
        INFO("[PID]set_pid_para_ki:%f\n",ki);
        pid_para.ki = ki;
    }
    void set_pid_para_kd(float kd)
    {
        INFO("[PID]set_pid_para_kd:%f\n",kd);
        pid_para.kd = kd;
    }
    float get_pid_para_kp()
    {
        return pid_para.kp;
    }
    float get_pid_para_ki()
    {
        return pid_para.ki;
    }
    float get_pid_para_kd()
    {
        return pid_para.kd;
    }

private:
    pid_para_t pid_para;//Pid参数


};
/*
串级PID
*/
class PID_Cascade
{
public:
    PID_Cascade(PID *Outer_Ring,PID *Inner_Ring)
    {
        pid_Cascade.outer = Outer_Ring->pid_para;//外环
        pid_Cascade.inner = Inner_Ring->pid_para;//内环
        this->Outer_Ring_Ptr    = Outer_Ring;
        this->Inner_Ring_Ptr    = Inner_Ring;
    }
    float PID_Cascade_Calculate(float angleRef,float angleFdb,float speedFdb);
private:
    pid_Cascade_t pid_Cascade;//串级PID data
    PID * Outer_Ring_Ptr;
    PID * Inner_Ring_Ptr;

};
#endif