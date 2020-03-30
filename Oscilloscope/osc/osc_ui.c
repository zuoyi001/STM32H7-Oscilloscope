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
#include "string.h"
#include "math.h"
#include "display_dev.h"
int sce_cof(void);
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("osc_ui",osc_create_ui,sce_cof,0,1);
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
widget_def btn[6];

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
	/* calculate main size */
  osc_calculate_main_size(dev,&win_main,osc_main_draw,0);
	/* create the group */
	osc_calculate_sg_size(dev,&win_group0,&win_group1,&win_group2,&win_group3,osc_group_draw);	
	/* 480 * 272 do not create the follow mesure group */
	if( dev->width > 480 && dev->width > 272 )
	{
		/* create the group */
		win_group4.msg.x = 1;
		win_group4.msg.y = dev->height - 62 - 6 - 22*2 - 4 - 1;
		win_group4.msg.x_size = win_group1.msg.x_size;
		win_group4.msg.y_size = 62;
		win_group4.dev = dev;
		//win_group4.msg.wflags = GUI_HIDE;
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
		//win_group5.msg.wflags = GUI_HIDE;
		/* set callback */
		win_group5.draw = osc_group_draw;
		/* create */
		gui_win_creater(&win_group5);			
	}
	/* create the menu win ui */
  osc_calculate_menu_size(dev,&win_menu,osc_menu_win_draw,0);
	/* btn */
  osc_calculate_btn_size(dev,&win_menu,btn,sizeof(btn) / sizeof(btn[0]));
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

draw_area_def * get_draw_area_msg(void);
void gui_test(void);
	
FOS_TSK_REGISTER(gui_test,PRIORITY_0,100);

volatile unsigned int cnt_p = 0;

extern unsigned short gram[800*480*3];

unsigned short * osc_arr = &gram[800*480];

int LCD_DrawLine_ili(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned int color,gui_dev_def * dev,unsigned int got)
{
 unsigned short t;
 static int len_pixel = 0;
 int xerr=0,yerr=0,delta_x,delta_y,distance;
 int incx,incy,uRow,uCol;
 delta_x=x2-x1;
 delta_y=y2-y1;
 uRow=x1;
 uCol=y1;
 if(delta_x>0)incx=1;
 else if(delta_x==0)incx=0;
 else {incx=-1;delta_x=-delta_x;}
 if(delta_y>0)incy=1;
 else if(delta_y==0)incy=0;
 else{incy=-1;delta_y=-delta_y;} 
 if( delta_x>delta_y)distance=delta_x;
 else distance=delta_y; 

 len_pixel = 0;

 /*------------------*/
 unsigned char to = got % 5;
 /*------------------*/
 
 for(t=0;t<=distance+1;t++ )
 {  
   
	volatile unsigned short dtes = osc_arr[uRow + uCol*dev->width];
	 
	if( osc_arr[uRow + uCol*dev->width] & 0x8000 )
	{
		continue;
	}
	 
	if( osc_arr[uRow + uCol*dev->width] & 0x001f )
	{
		osc_arr[uRow + uCol*dev->width] |= (1<<to);
		
		if( osc_arr[uRow + uCol*dev->width] & (1<<to) )
		{
			
		}
	}
	else
	{
    dev->set_point(uRow,uCol,RGB(255,255,7));
		osc_arr[uRow + uCol*dev->width] |= (1<<to);
		
		dtes = osc_arr[uRow + uCol*dev->width];
  }
	
  len_pixel++;

  xerr+=delta_x ; 
  yerr+=delta_y ; 
  if(xerr>distance) 
  { 
   xerr-=distance; 
   uRow+=incx; 
  } 
  if(yerr>distance) 
  { 
   yerr-=distance; 
   uCol+=incy; 
  } 
 }  

 return len_pixel;
}


int LCD_DrawLine_cleat(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned int color,gui_dev_def * dev,unsigned int got)
{
 unsigned short t;
 static int len_pixel = 0;
 int xerr=0,yerr=0,delta_x,delta_y,distance;
 int incx,incy,uRow,uCol;
 delta_x=x2-x1;
 delta_y=y2-y1;
 uRow=x1;
 uCol=y1;
 if(delta_x>0)incx=1;
 else if(delta_x==0)incx=0;
 else {incx=-1;delta_x=-delta_x;}
 if(delta_y>0)incy=1;
 else if(delta_y==0)incy=0;
 else{incy=-1;delta_y=-delta_y;} 
 if( delta_x>delta_y)distance=delta_x;
 else distance=delta_y; 

 len_pixel = 0;

 /*------------------*/
 unsigned char to = got % 5;
 /*------------------*/
 
 for(t=0;t<=distance+1;t++ )
 {  

	 
	 volatile unsigned short dtes = osc_arr[uRow + uCol*dev->width];

	if( osc_arr[uRow + uCol*dev->width] & 0x8000 )
	{
		continue;
	}	 
	 
	if( osc_arr[uRow + uCol*dev->width] & 0x001f )
	{
		osc_arr[uRow + uCol*dev->width] &=~ (1<<to);
		
		if( !(osc_arr[uRow + uCol*dev->width] & 0x1F) )
		{
			if( osc_arr[uRow + uCol*dev->width] & 0x0400 )
			{
				dev->set_point(uRow,uCol,RGB(7,3,7));
			}
			else
			{
				dev->set_point(uRow,uCol,RGB(199, 195, 199));
			}
		}
		else
		{
			//dev->set_point(uRow,uCol,RGB(0, 255, 0));
		}
	}
	else
	{
//    dev->set_point(uRow,uCol,color);
//		osc_arr[uRow + uCol*dev->width] |= (1<<to);
		//dev->set_point(uRow,uCol,RGB(0, 0, 255));
  }
	
	osc_arr[uRow + uCol*dev->width] &=~ (1<<to);
	
  len_pixel++;

  xerr+=delta_x ; 
  yerr+=delta_y ; 
  if(xerr>distance) 
  { 
   xerr-=distance; 
   uRow+=incx; 
  } 
  if(yerr>distance) 
  { 
   yerr-=distance; 
   uCol+=incy; 
  } 
 }  

 return len_pixel;
}

