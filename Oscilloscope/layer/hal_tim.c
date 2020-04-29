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
#include "hal_tim.h"
#include "fos.h"
#include "string.h"
/* enable or disable the test pwm */
#define DEBUG_PWM   (1)
/* register a init inode */
FOS_INODE_REGISTER("hal_gpio",hal_tim_init,0,0,14);
/* static tim handle */
static TIM_HandleTypeDef TIM_Handle; 
/* init base and open as default */
static int hal_tim_init(void)
{
	/* enable the clk */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_TIM1_CLK_ENABLE();
  /* define a struction */
	GPIO_InitTypeDef GPIO_Handle;
	TIM_OC_InitTypeDef TIM_OC_Handle;
	/* clear data */
	memset(&TIM_OC_Handle,0,sizeof(TIM_OC_Handle));
	/* define a struction */
	GPIO_Handle.Pin = GPIO_PIN_8;
	GPIO_Handle.Mode = GPIO_MODE_AF_PP;
	GPIO_Handle.Pull = GPIO_PULLUP;
	GPIO_Handle.Speed = GPIO_SPEED_HIGH;
	GPIO_Handle.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOA, &GPIO_Handle);
  /* time INIT */
	TIM_Handle.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	TIM_Handle.Instance = TIM1; 
	TIM_Handle.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM_Handle.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM_Handle.Init.Period = 1; 
	TIM_Handle.Init.Prescaler = 600 - 1;//5; //500us
	HAL_TIM_PWM_Init(&TIM_Handle);
  /* PWM init */
	TIM_OC_Handle.OCMode = TIM_OCMODE_PWM1; 
	TIM_OC_Handle.OCPolarity = TIM_OCPOLARITY_HIGH; 
	TIM_OC_Handle.Pulse = 1;
	/* CHANNEL init */
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle, &TIM_OC_Handle, TIM_CHANNEL_1); 
  /* start */
	HAL_TIM_PWM_Start(&TIM_Handle, TIM_CHANNEL_1);
#if DEBUG_PWM
  hal_test_init();
#endif
	/* return */
  return FS_OK;
}
#if DEBUG_PWM
/* test pwm */
static void hal_test_init(void)
{	
	/* define a struction */
	TIM_HandleTypeDef TIM_Handle_TIM9;
  TIM_OC_InitTypeDef TIM_OC_Handle;
	GPIO_InitTypeDef GPIO_Handle;
	/* enable the clk */
	__HAL_RCC_GPIOE_CLK_ENABLE();
	__HAL_RCC_TIM9_CLK_ENABLE();
	/* clear data */
	memset(&TIM_Handle_TIM9,0,sizeof(TIM_Handle_TIM9));
	memset(&TIM_OC_Handle,0,sizeof(TIM_OC_Handle));
  /* define a struction */
	GPIO_Handle.Pin = GPIO_PIN_5; 
	GPIO_Handle.Mode = GPIO_MODE_AF_PP;
	GPIO_Handle.Pull = GPIO_PULLUP;
	GPIO_Handle.Speed = GPIO_SPEED_HIGH;
	GPIO_Handle.Alternate = GPIO_AF3_TIM9;
	/* GPIO */
	HAL_GPIO_Init(GPIOE, &GPIO_Handle);			
	/* tim Init */
	TIM_Handle_TIM9.Channel = HAL_TIM_ACTIVE_CHANNEL_1;
	TIM_Handle_TIM9.Instance = TIM9; 
	TIM_Handle_TIM9.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1; 
	TIM_Handle_TIM9.Init.CounterMode = TIM_COUNTERMODE_UP; 
	TIM_Handle_TIM9.Init.Period = 99; 
	TIM_Handle_TIM9.Init.Prescaler = 89; //1K
	/* init */
	HAL_TIM_PWM_Init(&TIM_Handle_TIM9);
   /* PWM init */
	TIM_OC_Handle.OCMode = TIM_OCMODE_PWM1; 
	TIM_OC_Handle.OCPolarity = TIM_OCPOLARITY_HIGH; 
	TIM_OC_Handle.Pulse = 49;
	/* CHANNEL init */
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle_TIM9, &TIM_OC_Handle, TIM_CHANNEL_1); 
  /* start */
	HAL_TIM_PWM_Start(&TIM_Handle_TIM9, TIM_CHANNEL_1);	
}
#endif
/* void stop the pwm clock */
void hal_pwm_stop(void)
{
	HAL_TIM_PWM_Stop(&TIM_Handle, TIM_CHANNEL_1);
}
/* start the pwm clock */
void hal_pwm_start(void)
{
	HAL_TIM_PWM_Start(&TIM_Handle, TIM_CHANNEL_1);
}
/* void set pwm freq */
void hal_tim_psc(unsigned short psc)
{
	TIM1->PSC = psc - 1;
}






















