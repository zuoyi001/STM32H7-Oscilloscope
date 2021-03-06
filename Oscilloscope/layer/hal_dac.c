/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "fos.h"
#include "hal_dac.h"
/* register a init inode */
FOS_INODE_REGISTER("hal_dac",hal_dac_init,0,0,9);
/* Handle */
DAC_HandleTypeDef DAC1_Handler;
/* hal init */
static int hal_dac_init(void)
{
	/* enable clock */
	GPIO_InitTypeDef GPIO_Initure;

	__HAL_RCC_DAC12_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	GPIO_Initure.Pin=GPIO_PIN_5;
	GPIO_Initure.Mode=GPIO_MODE_ANALOG; 
	GPIO_Initure.Pull=GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);
	/* init dac */
	DAC_ChannelConfTypeDef DACCH1_Config;
	/* struct */
	DAC1_Handler.Instance = DAC1;
	HAL_DAC_Init(&DAC1_Handler);
	/* init */
	DACCH1_Config.DAC_Trigger = DAC_TRIGGER_NONE;
	DACCH1_Config.DAC_OutputBuffer = DAC_OUTPUTBUFFER_DISABLE;
	HAL_DAC_ConfigChannel(&DAC1_Handler,&DACCH1_Config,DAC_CHANNEL_2);
  /* start DAC */
	HAL_DAC_Start(&DAC1_Handler,DAC_CHANNEL_2);
#if 0 // test	
	osc_set_dac(0);
#endif
	/* return */
	return FS_OK;
}
/* set value */
void osc_set_dac(signed short mv)
{
	 /* channel */
	 unsigned short vo = ( mv + 10 + 2500 ) / 2;
	 /* transfer data */
	 unsigned short out = (unsigned short )((float)(vo+60) / 2500.0f * 4095.0f);
	 /* out data */
   HAL_DAC_SetValue(&DAC1_Handler,DAC_CHANNEL_2,DAC_ALIGN_12B_R,out);
}




















