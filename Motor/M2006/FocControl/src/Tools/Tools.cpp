#include "Tools.h"
// #define TOOLS_TYPE TOOLS_FIRE
#define CMD_BUFF_LEN 64
#define MAX_CMD_LEN  7 //一条指令最大长度字节，也就是读取到这个字节才会开始解析

ProtocolParser protocolParser;
Vofa_HandleTypedef vofa_handle;
static uint8_t ReadCmdBuffer[CMD_BUFF_LEN];

/**************************************************************/
void TOOL::Tools_Init()
{
/*宏选择在config.h*/   
#ifdef ENABLE_CONFIG_GRAPH_UART
#if (DEBUG_DISPLAY_UI_TYPE != 0)/*0类型不用初始化,不存在协议*/
    
    VOFA::Vofa_Init(&vofa_handle,VOFA_MODE_BLOCK_IF_FIFO_FULL);
#endif
#endif
}
/*工具以一定频率更新函数*/
static uint16_t freq_num = 0;
static uint8_t ReadCmdBuffer_num = 0;
void TOOL::Tools_Update()
{
#ifdef ENABLE_CONFIG_GRAPH_UART
#if (DEBUG_DISPLAY_UI_TYPE != 0)
    #if (VOFA_TRANSMISSION_TYPE==0)/*只有串口模式采用轮训，UDP采用异步触发模式，不用主动轮训*/
    VOFA::Vofa_ReceiveData(&vofa_handle);//接收数据并填充缓冲区
    #endif
    if(freq_num%4 == 0)
    {
        uint16_t length = VOFA::Vofa_ReadData(&vofa_handle,&ReadCmdBuffer[ReadCmdBuffer_num],
                                              CMD_BUFF_LEN - ReadCmdBuffer_num);
        if(length == 0) return;

        ReadCmdBuffer_num += length;
        if(ReadCmdBuffer_num > CMD_BUFF_LEN)
        {
            ReadCmdBuffer_num = 0;
            ERR("ReadCmdBuffer_num > CMD_BUFF_LEN");
            return;
        }

        uint16_t ret = protocolParser.Parser_Update(ReadCmdBuffer,ReadCmdBuffer_num);
        if(ret == 0)
        {
            ReadCmdBuffer_num = 0;
            memset(ReadCmdBuffer,0,CMD_BUFF_LEN);
        }
        freq_num = 0;
    }
    freq_num ++;
#endif
#endif
}
void TOOL::Example_Show()
{
   VOFA::Vofa_Example(&vofa_handle); 
}
void TOOL::SendUIData(float *data, uint16_t num)
{
    VOFA::Vofa_JustFloat(&vofa_handle, data,num);
}
