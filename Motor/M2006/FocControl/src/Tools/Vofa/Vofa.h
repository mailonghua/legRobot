/*
	MIT License
	Copyright (c) 2021 Jelin
*/

#ifndef VOFA_H
#define VOFA_H

/* User configuration params */
#define VOFA_BUFFER_SIZE 64		// Send/Receive data buffer length
#define VOFA_CMD_TAIL {0xAF,0xFA}	// Command frame tail

#include <stdint.h>
#include <stdbool.h>
#include "HAL/HAL.h"
#include <WiFi.h>
#include <AsyncUDP.h> 

typedef struct
{
	uint8_t buffer[VOFA_BUFFER_SIZE];	//Data buffer
	uint8_t* rp;						//Read data buffer pointer
	uint8_t* wp;						//Write data buffer pointer
	bool overflow;						//FIFO overflow sign bit
}Vofa_FIFOTypeDef;

typedef enum
{
	VOFA_MODE_SKIP, 				// Do not block
	VOFA_MODE_BLOCK_IF_FIFO_FULL 	// Wait until there is space in the buffer.
}Vofa_ModeTypeDef;

typedef struct
{
	uint8_t txBuffer[VOFA_BUFFER_SIZE];
	Vofa_FIFOTypeDef rxBuffer;
	Vofa_ModeTypeDef mode;
}Vofa_HandleTypedef;
namespace VOFA
{
/* initialization/configuration functions */
void Vofa_Init(Vofa_HandleTypedef* handle,Vofa_ModeTypeDef mode);

/* send data functions */
void Vofa_SendData(Vofa_HandleTypedef* handle,uint8_t* data,uint16_t num);
void Vofa_JustFloat(Vofa_HandleTypedef* handle,float* data,uint16_t num);
void Vofa_Printf(Vofa_HandleTypedef* handle,const char* format,...);

/* receive data functions */
void Vofa_PushReceiveData(Vofa_HandleTypedef* handle,uint8_t data);
void Vofa_ReceiveData(Vofa_HandleTypedef* handle);
uint16_t Vofa_ReadCmd(Vofa_HandleTypedef* handle,uint8_t* buffer,uint16_t bufferLen);
uint16_t Vofa_ReadLine(Vofa_HandleTypedef* handle,uint8_t* buffer,uint16_t bufferLen);
uint16_t Vofa_ReadData(Vofa_HandleTypedef* handle,uint8_t* buffer,uint16_t bufferLen);
uint16_t Vofa_ReadBufferLen(Vofa_HandleTypedef* handle);

/*Example*/
void Vofa_Example(Vofa_HandleTypedef *handle);
}
/* user call back functions */
void Vofa_CommunicateInit(Vofa_HandleTypedef *handle);
void Vofa_SendDataCallBack(Vofa_HandleTypedef* handle,uint8_t* data,uint16_t length);
int Vofa_GetDataCallBack(Vofa_HandleTypedef* handle);




#endif //VOFA_H
