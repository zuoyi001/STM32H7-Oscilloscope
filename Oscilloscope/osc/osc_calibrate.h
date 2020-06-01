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

#ifndef __OSC_CALIBRATE_H__
#define __OSC_CALIBRATE_H__
/* defines */
typedef struct
{
	/* gain dac */
	unsigned short gain_dac_e[2];
	/* gain_offset ch1*/
	signed short gain_offset_ch_e[2];	
}ep_struct_def;
/* saving data */
typedef struct
{
	/* save data */
	ep_struct_def ep_s[11];//define at osc_cfg
	/* check */
	unsigned short check_xor;
	/*-------*/
}eeprom_calibrate_def;
/* Includes ------------------------------------------------------------------*/
void key_swi_long_click(void);
unsigned char osc_get_calibrate_sta(void);
void osc_calibrate_api(unsigned char inx,signed short zm);
static int osc_calibrate_init(void);
static int calibrete_heap_init(void);
static int calibrate_config_init(void);
static int osc_save_cal_param(unsigned int index,unsigned char chn,unsigned short gain_dac,signed short offset_dac);
void key_math_long_click(void);

#endif
























