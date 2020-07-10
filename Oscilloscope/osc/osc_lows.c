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
#include "main.h"
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
/* declares */
static int osc_lows_init(void);
static int osc_lows_config(void);
/* thread */
FOS_INODE_REGISTER("osc_lows",osc_lows_init,osc_lows_config,0,9);
/* buffer */
unsigned char clock_sta_2 = 0;
static unsigned char ch1_no[2],ch2_no[2];
/* set */
static gui_dev_def * dev;
/* area */
const draw_area_def * area_lows;
/* run msg */
static osc_run_msg_def * runmsg;
/* static switch case data */
static unsigned char step = 1;
static unsigned short last_ch[2] = {0xffff,0xffff};
static unsigned short pixel_pos = 0;
static unsigned char win_clear_flag = 0;
/* line buffer */
static unsigned short line_buffer_ch[2][800];
/* osc set low */
static int osc_lows_init(void)
{
	return FS_OK;
}
/* staitc */
static int osc_lows_config(void)
{
	/* gui dev get */
	dev = get_gui_dev();
	/* get run msg for osc */
	runmsg = get_run_msg();
	/* get draw area */
	area_lows = get_draw_area_msg();		
	/* return OK */
	return FS_OK;	
}
/* thread */
void osc_low_speed_thread(void)
{
	static unsigned int ddfsc_half_tus,diffus,cnt = 0;
	
	ddfsc_half_tus = hal_sys_time_us();
	#if 1
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_SET);
	
	/* check */
	if( step == 0 )
	{
		/* read data from fifo */
	  /* read data from fifo */
		osc_read_fifo_data(clock_sta_2,1);
		/* transfor data */
		osc_read_fifo_noload(ch1_no,ch2_no,2);
		/* get io sta */
		/* set the fifo */
		osc_fifo_reset();		
		/* set lines */
		signed char use_ch[2];
		unsigned short ch_m[2];
		/* set index */
		unsigned char index = /*(clock_sta_2 == 1) ? ( pixel_pos % 2 ) :*/ ( ( pixel_pos + 1 ) % 2 );
		/* get data */
		use_ch[0] = (signed char)ch1_no[index];
		use_ch[1] = (signed char)ch2_no[index];
	  /* chang to lcd */
		ch_m[0] = area_lows->total_pixel_v - (float)(use_ch[0] + 128) / 255.0f * area_lows->total_pixel_v;
		ch_m[1] = area_lows->total_pixel_v - (float)(use_ch[1] + 128) / 255.0f * area_lows->total_pixel_v;	
		/* set lines */
		if( last_ch[0] != 0xffff && last_ch[1] != 0xffff )
		{
//			/* check clear */
//			if( win_clear_flag == 1 )
//			{
//				for( int i = 0 ; i < 75 ; i ++ )
//				{
//					osc_draw_lines(dev,area_lows->start_pos_x + pixel_pos,area_lows->start_pos_y + line_buffer_ch[0][pixel_pos],area_lows->start_pos_x + pixel_pos + 1,area_lows->start_pos_y + line_buffer_ch[0][pixel_pos+1] , 1 , 0 , 0 ); 
//					osc_draw_lines(dev,area_lows->start_pos_x + pixel_pos,area_lows->start_pos_y + line_buffer_ch[1][pixel_pos],area_lows->start_pos_x + pixel_pos + 1,area_lows->start_pos_y + line_buffer_ch[1][pixel_pos+1] , 1 , 1 , 0 ); 
//				}
//			}
			/* show lines */
			osc_draw_lines(dev,area_lows->start_pos_x + pixel_pos,area_lows->start_pos_y + last_ch[0],area_lows->start_pos_x + pixel_pos + 1,area_lows->start_pos_y + ch_m[0] , 0 , 0 , 0 ); 
			osc_draw_lines(dev,area_lows->start_pos_x + pixel_pos,area_lows->start_pos_y + last_ch[1],area_lows->start_pos_x + pixel_pos + 1,area_lows->start_pos_y + ch_m[1] , 0 , 1 , 0 ); 
			/* save data */
			line_buffer_ch[0][pixel_pos] = last_ch[0];
			line_buffer_ch[1][pixel_pos] = last_ch[1];
		}
		/* increame */
		pixel_pos ++ ;
		/* check */
		if( pixel_pos >= area_lows->total_pixel_h )
		{
			pixel_pos = 0;
			win_clear_flag = 1;
			
			cnt++;
		}
		/* reset */
		last_ch[0] = ch_m[0];
		last_ch[1] = ch_m[1];
		/* set to low */
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_SET);//set to low
	}
	else
	{
		HAL_GPIO_WritePin(GPIOE,GPIO_PIN_9,GPIO_PIN_RESET);//set to high
	}
	/* step */
	step ++ ;
	/* */
	step %= 2;
	#endif
	//HAL_GPIO_TogglePin(GPIOC,GPIO_PIN_4);
	
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_4,GPIO_PIN_RESET);
	
	diffus = hal_sys_time_us() - ddfsc_half_tus;
	

}

















































