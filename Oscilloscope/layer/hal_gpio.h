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
#define DIO_CLOCK_SEL       (15)
#define DIO_CLOCK_STA       (8)
#define DIO_R0              (11)
#define DIO_R1              (12)
#define DIO_R2              (13)
#define DIO_R3              (14)
#define FIFO_DATA_D8        (20)
#define FIFO_FULL0          (1)
#define FIFO_DIO_TR         (10)
#define KEY_MENU            (6)
/* ROT 7 ~ 0 */
#define DIO_ROT_TIME_UP     (21)
#define DIO_ROT_TIME_DM     (22)
#define DIO_ROT_VOL_UP      (23)
#define DIO_ROT_VOL_DM      (24)
#define DIO_ROT_TRIG_UP     (25)
#define DIO_ROT_TRIG_DM     (26)
#define DIO_ROT_HORI_UP     (27)
#define DIO_ROT_HORI_DM     (28)
/* coupling setting */
#define DIO_CH1_DCAC        (16)
#define DIO_CH2_DCAC        (17)
/* GAIN trl */
#define DIO_CH1_GAIN_CTRL   (18)
#define DIO_CH2_GAIN_CTRL   (19)
/* CD4051 ADDR */
#define DIO_CD4051_ADDR_A   (3)
#define DIO_CD4051_ADDR_B   (4)
#define DIO_CD4051_ADDR_C   (5)
/* COM data 2 */
#define DIO_CD4051_COM2     (7)
/* pwr ctrl pin */
#define DIO_PWR_CTRL        (30)
/* beep */
#define DIO_USB_S_ENABLE     (29)
/* */
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


