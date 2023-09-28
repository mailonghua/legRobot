#include "Vofa.h"
#include "HAL/HAL.h"
#if (VOFA_TRANSMISSION_TYPE==0)
void Vofa_CommunicateInit(Vofa_HandleTypedef *handle)
{
    Serial1.begin(115200);
    INFO("Init Vofa_CommunicateInit serial\n");
	return;
}
/*UART*/
void Vofa_SendDataCallBack(Vofa_HandleTypedef *handle, uint8_t *data, uint16_t length)
{
    VOFA_SEND(data,length);
}


int Vofa_GetDataCallBack(Vofa_HandleTypedef *handle)
{ 
    return  VOFA_READ();
}
#elif(VOFA_TRANSMISSION_TYPE==1)//UDP
extern Vofa_HandleTypedef vofa_handle;
AsyncUDP udp;
void onPacketCallBack(AsyncUDPPacket packet)
{
    uint8_t *rcv_data_ptr = packet.data();
    for(int i=0;i<packet.length();i++)
        VOFA::Vofa_PushReceiveData(&vofa_handle,rcv_data_ptr[i]);
}
void Vofa_CommunicateInit(Vofa_HandleTypedef *handle)
{
    INFO("[VOFA]:Init Vofa_CommunicateInit UDP LocalPort(%u),RemotePort(%u)\n",VOFA_UDP_LOCAL_PORT,VOFA_UDP_REMOTE_PORT);
	while (!udp.listen(VOFA_UDP_LOCAL_PORT)); //等待udp监听设置成功
    INFOLN("[VOFA]:Listen udp port success...");
    udp.onPacket(onPacketCallBack);
    return;
}
void Vofa_SendDataCallBack(Vofa_HandleTypedef *handle,uint8_t *data, uint16_t length)
{
    udp.broadcastTo(data,length, VOFA_UDP_REMOTE_PORT);
    return;
}


int Vofa_GetDataCallBack(Vofa_HandleTypedef *handle)
{
	return  0;
}
#endif