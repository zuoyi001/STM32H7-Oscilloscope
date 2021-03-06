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

void osc_stop_adc_clock(void);
void osc_start_adc_clock(unsigned char internal);
void osc_read_fifo_data(unsigned char clock_sta,unsigned int deep);
int osc_trig_read(unsigned short * ch1_m,unsigned short * ch2_m,int trig_type,int trig_source,int tflag,unsigned int ins,unsigned int deep);
static void osc_create_analog_data(signed char * ch1_o,signed char * ch2_o,unsigned short * ch1_m,unsigned short * ch2_m ,unsigned int);
void osc_voltage_output(unsigned short a,unsigned short b,unsigned short c,unsigned short d);
void osc_fifo_clock(unsigned short sta);
unsigned short osc_read_key_menu(void);
int osc_read_rot_idle(unsigned index);
void osc_gain_ctrl(unsigned char chn , unsigned char sta);
void osc_vol_dac(unsigned char chn,unsigned short gain_dac,signed short offset_dac);
void osc_dac_offset(unsigned char chn,signed short offset_mv);
void osc_set_key_addr(unsigned char addr_cnt);
int osc_read_com2(void);
void osc_coupling_setting(unsigned char chn,unsigned char dcac);
void osc_power_en(unsigned char mode);
void osc_beep_en(unsigned char mode);
int osc_api_peek(unsigned char chn , signed char * max,signed char * min);
int osc_read_com1(void);
void osc_backlight_en(unsigned short mode);
void osc_fifo_reset(void);
void osc_read_fifo_noload(unsigned char * ch1_noload,unsigned char * ch2_noload,unsigned int len);

#endif










