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
#include "hal_gpio.h"
#include "fos.h"
/* register a init inode */
FOS_INODE_REGISTER("hal_gpio",hal_gpio_init,0,0,0);
/* gpio_config_table */
const GPIO_CONFIG_DEF gpio_config_table[] = 
{
	/* 0 */
	{
		.capital = "RUN 3V3 POWER ENABLE",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_2,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 1 */
	{
		.capital = "MOTOR VREF ENABLE",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 2 */
	{
		.capital = "DC-DC 5V ENABLE",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_14,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 3 */
	{
		.capital = "RUN 5V ENABLE",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_13,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 4 */
	{
		.capital = "Screw LED ",
		.GPIO_GROUP = GPIOD,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 5 */
	{
		.capital = "WIFI LED",
		.GPIO_GROUP = GPIOD,
		.GPIO_PIN = GPIO_PIN_13,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 6 */
	{
		.capital = "CHARGE LED",
		.GPIO_GROUP = GPIOD,
		.GPIO_PIN = GPIO_PIN_12,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 7 */
	{
		.capital = "Left wheel speed pwm,link to TIM1_CH2,Right wheel speed pwm,link to TIM1_CH3",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_9|GPIO_PIN_10,
		.GPIO_MODE = GPIO_MODE_AF_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 8 */
	{
		.capital = "Right wheel dir",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_4,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 9 */
	{
		.capital = "Left wheel dir",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_6,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 10 */
	{
		.capital = "MAIN MOTOR PWM link to TIM8 CH1",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9,
		.GPIO_MODE = GPIO_MODE_AF_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 11 */
	{
		.capital = "FAN POWER",
		.GPIO_GROUP = GPIOD,
		.GPIO_PIN = GPIO_PIN_1,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 12 */
	{
		.capital = "TIM4_CH2 for CAP",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_0,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_NOPULL,
	},	
	/* 13 */
	{
		.capital = "ADC1->IN0~IN7",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_6|GPIO_PIN_7,
		.GPIO_MODE = GPIO_MODE_ANALOG,
		.GPIO_PULL = GPIO_NOPULL,
	},	
	/* 14 */
	{
		.capital = "ADC1->IN8~IN9",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_0|GPIO_PIN_1,
		.GPIO_MODE = GPIO_MODE_ANALOG,
		.GPIO_PULL = GPIO_NOPULL,
	},	
	/* 15 */
	{
		.capital = "ADC1->IN10~IN15",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_ANALOG,
		.GPIO_PULL = GPIO_NOPULL,
	},	
	/* 16 */
	{
		.capital = "TIM3_CH2 for CAP",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_NOPULL,
	},
};

/* v */
static int hal_gpio_init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  /* GPIO Ports Clock Enable */
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	/* init gpio one by one */
	for( int i = 0 ; i < sizeof(gpio_config_table) / sizeof(gpio_config_table[0]) ; i ++ )
	{
		/* Build initialization structure */
		GPIO_InitStruct.Pin = gpio_config_table[i].GPIO_PIN;
		GPIO_InitStruct.Mode = gpio_config_table[i].GPIO_MODE;
		GPIO_InitStruct.Pull = gpio_config_table[i].GPIO_PULL;
		GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
		/* init gpio */
		HAL_GPIO_Init((GPIO_TypeDef *)gpio_config_table[i].GPIO_GROUP , &GPIO_InitStruct);
		/* AF ? */
		if( gpio_config_table[i].GPIO_MODE != GPIO_MODE_AF_PP )
		{
			/* default status */
			HAL_GPIO_WritePin((GPIO_TypeDef *)gpio_config_table[i].GPIO_GROUP, gpio_config_table[i].GPIO_PIN, (GPIO_PinState)gpio_config_table[i].GPIO_DEFAULT);
		}
		/* OK */
	}
	
	/* return OK */
	return FS_OK;
}




















































