#ifndef _TOOLS__H__
#define _TOOLS__H__
// #include "FireUart/protocol.h"
#include "Vofa/Vofa.h"
#include "ProtocolParser/ProtocolParser.h"
namespace TOOL
{
    void Tools_Init();
    void Tools_Update();
    void Example_Show();
    void SendUIData(float *data, uint16_t num);
}
#endif