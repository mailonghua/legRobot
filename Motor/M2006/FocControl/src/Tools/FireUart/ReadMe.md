# 移植野火多功能调试助手--PID调试修改
## 修改发送函数
set_computer_value->FIRE_SEND(buffer,num)

##接收函數
void protocol_data_recv(uint8_t *data, uint16_t data_len);
该函数用来接收数据,将接收到的数据放在缓冲区里面
解析函数调用后会不断轮训缓冲区,若是曼如了长度就开始解析
[在STM32中该函数放在了串口接收中断函数中,如：
	void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
#ifdef DEBUG_SOFTWARE_PID
	uint8_t dr = __HAL_UART_FLUSH_DRREGISTER(&huart3);
	protocol_data_recv(&dr, 1);//一个一个字符进行填充
#endif
  /* USER CODE END USART3_IRQn 0 */
  HAL_UART_IRQHandler(&huart3);
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}
]
在这里使用主循环进行
##循环调用发送函数
void sync_computer_current_speed(void)

## 接收到的数据在哪里获取
receiving_process->case SET_P_I_D_CMD{...}
receiving_process->case SET_TARGET_CMD{...}
receiving_process->case SET_TARGET_CMD{...}

# 下位机发送给上位机的函数
Algorithm_Main.cpp
void ALGORITHM::Algorithm_Send_UI_Display()；

