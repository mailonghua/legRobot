/*
	MIT License
	Copyright (c) 2021 Jelin
*/

#include "Vofa.h"
#include <stdarg.h>
#include <stdio.h>

static const uint8_t cmdTail[] = VOFA_CMD_TAIL;
static const uint8_t justFloatTail[4] = {0x00, 0x00, 0x80, 0x7f};

void VOFA::Vofa_Init(Vofa_HandleTypedef* handle,Vofa_ModeTypeDef mode)
{
	handle->rxBuffer.rp = handle->rxBuffer.buffer;
	handle->rxBuffer.wp = handle->rxBuffer.buffer;
	handle->mode = mode;

	Vofa_CommunicateInit(handle);
}

void VOFA::Vofa_SendData(Vofa_HandleTypedef* handle,uint8_t* data,uint16_t num)
{
	Vofa_SendDataCallBack(handle, data, num);
}

void VOFA::Vofa_JustFloat(Vofa_HandleTypedef *handle,float *data, uint16_t num)
{
	Vofa_SendDataCallBack(handle, (uint8_t *)data, num * sizeof(float));
	Vofa_SendDataCallBack(handle, (uint8_t *)justFloatTail, 4);
}

void VOFA::Vofa_Printf(Vofa_HandleTypedef *handle, const char *format, ...)
{
	uint32_t n;
	va_list args;
	va_start(args, format);
	n = vsnprintf((char*)handle->txBuffer, VOFA_BUFFER_SIZE, format, args);
	Vofa_SendDataCallBack(handle, handle->txBuffer, n);
	va_end(args);
}

void VOFA::Vofa_ReceiveData(Vofa_HandleTypedef *handle)
{
	int ret = Vofa_GetDataCallBack(handle);
	if(ret < 0) return;
    uint8_t data = (uint8_t) ret;

	if (handle->rxBuffer.overflow && handle->mode == VOFA_MODE_BLOCK_IF_FIFO_FULL)
	{
		return;
	}

	*handle->rxBuffer.wp = data;
	handle->rxBuffer.wp++;

	if (handle->rxBuffer.wp == (handle->rxBuffer.buffer + VOFA_BUFFER_SIZE))
	{
		handle->rxBuffer.wp = handle->rxBuffer.buffer;
	}
	if (handle->rxBuffer.wp == handle->rxBuffer.rp)
	{
		handle->rxBuffer.overflow = true;
	}
}
void VOFA::Vofa_PushReceiveData(Vofa_HandleTypedef* handle,uint8_t data)
{
	if (handle->rxBuffer.overflow && handle->mode == VOFA_MODE_BLOCK_IF_FIFO_FULL)
	{
		return;
	}

	*handle->rxBuffer.wp = data;
	handle->rxBuffer.wp++;

	if (handle->rxBuffer.wp == (handle->rxBuffer.buffer + VOFA_BUFFER_SIZE))
	{
		handle->rxBuffer.wp = handle->rxBuffer.buffer;
	}
	if (handle->rxBuffer.wp == handle->rxBuffer.rp)
	{
		handle->rxBuffer.overflow = true;
	}
}
static uint8_t Vofa_GetByte(Vofa_HandleTypedef* handle,uint8_t* byte)
{
	if (handle->rxBuffer.rp == handle->rxBuffer.wp && !handle->rxBuffer.overflow)
	{
		return false;
	}

	if (handle->rxBuffer.overflow)
	{
		handle->rxBuffer.overflow = false;
	}

	*byte = *handle->rxBuffer.rp;
	*handle->rxBuffer.rp = 0;
	handle->rxBuffer.rp++;

	if (handle->rxBuffer.rp == (handle->rxBuffer.buffer + VOFA_BUFFER_SIZE))
	{
		handle->rxBuffer.rp = handle->rxBuffer.buffer;
	}

	return true;
}

uint16_t VOFA::Vofa_ReadCmd(Vofa_HandleTypedef* handle,uint8_t* buffer,uint16_t bufferLen)
{
	uint16_t length = 0;
	uint16_t i = 0;
	uint16_t tailCount = 0;

	for(i = 0;i<bufferLen && Vofa_GetByte(handle,&buffer[i]) && tailCount < sizeof(cmdTail);i++)
	{
		if(buffer[i] == cmdTail[tailCount])
		{
			tailCount++;
		}
		else
		{
			tailCount = 0;
		}

		length++;
	}
	return length;
}


uint16_t VOFA::Vofa_ReadLine(Vofa_HandleTypedef* handle,uint8_t* buffer,uint16_t bufferLen)
{
	uint16_t length = 0;
	uint16_t i = 0;

	for(i = 0;i < bufferLen && Vofa_GetByte(handle,&buffer[i]) && (buffer[i] != '\n');i++)
	{
		length++;
	}
	return length;
}

uint16_t VOFA::Vofa_ReadData(Vofa_HandleTypedef* handle,uint8_t* buffer,uint16_t bufferLen)
{
	uint16_t length = 0;
	uint16_t i = 0;

	for(i = 0;i < bufferLen && Vofa_GetByte(handle,&buffer[i]);i++)
	{
		length++;
		// Serial.printf("^^^0x%x^^^\n",buffer[i]);
	}
	return length;
}
uint16_t VOFA::Vofa_ReadBufferLen(Vofa_HandleTypedef* handle)
{
	if(handle->rxBuffer.wp < handle->rxBuffer.rp)
	{
		uint16_t diff_1 = (handle->rxBuffer.buffer + VOFA_BUFFER_SIZE) - handle->rxBuffer.rp + 1;//从rp到末尾
		uint16_t diff_2 = handle->rxBuffer.wp - handle->rxBuffer.buffer; 
		return(diff_1+diff_2);
	}
	return (handle->rxBuffer.wp - handle->rxBuffer.rp);
}
void VOFA::Vofa_Example(Vofa_HandleTypedef *handle)
{
	#define NUM_STEP 100
    #define AMPLITUDE 1000
    float x = 0.0;
    float y = 0.0;
    const float x_step = (2*PI)/NUM_STEP;
	float channel_data[2];
    for(int i=0;i<NUM_STEP;i++)//输出一个正弦 and ....
    {
        x = -PI + x_step*i;
        channel_data[0] = sin(x) ;
		channel_data[1] = cos(x) ;
		// channel_data[2] = tan(x);
        Vofa_JustFloat(handle, channel_data,2);
    }
}
#ifdef __GNUC__
__attribute__((weak))
void Vofa_CommunicateInit(Vofa_HandleTypedef *handle)
{
	return;
}
__attribute__((weak))
void Vofa_SendDataCallBack(Vofa_HandleTypedef *handle, uint8_t *data, uint16_t length)
{
	return;
}

__attribute__((weak))
int Vofa_GetDataCallBack(Vofa_HandleTypedef *handle)
{
	return -1;
}
#endif
