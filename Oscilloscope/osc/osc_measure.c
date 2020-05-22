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
#include "hal_tim.h"
#include "osc_ui.h"
#include "gui.h"
#include "string.h"
#include "math.h"
#include "osc_measure.h"
#include "stdio.h"
#include "osc_api.h"
#include "osc_cfg.h"
/* measure item enable */
static unsigned char measure_enable_ch[2][3];
/* Private includes ----------------------------------------------------------*/
FOS_TSK_REGISTER(osc_measure_thread,PRIORITY_0,1000);/* run as 1s */
FOS_INODE_REGISTER("osc_measure",osc_heep,osc_measure_init,0,15);
/* measure mearch */
const osc_measure_def osc_measure_ch[] = 
{
	/* 0 */
	{
		.capital = "      ",
		.method = 0,
	},	
	/* 1 */
	{
		.capital = "频率",
		.method = freq_measure_ch,
	},
	/* 2 */
	{
		.capital = "周期  ",
		.method = 0,
	},	
	/* 3 */
	{
		.capital = "峰峰值",
		.method = peek_measure_ch,
	},
	/* 4 */
	{
		.capital = "最大值",
		.method = 0,
	},
	/* 5 */
	{
		.capital = "最小值",
		.method = 0,
	},	
	/* 6 */
	{
		.capital = "平均值",
		.method = 0,
	},	
	/* 7 */
	{
		.capital = "占空比",
		.method = 0,
	},	
	/* 8 */
	{
		.capital = "正脉宽",
		.method = 0,
	},
	/* 9 */
	{
		.capital = "负脉宽",
		.method = 0,
	},
};
/* staic osc fix */
static int osc_heep(void)
{
	/* set default settings */
	measure_enable_ch[0][0] = 1;
	measure_enable_ch[0][1] = 3;
	measure_enable_ch[0][2] = 7;
	/* ch2 */
	measure_enable_ch[1][0] = 1;
	measure_enable_ch[1][1] = 3;
	measure_enable_ch[1][2] = 7;	
	/*----------------------*/
	return FS_OK;
}
/* oc measure init set default */
static int osc_measure_init(void)
{
	/* set measure item */
	unsigned char tch;	
	/* set ch1 */
	for( int i = 0 ; i < 3 ; i ++ )
	{
		/* set measure item */
		tch = measure_enable_ch[0][i];
		/* set capital */
		osc_ui_measure_capital(0,i,osc_measure_ch[tch].capital);
		/* clean the orignal data */
		osc_ui_measure_data(0,i,"     ");	
		/* set ch2 */
		/* set measure item */
		tch = measure_enable_ch[1][i];
		/* set capital */
		osc_ui_measure_capital(1,i,osc_measure_ch[tch].capital);
		/* clean the orignal data */
		osc_ui_measure_data(1,i,"     ");	
	}	
	/* return OK */
	return FS_OK;
}
/* update measure item */
void osc_measure_item_update(unsigned char chn,unsigned char item,unsigned char measure_item,unsigned char enable)
{
	/* chn */
	if( chn > 1 || item > 2 )
	{
		/* error chn and item */
		return;
	}
	/* item */
	if( measure_item >= sizeof(osc_measure_ch) / sizeof(osc_measure_ch[0]))
	{
		/* error item return */
		return;
	}
	/* good data */
	unsigned char tch = enable ? measure_item : 0;
	/* set measure */
	measure_enable_ch[chn][item] = tch;
	/* set capital */
	osc_ui_measure_capital(chn,item,osc_measure_ch[tch].capital);
	/* clean the orignal data */
	osc_ui_measure_data(chn,item,"     ");	
	/* end of func */
}
/* static freq mearsure */
static int freq_measure_ch(unsigned char chn , char * buf)
{
	/* set data */
	static unsigned int last_sys_time_us[2];
	static unsigned int last_tim2_cnt_pulse[2];	
	/* get time */
	unsigned int now_time = hal_sys_time_us();
	/* calbrate freq */
	unsigned int now_cnt = ( chn == 0 ) ? hal_tim2_cnt() : hal_tim3_cnt();
	/* cnt tes */
	float freq = (float)( now_cnt - last_tim2_cnt_pulse[chn] ) / ((float)( now_time - last_sys_time_us[chn] ) / 1000000) ;
	/* reset */
	last_tim2_cnt_pulse[chn] = now_cnt;
	last_sys_time_us[chn] = now_time;
	/* check */
	if( freq > 1000000 )
	{
		/* gewei */
		if( freq < 10000000 )
		{
		  sprintf(buf,"%1.3fMHz",freq / 1000000.0f);
		}
		else if( freq >= 10000000 && freq < 100000000 )
		{
			sprintf(buf,"%2.2fMHz",freq / 1000000.0f);
		}
		else
		{
			sprintf(buf,"%3.1fMHz",freq / 1000000.0f);
		}
	}
	else if( freq >= 1000 )
	{
		/* gewei */
		if( freq < 10000 )
		{
			sprintf(buf,"%1.3fKHz",freq / 1000.0f);
		}
		else if( freq >= 10000 && freq < 100000 )
		{
			sprintf(buf,"%2.2fKHz",freq / 1000.0f);
		}
		else
		{
			sprintf(buf,"%3.1fKHz",freq / 1000.0f);
		}
	}
	else
	{
		/* gewei */
		if( freq < 10 )
		{
			sprintf(buf,"%1.3fHz ",freq );
		}
		else if( freq >= 10 && freq < 100 )
		{
			sprintf(buf,"%2.2fHz ",freq );
		}
		else
		{
			sprintf(buf,"%3.1fHz ",freq );
		}			
	}
	/* get len */
	osc_set_str(buf,8);	
	/* return OK */
	return FS_OK;
}
/* p-p measure */
static int peek_measure_ch(unsigned char chn , char * buf)
{
	/* osc vol scale */
	const osc_vol_scale_def * ovs;
	/* get peek value */
	signed char max,min;
	/* get */
	int ret = osc_api_peek(chn,&max,&min);
	/* check */
	if( ret == FS_OK )
	{
		/* get vol scale */
		ovs = osc_get_vol_scale(chn);
		/* calbrate ppk */
		float peek_mv = ((float)max * 4.0f - (float)min* 4.0f) / 128.0f * ovs->mv_int;
		/* show */
		if( peek_mv >= 1000 )
		{
			/* change */
			sprintf(buf,"%2.2fV", peek_mv / 1000.0f);				
		}
		else
		{
			sprintf(buf,"%3.1fmV", peek_mv );
		}
		/* get len */
		osc_set_str(buf,7);
		/* return OK */
		return FS_OK;
	}
	/* bad data */
	return FS_ERR;
}
/* check and set str len */
static void osc_set_str(char * bdf,unsigned int limit)
{
	/* get len */
	int len = strlen(bdf);
	/* check */
	if( len < limit )
	{
		memset( bdf + len , ' ' , limit - len );
	}
	/* set tail */
	bdf[limit] = 0;	
	/* end of func */
}
/* measure thread */
static void osc_measure_thread(void)
{
	/* static buffer for measure */
	static char buf[6][16];
	/* tmp */
	unsigned char tch;
	/* start measure thread */
	for( int i = 0 ; i < 3 ; i ++ )
	{
		/* ch1 */
		tch = measure_enable_ch[0][i];
		/* ok or not */
		if( tch != 0 && osc_measure_ch[tch].method != 0 )
		{
			/* run ch1 */
			int ret = osc_measure_ch[tch].method(0,buf[i]);
			/* measure ok */
			osc_ui_measure_data(0,i,( ret == FS_OK ) ? buf[i] : "----");
			/*------------*/
		}
		/* ch2 */
		tch = measure_enable_ch[1][i];
		/* ok or not */
		if( tch != 0 && osc_measure_ch[tch].method != 0 )
		{
			/* run ch1 */
			int ret = osc_measure_ch[tch].method(1,buf[ i + 3 ]);
			/* measure ok */
			osc_ui_measure_data(1,i,( ret == FS_OK ) ? buf[ i + 3 ] : "----");
			/*------------*/
		}
	}
}






























