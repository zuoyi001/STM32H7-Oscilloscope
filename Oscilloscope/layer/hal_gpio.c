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
FOS_INODE_REGISTER("hal_gpio",hal_gpio_init,0,0,14);
/* gpio_config_table */
const GPIO_CONFIG_DEF gpio_config_table[] = 
{
	/* 0 */
	{
		.capital = "FIFO Data from D0 to D7 ",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 1 */
	{
		.capital = "FIFO FULL 0 ",
		.GPIO_GROUP = GPIOF,
		.GPIO_PIN = GPIO_PIN_9,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 2 */
	{
		.capital = "FIFO FULL 1 ",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_0,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 3 */
	{
		.capital = "FIFO FULL 2 ",
		.GPIO_GROUP = GPIOH,
		.GPIO_PIN = GPIO_PIN_8,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 4 */
	{
		.capital = "FIFO FULL 3 ",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_12,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 5 */
	{
		.capital = "KEY RUN/STOP",
		.GPIO_GROUP = GPIOF,
		.GPIO_PIN = GPIO_PIN_7,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 6 */
	{
		.capital = "KEY AUTO",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_2,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 7 */
	{
		.capital = "KEY MEASURE",
		.GPIO_GROUP = GPIOH,
		.GPIO_PIN = GPIO_PIN_2,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 8 */
	{
		.capital = "KEY SINGLE",
		.GPIO_GROUP = GPIOH,
		.GPIO_PIN = GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},		
	/* 9 */
	{
		.capital = "KEY MENU",
		.GPIO_GROUP = GPIOH,
		.GPIO_PIN = GPIO_PIN_4,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 10 */
	{
		.capital = "KEY DC/AC",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_6,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},		
	/* 11 */
	{
		.capital = "KEY CHNN",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 12 */
	{
		.capital = "KEY SWI",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_4,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 13 */
	{
		.capital = "KEY CAL",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 14 */
	{
		.capital = "KEY MATH",
		.GPIO_GROUP = GPIOE,
		.GPIO_PIN = GPIO_PIN_2,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 15 */
	{
		.capital = "CLOCK STA",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_1,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 16 */
	{
		.capital = "FIFO RESET",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_1,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 17 */
	{
		.capital = "DIO-TR",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_7,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},
	/* 18 */
	{
		.capital = "DIO-R0",
		.GPIO_GROUP = GPIOI,
		.GPIO_PIN = GPIO_PIN_11,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 19 */
	{
		.capital = "DIO-R1",
		.GPIO_GROUP = GPIOI,
		.GPIO_PIN = GPIO_PIN_8,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 20 */
	{
		.capital = "DIO-R2",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_15,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 21 */
	{
		.capital = "DIO-R3",
		.GPIO_GROUP = GPIOG,
		.GPIO_PIN = GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 22 */
	{
		.capital = "CPC",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_0,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},		
	/* 23 */
	{
		.capital = "CH1_DC/AC",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_4,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 24 */
	{
		.capital = "CH2_DC/AC",
		.GPIO_GROUP = GPIOI,
		.GPIO_PIN = GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 25 */
	{
		.capital = "CH2 GAIN/CTRL",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_3,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 26 */
	{
		.capital = "CH1_GAIN/CTRL",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_OUTPUT_PP,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 27 */
	{
		.capital = "FIFO Data D8 ",
		.GPIO_GROUP = GPIOC,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_INPUT,
		.GPIO_PULL = GPIO_PULLUP,
		.GPIO_DEFAULT = GPIO_PIN_SET,
	},	
	/* 28.EXIT for ROT */
	{
		.capital = "ROT7",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_1,
		.GPIO_MODE = GPIO_MODE_AF_PP,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
	/* 29.EXIT for ROT */
	{
		.capital = "ROT6",
		.GPIO_GROUP = GPIOF,
		.GPIO_PIN = GPIO_PIN_6,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 30.EXIT for ROT */
	{
		.capital = "ROT3",
		.GPIO_GROUP = GPIOD,
		.GPIO_PIN = GPIO_PIN_2,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 31.EXIT for ROT */
	{
		.capital = "ROT2",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_15,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 32.EXIT for ROT */
	{
		.capital = "ROT1",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_14,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 33.EXIT for ROT */
	{
		.capital = "ROT0",
		.GPIO_GROUP = GPIOB,
		.GPIO_PIN = GPIO_PIN_13,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 34.EXIT for ROT */
	{
		.capital = "ROT4",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_4,
		.GPIO_MODE = GPIO_MODE_IT_RISING,
		.GPIO_PULL = GPIO_PULLDOWN,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},	
	/* 35.EXIT for ROT */
	{
		.capital = "ROT5",
		.GPIO_GROUP = GPIOA,
		.GPIO_PIN = GPIO_PIN_5,
		.GPIO_MODE = GPIO_MODE_ANALOG,
		.GPIO_PULL = GPIO_NOPULL,
		.GPIO_DEFAULT = GPIO_PIN_RESET,
	},
};

/* v */
static int hal_gpio_init(void)
{
	/* gpio struction */
  GPIO_InitTypeDef GPIO_InitStruct = {0};
	
  /* GPIO Ports Clock Enable */
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();	
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOI_CLK_ENABLE();
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
/* write gpio */
void hal_write_gpio(unsigned short index,unsigned short sta)
{
	/* over the gate */
	if( index > sizeof(gpio_config_table) / sizeof(gpio_config_table[0]) )
	{
		return;//can not write
	}
	/* write */
	HAL_GPIO_WritePin((GPIO_TypeDef *)gpio_config_table[index].GPIO_GROUP, gpio_config_table[index].GPIO_PIN, (GPIO_PinState)sta);
}
/* read gpio */
unsigned short hal_read_gpio(unsigned short index)
{
	/* over the gate */
	if( index > sizeof(gpio_config_table) / sizeof(gpio_config_table[0]) )
	{
		return 0xffff;//can not write
	}
  /* read */
	GPIO_TypeDef * gpio_g = (GPIO_TypeDef *)gpio_config_table[index].GPIO_GROUP;
	/* reutrn */
  return ( gpio_g->IDR & gpio_config_table[index].GPIO_PIN );
}

















































