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

/* hal afio map */
#define FIFO_DATA           (0)
#define DIO_CLOCK_SEL       (22)
#define DIO_CLOCK_STA       (15)
#define DIO_R0              (18)
#define DIO_R1              (19)
#define DIO_R2              (20)
#define DIO_R3              (21)
#define FIFO_DATA_D8        (27)
#define FIFO_FULL0          (1)
#define FIFO_DIO_TR         (17)
#define KEY_MENU            (9)
/* ROT 7 ~ 0 */
#define DIO_ROT_TIME_UP     (28)
#define DIO_ROT_TIME_DM     (29)
#define DIO_ROT_VOL_UP      (30)
#define DIO_ROT_VOL_DM      (31)
#define DIO_ROT_TRIG_UP     (32)
#define DIO_ROT_TRIG_DM     (33)
#define DIO_ROT_HORI_UP     (34)
#define DIO_ROT_HORI_DM     (35)
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
void hal_write_gpio(unsigned short index,unsigned short sta);
unsigned short hal_read_gpio(unsigned short index);
/* end of files */
#endif


