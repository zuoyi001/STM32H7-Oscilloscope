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
#include "stdafx.h"
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
/* btn */
widget_def btn_0;
widget_def btn_1;
widget_def btn_2;
widget_def btn_3;
widget_def btn_4;
/* ch1 and ch2 icon */
widget_def ch1_icon;
widget_def ch2_icon;
widget_def ch3_icon;

widget_def ch4_icon;
widget_def ch5_icon;
widget_def ch6_icon;

widget_def ch7_icon;
widget_def ch8_icon;
/* gui dev */
static gui_dev_def * dev;
#if 0
void draw_hz(const char * hzd,unsigned short x,unsigned short y);
const char * find_hz(char * da);
#endif
/* osc create ui */
int osc_create_ui(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* create the win */
	win_main.msg.x = 0;
	win_main.msg.y = 0;
	win_main.msg.x_size = dev->width;
	win_main.msg.y_size = dev->height;
	win_main.dev = dev;
	/* set callback */
	win_main.draw = osc_main_draw;
	/*--------------*/
	gui_win_creater(&win_main);
	/* create the group */
	win_group0.msg.x = 1;
	win_group0.msg.y = dev->height - 22 - 4;
	win_group0.msg.x_size = dev->width - 3;
	win_group0.msg.y_size = 22;
	win_group0.dev = dev;
	/* set callback */
	win_group0.draw = osc_group_draw;
	/* create */
    gui_win_creater(&win_group0);
	/* create the group */
	win_group1.msg.x = 1;
	win_group1.msg.y = dev->height - 22*2 - 4 - 1;
	win_group1.msg.x_size = (unsigned short)(((float)347 / ( float)790) * (dev->width -10));
	win_group1.msg.y_size = 22;
	win_group1.dev = dev;
	/* set callback */
	win_group1.draw = osc_group_draw;
	/* create */
	gui_win_creater(&win_group1);	
	/* create the group */
	win_group2.msg.x = win_group1.msg.x_size + 4;
	win_group2.msg.y = dev->height - 22*2 - 4 - 1;
	win_group2.msg.x_size = (unsigned short)(((float)202 / ( float)790) * (dev->width - 10));
	win_group2.msg.y_size = 22;
	win_group2.dev = dev;
	/* set callback */
	win_group2.draw = osc_group_draw;
	/* create */
	gui_win_creater(&win_group2);		
	/* create the group */
	win_group3.msg.x = win_group2.msg.x + win_group2.msg.x_size + 4;
	win_group3.msg.y = dev->height - 22*2 - 4 - 1;
	win_group3.msg.x_size = (unsigned short)(((float)241 / ( float)790) * (dev->width - 10) );
	win_group3.msg.y_size = 22;
	win_group3.dev = dev;
	/* set callback */
	win_group3.draw = osc_group_draw;
	/* create */
	gui_win_creater(&win_group3);		
	/* 480 * 272 do not create the follow mesure group */
	if( dev->width > 480 && dev->width > 272 )
	{
		/* create the group */
		win_group4.msg.x = 1;
		win_group4.msg.y = dev->height - 62 - 6 - 22*2 - 4 - 1;
		win_group4.msg.x_size = win_group1.msg.x_size;
		win_group4.msg.y_size = 62;
		win_group4.dev = dev;
		/* set callback */
		win_group4.draw = osc_group_draw;
		/* create */
		gui_win_creater(&win_group4);		
		/* create the group */
		win_group5.msg.x = win_group2.msg.x;
		win_group5.msg.y = dev->height - 62 - 6 - 22*2 - 4 - 1;
		win_group5.msg.x_size = win_group1.msg.x_size;
		win_group5.msg.y_size = 62;
		win_group5.dev = dev;
		/* set callback */
		win_group5.draw = osc_group_draw;
		/* create */
		gui_win_creater(&win_group5);			
	}
	/* create the menu win ui */
	win_menu.msg.x_size = (unsigned short)(((float)107 / (float)800)*dev->width);
	win_menu.msg.y_size = dev->height - 22*2 - 1 - 1 - 4;
	win_menu.msg.x = dev->width - win_menu.msg.x_size;
	win_menu.msg.y = 0;
	win_menu.dev = dev;
	/* set callback */
	win_menu.draw = osc_menu_win_draw;
	/* create */
	gui_win_creater(&win_menu);			
	/* button ypos */
	unsigned short btn_tpos = 0;
	/* create btn */
	btn_0.msg.x = 5;
	btn_0.msg.y = 24;
	btn_0.msg.x_size = 102;
	btn_0.msg.y_size = 79;
	btn_0.dev = dev;
	btn_0.draw = osc_create_button;
	btn_0.parent = &win_menu;
	/* create */
	gui_widget_creater(&btn_0);
	/* create btn 1 */
	btn_tpos = (btn_0.msg.y_size + 2 )*1 + btn_0.msg.y;
	btn_1.msg.x = 5;
	btn_1.msg.y = btn_tpos;
	btn_1.msg.x_size = 102;
	btn_1.msg.y_size = 79;
	btn_1.dev = dev;
	btn_1.draw = osc_create_button;
	btn_1.parent = &win_menu;
	/* create */
	gui_widget_creater(&btn_1);
	/* btn2 */
    btn_tpos = (btn_0.msg.y_size + 2 )*2 + btn_0.msg.y;
	btn_2.msg.x = 5;
	btn_2.msg.y = btn_tpos;
	btn_2.msg.x_size = 102;
	btn_2.msg.y_size = 79;
	btn_2.dev = dev;
	btn_2.draw = osc_create_button;
	btn_2.parent = &win_menu;
	/* create */
	gui_widget_creater(&btn_2);
	/* btn3 */
    btn_tpos = (btn_0.msg.y_size + 2 )*3 + btn_0.msg.y;
	btn_3.msg.x = 5;
	btn_3.msg.y = btn_tpos;
	btn_3.msg.x_size = 102;
	btn_3.msg.y_size = 79;
	btn_3.dev = dev;
	btn_3.draw = osc_create_button;
	btn_3.parent = &win_menu;
	/* create */
	gui_widget_creater(&btn_3);
	/* btn4 */
    btn_tpos = (btn_0.msg.y_size + 2 )*4 + btn_0.msg.y;
	btn_4.msg.x = 5;
	btn_4.msg.y = btn_tpos;
	btn_4.msg.x_size = 102;
	btn_4.msg.y_size = 79;
	btn_4.dev = dev;
	btn_4.draw = osc_create_button;
	btn_4.parent = &win_menu;
	/* create */
	gui_widget_creater(&btn_4);
	/* create icon ch1 */
	ch1_icon.msg.x = 6;
	ch1_icon.msg.y = 3;
    ch1_icon.dev = dev;
	ch1_icon.draw = osc_create_ch1_micon;
	ch1_icon.parent = &win_group4;

	gui_widget_creater(&ch1_icon);

	ch2_icon.msg.x = 6;
	ch2_icon.msg.y = 3 + 16 + 4;
    ch2_icon.dev = dev;
	ch2_icon.draw = osc_create_ch1_micon;
	ch2_icon.parent = &win_group4;

	gui_widget_creater(&ch2_icon);
	
	ch3_icon.msg.x = 6;
	ch3_icon.msg.y = 3 + 32 + 8;
    ch3_icon.dev = dev;
	ch3_icon.draw = osc_create_ch1_micon;
	ch3_icon.parent = &win_group4;

	gui_widget_creater(&ch3_icon);

	memcpy(&ch4_icon,&ch1_icon,sizeof(ch1_icon));
	memcpy(&ch5_icon,&ch2_icon,sizeof(ch2_icon));
	memcpy(&ch6_icon,&ch3_icon,sizeof(ch3_icon));

	ch4_icon.draw = osc_create_ch2_micon;
	ch5_icon.draw = osc_create_ch2_micon;
	ch6_icon.draw = osc_create_ch2_micon;
	ch4_icon.parent = &win_group5;
	ch5_icon.parent = &win_group5;
	ch6_icon.parent = &win_group5;
	ch4_icon.peer_linker = 0;
	ch5_icon.peer_linker = 0;
	ch6_icon.peer_linker = 0;
	/* create */
	gui_widget_creater(&ch4_icon);
	gui_widget_creater(&ch5_icon);
	gui_widget_creater(&ch6_icon);
	/* create */
	ch7_icon.msg.x = 8;
	ch7_icon.msg.y = 3;
	ch7_icon.dev = dev;
	ch7_icon.draw = osc_create_ch1_micon;
	ch7_icon.parent = &win_group1;
	gui_widget_creater(&ch7_icon);

	ch8_icon.msg.x = 8 + 160;
	ch8_icon.msg.y = 3;
	ch8_icon.dev = dev;
	ch8_icon.draw = osc_create_ch2_micon;
	ch8_icon.parent = &win_group1;
	gui_widget_creater(&ch8_icon);
	/* return */
	return FS_OK;
}
/* draw function */
static void osc_main_draw(window_def * win)
{
	/* create lisn */
	create_grid_data(win->dev);
	/* end if data */
}
/* draw group */
static void osc_group_draw(window_def * win)
{
	draw_group_win(win->dev,&win->msg);
}
/* static draw menu win */
static void osc_menu_win_draw(window_def * win)
{
	draw_menu_win(win->dev,&win->msg);
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













































