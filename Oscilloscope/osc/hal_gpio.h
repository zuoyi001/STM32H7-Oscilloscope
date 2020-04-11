/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__	

#define GPIO_DEFAULT_HIGH   (1)
#define GPIO_DEFAULT_LOW    (0)
/* typedef gpio config table */

typedef struct{
	char * capital;
	void * GPIO_GROUP;
	unsigned int GPIO_PIN;
	unsigned int GPIO_MODE;
	unsigned int GPIO_PULL;
	unsigned int GPIO_DEFAULT;
}GPIO_CONFIG_DEF;

/* function declears */
static int hal_gpio_init(void);
/* end of files */
#endif


