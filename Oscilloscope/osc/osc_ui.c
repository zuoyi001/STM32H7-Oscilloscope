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
window_def win_group0;
window_def win_group1;
window_def win_group2;
window_def win_group3;
window_def win_group4;
window_def win_group5;
window_def win_menu;
/* gui dev */
static gui_dev_def * dev;
#if 0
void draw_hz(const char * hzd,unsigned short x,unsigned short y);
const char * find_hz(char * da);
#endif
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
	/* create the group */
	win_group0.msg.x = 1;
	win_group0.msg.y = dev->height - 22 - 4;
	win_group0.msg.x_end = dev->width - 3;
	win_group0.msg.y_end = 22;
	/* set callback */
	win_group0.msg.draw = osc_group_draw;
	/* create */
  gui_win_creater(&win_group0);
	/* create the group */
	win_group1.msg.x = 1;
	win_group1.msg.y = dev->height - 22*2 - 4 - 1;
	win_group1.msg.x_end = (unsigned short)(((float)347 / ( float)790) * (dev->width -10));
	win_group1.msg.y_end = 22;
	/* set callback */
	win_group1.msg.draw = osc_group_draw;
	/* create */
	gui_win_creater(&win_group1);	
	/* create the group */
	win_group2.msg.x = win_group1.msg.x_end + 4;
	win_group2.msg.y = dev->height - 22*2 - 4 - 1;
	win_group2.msg.x_end = (unsigned short)(((float)202 / ( float)790) * (dev->width - 10));
	win_group2.msg.y_end = 22;
	/* set callback */
	win_group2.msg.draw = osc_group_draw;
	/* create */
	gui_win_creater(&win_group2);		
	/* create the group */
	win_group3.msg.x = win_group2.msg.x + win_group2.msg.x_end + 4;
	win_group3.msg.y = dev->height - 22*2 - 4 - 1;
	win_group3.msg.x_end = (unsigned short)(((float)241 / ( float)790) * (dev->width - 10) );
	win_group3.msg.y_end = 22;
	/* set callback */
	win_group3.msg.draw = osc_group_draw;
	/* create */
	gui_win_creater(&win_group3);		
	/* 480 * 272 do not create the follow mesure group */
	if( dev->width > 480 && dev->width > 272 )
	{
		/* create the group */
		win_group4.msg.x = 1;
		win_group4.msg.y = dev->height - 62 - 6 - 22*2 - 4 - 1;
		win_group4.msg.x_end = win_group1.msg.x_end;
		win_group4.msg.y_end = 62;
		/* set callback */
		win_group4.msg.draw = osc_group_draw;
		/* create */
		gui_win_creater(&win_group4);		
		/* create the group */
		win_group5.msg.x = win_group2.msg.x;
		win_group5.msg.y = dev->height - 62 - 6 - 22*2 - 4 - 1;
		win_group5.msg.x_end = win_group1.msg.x_end;
		win_group5.msg.y_end = 62;
		/* set callback */
		win_group5.msg.draw = osc_group_draw;
		/* create */
		gui_win_creater(&win_group5);			
	}
	/* create the menu win ui */
	win_menu.msg.x_end = (unsigned short)(((float)107 / (float)800)*dev->width);
	win_menu.msg.y_end = dev->height - 22*2 - 1 - 1 - 4;
	win_menu.msg.x = dev->width - win_menu.msg.x_end;
	win_menu.msg.y = 0;
	/* set callback */
	win_menu.msg.draw = osc_menu_win_draw;
	/* create */
	gui_win_creater(&win_menu);			
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
	/* end if data */
}
/* draw group */
static void osc_group_draw(struct gui_info *info)
{
	draw_group_win(dev,info);
}
/* static draw menu win */
static void osc_menu_win_draw(struct gui_info *info)
{
	draw_menu_win(dev,info);
}
#if 0
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
#endif













































