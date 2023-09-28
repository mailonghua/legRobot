#include <Arduino.h>
#include "HAL/HAL.h"
#include "microROS/microROS.h"
#include "HAL/HAL_Def.h"
// #include "APP/UI/UI_Main.h"
#include "APP/Algorithm/Algorithm_Main.h"
#include "Tools/Tools.h"
static Micro_ROS microROSNode("Leg_Robot_microROS");
portTickType xLastWakeTime;
const portTickType xFrequency = pdMS_TO_TICKS(MAIN_LOOP_UPDATE);
void setup() {
  Serial.begin(115200);/*调试串口||UI界面*/
  INFO("Start init...\r\n");
  HAL::HAL_Init();
  TOOL::Tools_Init();/*Display UI Init*/
  //microROSNode.Micro_ROS_Start(); /*init and start microros thread*/
  ALGORITHM::Algorithm_Init();/*init and start algorithm thread*/
}

void loop() {
  HAL::HAL_Update();
  TOOL::Tools_Update();
  vTaskDelayUntil(&xLastWakeTime, xFrequency);
}