static unsigned short color_cnt = 10000;

draw_area_def * msg_area;


double parse = 0.0;
void create_osc_grid_status(void);

int sce_cof(void)
{
	create_osc_grid_status();
	return 0;
}

unsigned short line_show[5][800];

int create_sin_lines(unsigned short * dft)
{

	#if 0
	
	msg_area = get_draw_area_msg();
	
  /* draw other grid data */
	double sin_x = 0;
	unsigned short line_t = 0xffff;
	/* sinx */
	for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal ; i ++ )
	{
	
	 double te = sin( sin_x + parse );
		
   short tm = (short)( te * msg_area->pixel_vertiacl * msg_area->num_vertical / 1 ) + msg_area->pixel_vertiacl * msg_area->num_vertical / 2;
		
		
		tm += (i%2)?10:(-10);
//	 double tce = cos( sin_x );
//		
//   short tcm = (short)( tce * EM * (VERTICAL_GRID_TOTAL / 4) ) + EM * (VERTICAL_GRID_TOTAL / 2) + TOP_REMAIN_PIXEL + 1 + EM*LITTLE_GRIG_NUM;	 
		
//		dev->set_point( msg_area->start_pos_x + i , msg_area->start_pos_y + tm , RGB(255,255,7));
 
		if( line_t != 0xffff )
		{
			LCD_DrawLine_ili(msg_area->start_pos_x + i,msg_area->start_pos_y + tm , msg_area->start_pos_x + i - 1 ,msg_area->start_pos_y + line_t,color_cnt++,dev,gor);
		}
    
		line_t = tm;
//		dev->set_point( LEFT_REMAIN_PIXEL + 1 + i , tcm , RGB(7,227,231));
		
		sin_x += (6.28) / (double)(msg_area->pixel_horizontal * msg_area->num_horizontal) * 3;
		parse -= 0.0001;
	}
	
	return 0;
#else
	msg_area = get_draw_area_msg();
	double sin_x = 0;
	/* sinx */
	for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal ; i ++ )
	{
		 double te = sin( sin_x + parse );
		
     short tm = (short)( te * msg_area->pixel_vertiacl * msg_area->num_vertical / 2 ) + msg_area->pixel_vertiacl * msg_area->num_vertical / 2;

     tm += (i%2)?5:(-5);	
		
		  dft[i] = tm;
		
		  sin_x += (6.28) / (double)(msg_area->pixel_horizontal * msg_area->num_horizontal) * 3;
	}
	
	parse -= 0.6;
#endif
	
	return 0;
}


void show_line(unsigned short * line_d,unsigned short index)
{
	msg_area = get_draw_area_msg();
	
	for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - 1 ; i ++ )
	{
		LCD_DrawLine_ili(msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[i] , msg_area->start_pos_x + i + 1 ,msg_area->start_pos_y + line_d[i+1],color_cnt,dev,index);
	}
	
	color_cnt += 700;
}

void hide_line(unsigned short * line_d,unsigned short index)
{
	msg_area = get_draw_area_msg();
	
	for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - 1 ; i ++ )
	{
		LCD_DrawLine_cleat(msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[i] , msg_area->start_pos_x + i + 1 ,msg_area->start_pos_y + line_d[i+1],color_cnt++,dev,index);
	}
}

unsigned short dfefef;

void create_osc_grid_status(void)
{
	
	memset(osc_arr,0,800*480*2);
	
	for( int i = 0 ; i < dev->height ; i ++ )
	{
		for( int j = 0 ; j < dev->width ; j ++ )
		{
			if( gui_find_connect(&win_main,j,i) == 1)
			{
				osc_arr[i*dev->width + j] |= 0x8000;
			}
			else
			{
				osc_arr[i*dev->width + j] &=~ 0x8000;
			}
			
			if( gram[i*dev->width + j] == RGB(7,3,7) )
			{
				osc_arr[i*dev->width + j] |= 0x0400;
			}
			else if( gram[i*dev->width + j] == RGB(199, 195, 199) )
			{
				osc_arr[i*dev->width + j] &=~ 0x0400;
			}
			else
			{
				osc_arr[i*dev->width + j] |= 0x8000;
			}
		}
	}
}


void gui_test(void)
{
	//dev->clear_display_dev(0);
	//create_sin_lines(cnt_p);
	
	//memset(&gram[800*480],color_cnt++,800*480*2);
	
	//dev->fill_color(msg_area->start_pos_x,msg_area->start_pos_y,msg_area->stop_pos_x,msg_area->stop_pos_y,&gram[800*480]);
	
	if( cnt_p >= 2 )
	{
		hide_line(line_show[cnt_p%2],cnt_p%2);
	}
	
	create_sin_lines(line_show[cnt_p%2]);
	
	show_line(line_show[cnt_p%2],cnt_p%2);
	
	cnt_p++;
}










































