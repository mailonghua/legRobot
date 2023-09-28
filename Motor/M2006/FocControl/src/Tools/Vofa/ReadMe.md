# VOFA
## 接收的格式
/*
    数据格式：
    FLOAT 01 AF FA
    其中
    01:指令
    AF FA：代表指令尾部标识
*/

## 通信方式
config.h
串口方式
#define VOFA_TRANSMISSION_TYPE (0)
UDP通信方式
#define VOFA_TRANSMISSION_TYPE (1)

注意：若是高频率发送数据，UDP存在数据丢失，串口不会
比如20hz发送偶发出现数据丢失，丢数据也跟当前电脑负载有关
