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
#include "stdafx.h"
/* Includes ------------------------------------------------------------------*/
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "hal_gpio.h"
#include "hal_tim.h"
#include "osc_api.h"
/* functions declare */
static void osc_thread(void);
/* Includes ------------------------------------------------------------------*/
FOS_TSK_REGISTER(osc_thread,PRIORITY_IDLE,100); /* gui detecter task run as idle */
/* buffer */
static unsigned char clock_sta = 0;
/* cache data */
static unsigned short cache_fifo_ori[4][FIFO_DEEP];
static signed char cache_ch[2][FIFO_DEEP*2];
static unsigned short cache_dev[2][1366];//max supply num is 1366
/* gui task */
static void osc_thread(void)
{
	/* nothing to do */
	if( hal_read_gpio(FIFO_FULL0) != 0 )
	{
		return; // not use to deal
	}
	/* ok we've some data , stop the clock first */
	osc_stop_clock();
	/* read data from fifo */
	osc_read_fifo_data(cache_fifo_ori[0],cache_fifo_ori[1],cache_fifo_ori[2],cache_fifo_ori[3],FIFO_DEEP);
	/* transfor data */
	osc_trig_read(cache_fifo_ori[0],cache_fifo_ori[1],cache_fifo_ori[2],cache_fifo_ori[3],cache_ch[0],cache_ch[1],TRIG_MODE_RISING,TRIG_SOURCE_CH1,clock_sta);
	/* create the analog data */
	osc_create_analog_data(cache_ch[0],cache_ch[1],cache_dev[0],cache_dev[1]);
	/* read next seq */
	clock_sta = hal_read_gpio(DIO_CLOCK_STA) ? 1 : 0;
	/* restart pwm */
	osc_start_clock(0);//for test select the inter clock
}




































