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
#define DEBUG_PWM   (0)
/* register a init inode */
FOS_INODE_REGISTER("hal_gpio",hal_tim_init,0,0,14);
/* static tim handle */
static TIM_HandleTypeDef TIM_Handle; 
static TIM_HandleTypeDef htim5;
static TIM_HandleTypeDef htim2;
static TIM_HandleTypeDef htim3;
static void mx_time5_init(void);
/* static void get tim3 cnt */
static unsigned int tim3_cnt_gt = 0;
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
	GPIO_Handle.Pin = GPIO_PIN_9;
	GPIO_Handle.Mode = GPIO_MODE_AF_PP;
	GPIO_Handle.Pull = GPIO_PULLUP;
	GPIO_Handle.Speed = GPIO_SPEED_HIGH;
	GPIO_Handle.Alternate = GPIO_AF1_TIM1;
	HAL_GPIO_Init(GPIOE, &GPIO_Handle);
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
	/* 32bit tim init */
	mx_time5_init();
	/* tim2 */
	hal_tim2_cap_init();
	hal_tim3_cap_init();
	/*----------------*/
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
	TIM_Handle_TIM9.Init.Period = 999; 
	TIM_Handle_TIM9.Init.Prescaler = 89; //1K
	/* init */
	HAL_TIM_PWM_Init(&TIM_Handle_TIM9);
   /* PWM init */
	TIM_OC_Handle.OCMode = TIM_OCMODE_PWM1; 
	TIM_OC_Handle.OCPolarity = TIM_OCPOLARITY_HIGH; 
	TIM_OC_Handle.Pulse = 499;
	/* CHANNEL init */
	HAL_TIM_PWM_ConfigChannel(&TIM_Handle_TIM9, &TIM_OC_Handle, TIM_CHANNEL_1); 
  /* start */
	HAL_TIM_PWM_Start(&TIM_Handle_TIM9, TIM_CHANNEL_1);	
}
#endif
static void mx_time5_init(void)
{
  /* USER CODE BEGIN TIM7_Init 0 */
  __HAL_RCC_TIM5_CLK_ENABLE();
  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 180 - 1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 0xFFFFFFFF;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */
  HAL_TIM_Base_Start(&htim5);
  /* USER CODE END TIM7_Init 2 */
}
/* TIM2 CAP */
static void hal_tim2_cap_init(void)
{
	/* USER CODE BEGIN TIM2_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE END TIM2_MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_TIM2_CLK_ENABLE();
  /* Peripheral clock enable */
	__HAL_RCC_GPIOA_CLK_ENABLE();
	/**TIM2 GPIO Configuration    
	PA0     ------> TIM2_ETR 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);	

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 0xFFFFFFFF;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */
  HAL_TIM_Base_Start(&htim2);
  /* USER CODE END TIM2_Init 2 */	
}
/* TIM3 CAP */
static void hal_tim3_cap_init(void)
{
	/* USER CODE BEGIN TIM3_MspInit 0 */
	GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE END TIM3_MspInit 0 */
	/* Peripheral clock enable */
	__HAL_RCC_TIM3_CLK_ENABLE();

	__HAL_RCC_GPIOD_CLK_ENABLE();
	/**TIM3 GPIO Configuration    
	PD2     ------> TIM3_ETR 
	*/
	GPIO_InitStruct.Pin = GPIO_PIN_2;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF2_TIM3;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* TIM3 interrupt Init */
	HAL_NVIC_SetPriority(TIM3_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(TIM3_IRQn);
	/* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 0;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 0xffff;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_ETRMODE2;
  sClockSourceConfig.ClockPolarity = TIM_CLOCKPOLARITY_NONINVERTED;
  sClockSourceConfig.ClockPrescaler = TIM_CLOCKPRESCALER_DIV1;
  sClockSourceConfig.ClockFilter = 0;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
	HAL_TIM_Base_Start_IT(&htim3);
  /* USER CODE END TIM3_Init 2 */

	/* USER CODE END TIM3_MspInit 1 */	
}
/* cnt ++ */
void TIM3_IRQHandler(void)
{
  /* USER CODE BEGIN TIM3_IRQn 0 */
  tim3_cnt_gt ++ ;
  /* USER CODE END TIM3_IRQn 0 */
  HAL_TIM_IRQHandler(&htim3);
  /* USER CODE BEGIN TIM3_IRQn 1 */

  /* USER CODE END TIM3_IRQn 1 */
}
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
void hal_tim_psc(unsigned int psc)
{
	if( psc >= 65000 )
	{
		TIM1->ARR = 3;
		TIM1->CCR1 = 2;
		TIM1->PSC = psc / 4 - 1;		
	}
	else
	{
		TIM1->ARR = 1;
		TIM1->CCR1 = 1;
		TIM1->PSC = psc - 1;
	}
}
/* get sys time */
unsigned int hal_sys_time_us(void)
{
	return TIM5->CNT;
}
/* get tim2 cnt */
unsigned int hal_tim2_cnt(void)
{
	return TIM2->CNT;
}
/* get tim3 cnt */
unsigned int hal_tim3_cnt(void)
{
	return (tim3_cnt_gt << 16) + TIM3->CNT;
}





















