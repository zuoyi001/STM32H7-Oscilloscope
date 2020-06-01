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
#include "k24c02.h"
#include "string.h"
#include "hal_exit.h"
/* Private includes ----------------------------------------------------------*/
static unsigned char osc_calibrate_mode = 0;
/* static gain or offset */
static signed short gain_cal = 0,offset_cal = 0;
/* register node */
FOS_INODE_REGISTER("calibrate",calibrete_heap_init,calibrate_config_init,0,8);
/* datd */
static eeprom_calibrate_def ep_cal_param;
/* static */
static int calibrete_heap_init(void)
{
	return FS_OK;
}
/* init param */
static int calibrate_config_init(void)
{
	/* init param */
	osc_calibrate_init();
	/* return OK */
	return FS_OK;
}
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
/* save data */
void key_math_long_click(void)
{
	/* check sta */
	if( osc_get_calibrate_sta() == 1 )
	{
		/* osc vol scale */
		const osc_vol_scale_def * ovs;
		/* get run msg for osc */
		osc_run_msg_def * runmsg = get_run_msg();
		/* unsigned chn */
		unsigned char chn = runmsg->chn_focus;		
		/* get rotation data */
	  signed short ovs_index = runmsg->vol_scale_ch[chn];		
		/* get vol scale */
		ovs = osc_get_vol_scale(runmsg->chn_focus);	
		/* save data */
		if( osc_save_cal_param(ovs_index,chn,ovs->gain_dac[chn] + gain_cal,ovs->gain_offset_ch[chn] + offset_cal) == FS_OK )
		{
			osc_ui_tips_str_dir("参数保存成功          ");		
			/* clear the gain and offset */
			gain_cal = 0;
			offset_cal = 0;
		}
		else
		{
			osc_ui_tips_str_dir("参数保存失败          ");
		}
	}
	else
	{
		/* error mode */
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
/* check xor */
static unsigned short osc_check_xor(void * src,unsigned int len)
{
	unsigned short cxor = 0;
	/* change */
	unsigned char * tm = src;
  /* cal */
  for( int i = 0 ; i < len ; i ++ )
  {
		cxor ^= tm[i];
	}		
	/* return */
	return cxor;
}
/* init param */
static int osc_calibrate_init(void)
{
	/* read from eeprom at addr is 0 */
	osc_k24C02_read(0,&ep_cal_param,sizeof(ep_cal_param));
	/* check */
	unsigned short cxor = osc_check_xor(&ep_cal_param,sizeof(ep_cal_param) - 2);
	/* check */
	if( cxor == ep_cal_param.check_xor )
	{
		/* check ok copy data */
		osc_vol_scale_def * ovsd;
		/* set data */
		for( int i = 0 ; i < 11 ; i ++ )
		{
			/* get index */
			ovsd = osc_vol_scale_s(i);
			/* set data */
			ovsd->gain_dac[0] = ep_cal_param.ep_s[i].gain_dac_e[0];
			ovsd->gain_dac[1] = ep_cal_param.ep_s[i].gain_dac_e[1];
			/* offset */
			ovsd->gain_offset_ch[0] = ep_cal_param.ep_s[i].gain_offset_ch_e[0];
			ovsd->gain_offset_ch[1] = ep_cal_param.ep_s[i].gain_offset_ch_e[1];
		}
		/* ok */
		return FS_OK;
	}
	else
	{
		/* check error */
		osc_ui_tips_str_dir("系统参数丢失，重新校准或使用默认");
		/* set default */
		osc_vol_scale_def * ovsd;
		/* set data */
		for( int i = 0 ; i < 11 ; i ++ )
		{
			/* get index */
			ovsd = osc_vol_scale_s(i);
			/* set data */
			ep_cal_param.ep_s[i].gain_dac_e[0] = ovsd->gain_dac[0];
			ep_cal_param.ep_s[i].gain_dac_e[1] = ovsd->gain_dac[1];
			/* offset */
			ep_cal_param.ep_s[i].gain_offset_ch_e[0] = ovsd->gain_offset_ch[0];
			ep_cal_param.ep_s[i].gain_offset_ch_e[1] = ovsd->gain_offset_ch[1];
		}		
		/* return */
		return FS_ERR;
	}
}
/* save data */
static int osc_save_cal_param(unsigned int index,unsigned char chn,unsigned short gain_dac,signed short offset_dac)
{
	/* set data */
	ep_cal_param.ep_s[index].gain_dac_e[chn] = gain_dac;
	ep_cal_param.ep_s[index].gain_offset_ch_e[chn] = offset_dac;
	/* check xor */
	ep_cal_param.check_xor = osc_check_xor( &ep_cal_param , sizeof(ep_cal_param) - 2 );
	/* write data */
	osc_k24c02_write(0,&ep_cal_param , sizeof(ep_cal_param));
	/* read back and check */
	eeprom_calibrate_def tmp;
	/* read */
	osc_k24C02_read(0,&tmp , sizeof(tmp));
	/* check */
	if( memcmp(&tmp,&ep_cal_param,sizeof(tmp)) == 0 )
	{
		/* set sys */
		osc_vol_scale_def * ovsd = osc_vol_scale_s(index);
		/* set */
		ovsd->gain_dac[chn] = gain_dac;
		ovsd->gain_offset_ch[chn] = offset_dac;			
		/* return OK */
		return FS_OK;
	}
	else
	{
		return FS_ERR;
	}
}







































