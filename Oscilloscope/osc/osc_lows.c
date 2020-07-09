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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "hal_gpio.h"
#include "hal_tim.h"
#include "osc_api.h"
#include "osc_cfg.h"
#include "math.h"
#include "string.h"
#include "hal_exit.h"
#include "osc_line.h"
#include "osc_menu.h"
#include "osc_ui.h"
/* thread */
/* buffer */
unsigned char clock_sta_2 = 0;
static unsigned char ch1_no[2],ch2_no[2];
/* thread */
void osc_low_speed_thread(void)
{
	/* static switch case data */
	static unsigned char step = 1;
	/* check */
	if( step == 0 )
	{
		/* read data from fifo */
	  /* read data from fifo */
		osc_read_fifo_data(clock_sta_2,1);
		/* transfor data */
		osc_read_fifo_noload(ch1_no,ch2_no,2);
		/* get io sta */
    //clock_sta_2 = hal_read_gpio(DIO_CLOCK_STA) ? 1 : 0;		
		/* set the fifo */
		osc_fifo_reset();		
		/* set lines */
		
		/* set to low */
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);//set to low
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);//set to high
	}
	/* step */
	step ++ ;
	/* */
	step %= 2;
}

















































