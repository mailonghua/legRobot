#include "PID.h"
float PID::PID_Calculate(float ref, float fdb)
{
    pid_para.ref = ref;
    pid_para.fdb = fdb;
    pid_para.err[1] = pid_para.err[0];
    pid_para.err[0] = pid_para.ref - pid_para.fdb;

    pid_para.p_out  = pid_para.kp * pid_para.err[0];
    pid_para.i_out += pid_para.ki * pid_para.err[0];
    pid_para.d_out  = pid_para.kd * (pid_para.err[0] - pid_para.err[1]);
    LIMIT_MIN_MAX(pid_para.i_out, -pid_para.i_max, pid_para.i_max);

    pid_para.output = pid_para.p_out + pid_para.i_out + pid_para.d_out;
    LIMIT_MIN_MAX(pid_para.output, -pid_para.out_max, pid_para.out_max);
    // INFO("pid_para.i_out[%f],pid_para.output[%f], ref[%f],fdb[%f],err[%f]\n",pid_para.i_out,pid_para.output,ref,fdb, pid_para.err[0]);
    return pid_para.output;
}
/*
串级PID:[目标角度]-->角度环-->速度环-->[电机电流]
*/
float PID_Cascade::PID_Cascade_Calculate(float Outer_Ring_IN,float Outer_Ring_Fdb,float Inner_Ring_Fdb)
{
    if((Outer_Ring_Ptr == NULL)||(Inner_Ring_Ptr == NULL))
    {
        ERR("PID_Cascade_Calculate error...\r\n");
        return 0;
    }
    this->Outer_Ring_Ptr->PID_Calculate(Outer_Ring_IN,Outer_Ring_Fdb);//内环计算
    this->Inner_Ring_Ptr->PID_Calculate(this->Outer_Ring_Ptr->pid_para.output,Inner_Ring_Fdb);//外环的输出作为内环的输入

    return 0;
}