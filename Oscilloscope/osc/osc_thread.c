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
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "hal_gpio.h"
#include "hal_tim.h"
#include "osc_api.h"
#include "osc_cfg.h"
#include "math.h"
#include "string.h"
#include "hal_exit.h"
#include "osc_line.h"
#include "osc_menu.h"
#include "osc_ui.h"
/* functions declare */
static void osc_thread(void);
static int osc_thead_init(void);
/* Includes ------------------------------------------------------------------*/
FOS_TSK_REGISTER(osc_thread,PRIORITY_IDLE,100); /* gui detecter task run as idle */
FOS_INODE_REGISTER("osc_thread",0,osc_thead_init,0,1);
/* buffer */
static unsigned char clock_sta = 0;
/* gui dev */
static gui_dev_def * dev;
/* cnt p */
static unsigned int cnt_p = 0;
/* line_buffer */
static unsigned short line_buffer_ch1[2][800];
static unsigned short line_buffer_ch2[2][800];
/* zoom */
unsigned short line_zoom[2];
/* time sw */
const osc_time_def * osc_time_sw;
/* run msg */
osc_run_msg_def * runmsg;
/* upload default setting */
static int osc_thead_init(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* set init param */
	osc_rot_set(OSC_TIME_ROT,2);
	osc_rot_set(OSC_VOL_OFFSET_SCALE,200);
	osc_rot_set(OSC_VOL_SCALE,8);
	osc_rot_set(OSC_TRIG_SCALE,200);
	/* for test */
	osc_voltage_output(1870,2000,270,20);//1870,2000,0,270
	/* get run msg for osc */
	runmsg = get_run_msg();
	/* return as usual */
	return FS_OK;
}
/* delay for yuhui */
static void delay_us_yo(unsigned int t)
{
	t *=100;
	
	while(t--);
}
/* gui task */
static void osc_thread(void)
{
	/* thread */
  osc_offset_scale_thread(0);
	osc_trig_scale_thread(0);
	osc_vol_scale_thread(0);
	/* get scan time */
	osc_time_sw = osc_scan_thread();
	/* single thread */
	osc_single_thread();
	/* nothing to do */
	if( hal_read_gpio(FIFO_FULL0) != 0 )
	{
		return; // not use to deal
	}
	/* ok we've some data , stop the clock first */
	osc_stop_adc_clock();
	/* disable the tr */
	osc_fifo_clock(0);
	/* read data from fifo */
	osc_read_fifo_data(clock_sta);
	/* transfor data */
	int ret = osc_trig_read(line_buffer_ch1[cnt_p%2],line_buffer_ch2[cnt_p%2],runmsg->trig_type,runmsg->trig_source,clock_sta);
	/* single mode */
	if( (runmsg->trig_mode == RUN_TRIG_SINGLE && ret == FS_OK) || 
		   runmsg->trig_mode == RUN_TRIG_AUTO || 
	    (runmsg->trig_mode == RUN_TRIG_NORMAL && ret == FS_OK )
	  )
	{
		/* get zoom buffer */
		line_zoom[cnt_p%2] = osc_time_sw->osc_zoom_factor;
		/* ch1 enable */
		if( runmsg->chn_enable[0] == 0 ) // enable
		{
			/* show line ch1 */
			osc_create_lines(dev,line_buffer_ch1[cnt_p%2],0,cnt_p%2,0,line_zoom[cnt_p%2]);
		}
		/* ch2 enable */
		if( runmsg->chn_enable[1] == 0 ) // enable
		{		
			/* show line ch2 */
		  osc_create_lines(dev,line_buffer_ch2[cnt_p%2],0,cnt_p%2,1,line_zoom[cnt_p%2]);
		}
		/* clear the ch lines */
		if( cnt_p >= 1  )
		{
			/* hold on a while */
			delay_us_yo(20);
			/* select one group */
			if( cnt_p % 2 )
			{
				/* ch1 enable */
				if( runmsg->chn_enable[0] == 0 ) // enable
				{				
					osc_create_lines(dev,line_buffer_ch1[0],1,0,0,line_zoom[0]);
				}
				/* ch2 enable */
				if( runmsg->chn_enable[1] == 0 ) // enable
				{			
					osc_create_lines(dev,line_buffer_ch2[0],1,0,1,line_zoom[0]);
				}
			}
			else
			{
				/* ch1 enable */
				if( runmsg->chn_enable[0] == 0 ) // enable
				{								
					osc_create_lines(dev,line_buffer_ch1[1],1,1,0,line_zoom[1]);
				}
				/* ch2 enable */
				if( runmsg->chn_enable[1] == 0 ) // enable
				{					
					osc_create_lines(dev,line_buffer_ch2[1],1,1,1,line_zoom[1]);
				}
			}
		}
		/* trig check */
		if( runmsg->trig_mode == RUN_TRIG_SINGLE )
		{
			/* get trig res */
			if( ret == FS_OK )
			{
				/* set stop */
				osc_ui_set_trig_text("stop  ");
				/* revert trig mode */
				runmsg->trig_mode = runmsg->backup_trig_mode;					
				/* stop here */
				while(runmsg->run_mode);			
				/* end of single */
			}
		}
		/* stop or single */
		if( runmsg->run_mode == RUN_STOP_MODE )
		{
			/* set stop */
			osc_ui_set_trig_text("stop  ");
			/* stop here */
			while(runmsg->run_mode);
		}		
		/* incremer */
		cnt_p++;
  }
	/* read next seq */
	clock_sta = hal_read_gpio(DIO_CLOCK_STA) ? 1 : 0;
	/* enable tr */
	osc_fifo_clock(1);
	/* restart pwm */
	osc_start_adc_clock(osc_time_sw->osc_clock_ex);//for test select the inter clock
	
}
/* void line clear */
void osc_clear_all_lines(void)
{
	osc_create_lines(dev,line_buffer_ch1[0],1,0,0,line_zoom[0]);
	osc_create_lines(dev,line_buffer_ch2[0],1,0,1,line_zoom[0]);	
	osc_create_lines(dev,line_buffer_ch1[1],1,1,0,line_zoom[1]);
	osc_create_lines(dev,line_buffer_ch2[1],1,1,1,line_zoom[1]);
}



































