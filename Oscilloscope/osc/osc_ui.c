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

/* Includes ------------------------------------------------------------------*/
#include "fos.h"
#include "osc.h"
#include "osc_ui.h"
#include "gui.h"
#include "hz_out.h"
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("osc_ui",osc_create_ui,0,0,1);
/* define win */
window_def win_main;
/* gui dev */
static gui_dev_def * dev;

void draw_hz(const char * hzd,unsigned short x,unsigned short y);
const char * find_hz(char * da);
/* osc create ui */
static int osc_create_ui(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* create the win */
	win_main.msg.x = 0;
	win_main.msg.y = 0;
	win_main.msg.x_end = dev->width;
	win_main.msg.y_end = dev->height;
	/* set callback */
	win_main.msg.draw = osc_main_draw;
	/*--------------*/
	gui_win_creater(&win_main);
	/* return */
	return FS_OK;
}
/* draw function */
static void osc_main_draw(struct gui_info *info)
{
	/* ignore the complier warnning */
	(void)info;
	/* create lisn */
	create_grid_data(dev);
	unsigned short cnt = 0;
	
  const char * pod ;
	
	pod	= find_hz("Å£");
	draw_hz(pod,cnt,0);
	cnt += 16;

	pod	= find_hz("¸ç");
	draw_hz(pod,cnt,0);
	cnt += 16;

	pod	= find_hz("Éµ");
	draw_hz(pod,cnt,0);
	cnt += 16;

	pod	= find_hz("±Æ");
	draw_hz(pod,cnt,0);
	cnt += 16;	
	
	pod	= find_hz("Ìì");
	draw_hz(pod,cnt,0);
	cnt += 16;

	pod	= find_hz("²Å");
	draw_hz(pod,cnt,0);
	cnt += 16;
	
	pod	= find_hz("ºÇ");
	draw_hz(pod,cnt,0);
	cnt += 16;
	
	pod	= find_hz("Å¶");
	draw_hz(pod,cnt,0);
	cnt += 16;	
	
	pod	= find_hz("·å");
	draw_hz(pod,cnt,0);
	cnt += 16;	
	/* end if data */
}

/* test */
const unsigned char hz_dd[32] = {
	0x20,
0x20,
0x20,
0x20,
0x24,
0x20,
0x26,
0x20,
0x25,
0xe0,
0xfc,
0xa0,
0x24,
0x20,
0x27,
0xff,
0x24,
0x20,
0xfc,
0x60,
0x24,
0xa0,
0x27,
0x20,
0x25,
0x20,
0x24,
0x20,
0x20,
0x20,
0x00,
0x00,
};

const char * find_hz(char * da)
{
	
	unsigned short * tm = (unsigned short *)da;
	
	for( int i = 0 ; i < sizeof(HZ_INDEX) / sizeof(HZ_INDEX[0]) ; i ++ )
	{
		if( tm[0] == HZ_INDEX[i] )
		{
			return (const char *)HZ_DATAX[i];
		}
	}
	
	return 0;
}

void draw_hz(const char * hzd,unsigned short x,unsigned short y)
{
	int ti = 0;
   for( int i = 0 ; i < 32 ; i ++ )
	{
		for( int j = 0 ; j < 8 ; j ++ )
		{
			if( ( hzd[i] << j ) & 0x80 )
			{
				dev->set_point(x+ti/16,y+ti%16,0xFFFF);
			}
			ti++;
		}
	}
}














































