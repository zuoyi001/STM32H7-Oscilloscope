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
#define RUN_TRIG_AUTO    (0)
#define RUN_TRIG_NORMAL  (1)
#define RUN_TRIG_SMART   (2)
#define RUN_TRIG_SINGLE  (3)
/* run mode */
#define FUN_COUNTINUE    (0)
#define RUN_STOP_MODE    (1)
/* typedef global settings */
typedef struct
{
	/* check head */
	unsigned short head;
	/* trig type */
	unsigned char trig_type;//falling or rising edge
	/* trig mode */
	unsigned char trig_mode;// auto . normal . smart . single
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
	/* backup trig mode  */
	unsigned char backup_trig_mode;
	/* user 0 */
	unsigned char user0;
	/* check head */
	unsigned short tail;	
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
osc_run_msg_def * get_run_msg(void);
void osc_clear_all_lines(void);
static void osc_signle_mode(void);
void osc_single_thread(void);
static void key_menu_Longfress_callback(void);
static void key_trig_short_click(void);
static void key_chnn_short_click(void);
static void key_swi_short_click(void);
static void key_math_short_click(void);
static void key_onoff_short_click(void);
static void key_onoff_long_click(void);

#endif
























