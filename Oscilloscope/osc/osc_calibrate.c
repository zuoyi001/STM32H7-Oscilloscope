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
#include "fos.h"
#include "osc_ui.h"
#include "osc_calibrate.h"
#include "osc_cfg.h"
#include "osc_menu.h"
#include "osc_api.h"
#include "stdio.h"
/* Private includes ----------------------------------------------------------*/
static unsigned char osc_calibrate_mode = 0;
/* static gain or offset */
static signed short gain_cal = 0,offset_cal = 0;
/* enter to calbrate mode */
void key_swi_long_click(void)
{
	/* enter or exit */
	if( osc_calibrate_mode == 0 )
	{
		/* enter */
		osc_calibrate_mode = 1;
		/* set notice */
		osc_ui_tips_str_dir("进入校准模式");
		/* clear */
		gain_cal = 0;
		offset_cal = 0;
	}
	else
	{
		/* enter */
		osc_calibrate_mode = 0;
		/* set notice */
		osc_ui_tips_str_dir("退出校准模式");		
	}
}
/* get calbrate mode */
unsigned char osc_get_calibrate_sta(void)
{
	return osc_calibrate_mode;
}
/* show data */
static void osc_calibrate_str(unsigned char chn,unsigned short gain,signed short offset)
{
	/* stata */
	static char buf[32];
	/* create */
	sprintf(buf,"ch%d,gain:%d,offset:%d   ",chn + 1,gain,offset);
	/* show */
	osc_ui_tips_str_dir(buf);
}
/* void osc calibrate app */
void osc_calibrate_api(unsigned char inx,signed short zm)
{
	/* osc vol scale */
	const osc_vol_scale_def * ovs;
	/* get run msg for osc */
	osc_run_msg_def * runmsg = get_run_msg();
	/* get vol scale */
	ovs = osc_get_vol_scale(runmsg->chn_focus);
	/* unsigned chn */
	unsigned char chn = runmsg->chn_focus;
	/* gain or offset */
	if( inx == OSC_VOL_OFFSET_SCALE )
	{
		/* gain */
		gain_cal += zm;
		/* set new gain */
		osc_vol_dac(chn,ovs->gain_dac[chn] + gain_cal,ovs->gain_offset_ch[chn] + offset_cal);
	}
	else if( inx == OSC_TRIG_SCALE )
	{
		/* offset */
		offset_cal += zm;
		/* set new gain */
		osc_vol_dac(chn,ovs->gain_dac[chn] + gain_cal,ovs->gain_offset_ch[chn] + offset_cal);		
	}
	else
	{
		/* can not supply */
	}
	/* show */
	osc_calibrate_str(chn,ovs->gain_dac[chn] + gain_cal,ovs->gain_offset_ch[chn] + offset_cal);
}
/* saveing data */
 







































