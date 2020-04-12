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

#ifndef __OOSC_API_H__
#define __OOSC_API_H__
/* Includes ------------------------------------------------------------------*/
/* FIFO DEEP */
#define FIFO_DEEP (4096)
/* Private includes ----------------------------------------------------------*/

void osc_stop_clock(void);
void osc_start_clock(unsigned char internal);
void osc_read_fifo_data(unsigned short * ch1,unsigned short * ch1n,unsigned short * ch2,unsigned short * ch2n,unsigned short fifo_deep);
void osc_trig_read(unsigned short * ch1,unsigned short * ch1n,unsigned short * ch2,unsigned short * ch2n , signed char * ch1_o,signed char * ch2_o,int trig_type,int trig_source,int tflag);
void osc_create_analog_data(signed char * ch1_o,signed char * ch2_o,unsigned short * ch1_m,unsigned short * ch2_m );

#endif










