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

#ifndef __OSC_MENU_H__
#define __OSC_MENU_H__
/* Includes ------------------------------------------------------------------*/
/* typedef global settings */
typedef struct
{
	/* trig type */
	unsigned char trig_type;//falling or rising edge
	/* trig mode */
	unsigned char trig_mode;// normal . auto . single . smart
	/* trig source */
	unsigned char trig_source;// ch1 or ch2
	/* trig vol ch1 ch2 */
	unsigned short trig_vol_level_ch[2];
	/* vol scale ch1 and ch2 */
	unsigned short vol_scale_ch[2];
	/* vol offset sacle , ch1 ch2*/
	unsigned short vol_offset_scale[2];
	/* run mode */
	unsigned short run_mode ; // run . stop . single
	/* coupling */
	unsigned short coupling_type[2] ; // 0 is dc 1 is ac
	/* ch enable */
	unsigned char chn_enable[2] ;//0 is open.1 is close
	/* chn focus */
	unsigned char chn_focus;
	/*-----------*/
}osc_run_msg_def;
/* function */
static void osc_menu_thread(void);
static int osc_menu_heep(void);
static int osc_menu_init(void);
static void key_runstop_callback(void);
static void key_auto_callback(void);
static void key_measure_callback(void);
static void key_single_callback(void);
static void key_menu_callback(void);
static void check_COM2_event(unsigned char * sta_buf,unsigned int len);
static void menu_update(void);
static void menu_hide_auto(void);
static void osc_menu_hide_thread(void);

#endif
























