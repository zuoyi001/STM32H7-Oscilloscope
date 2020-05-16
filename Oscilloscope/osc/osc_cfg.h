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

#ifndef __OSC_CFG_H__
#define __OSC_CFG_H__
/* Includes ------------------------------------------------------------------*/
#define OSC_UNIT_US   (0)
#define OSC_UNIT_MS   (1)
#define OSC_UNIT_S    (2)
#define OSC_UINT_NS   (3)

#define OSC_BASE_CLOCK  (180)

/* Private includes ----------------------------------------------------------*/
/* MACROS */
#define OSC_TIME_ROT           (0)
#define OSC_VOL_SCALE          (1)
#define OSC_VOL_OFFSET_SCALE   (2)
#define OSC_TRIG_SCALE         (3)

/* osc_ time config */
typedef struct{
	/* show title */
  char * str;
	/* scan time */
  float osc_time;
	/* unit */
	unsigned int osc_unit;
	/* source inter or ex */
	unsigned int osc_clock_ex;
	/* Zoom Factor */
	unsigned int osc_zoom_factor;
	/* end if */
}osc_time_def;
/* voltage scale */
typedef struct
{
	/* show * str */
	char * str;
	/* gain dac */
	unsigned short gain_dac;
	/* gain_offset ch1*/
	signed short gain_offset_ch[2];
	/* gain_sel */
	unsigned short gain_sel;
}osc_vol_scale_def;
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

/* set scan time */
const osc_time_def * osc_scan_thread(void);
void osc_offset_scale_thread(unsigned char chn);
void osc_trig_scale_thread(unsigned char chn);
const osc_vol_scale_def * osc_vol_scale_thread(unsigned char chn);
static void osc_cfg_task(void);
#endif
























