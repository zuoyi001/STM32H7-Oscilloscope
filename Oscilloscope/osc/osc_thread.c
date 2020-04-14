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
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "hal_gpio.h"
#include "hal_tim.h"
#include "osc_api.h"


#include "string.h"


/* functions declare */
static void osc_thread(void);
static int osc_thead_init(void);
/* Includes ------------------------------------------------------------------*/
FOS_TSK_REGISTER(osc_thread,PRIORITY_IDLE,100); /* gui detecter task run as idle */
FOS_INODE_REGISTER("osc_thread",0,osc_thead_init,0,1);
/* buffer */
static unsigned char clock_sta = 0;
/* cache data */


unsigned short dfefef;

extern window_def win_main;

void create_osc_grid_status(void);
void show_line(unsigned short * line_d,unsigned short index,unsigned char chn );
void hide_line(unsigned short * line_d,unsigned short index,unsigned char chn );
/* gui dev */
gui_dev_def * dev;

draw_area_def * msg_area;

 static unsigned int cnt_p = 0;
	
unsigned short line_show[5][800];
unsigned short line_show2[5][800];

/* upload default setting */
static int osc_thead_init(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	
	msg_area = get_draw_area_msg();
	
	create_osc_grid_status();
	
	/* for test */
	osc_voltage_output(1870,2000,0,270);//1870,2000,0,270
	/* return as usual */
	return FS_OK;
}

void delay_us(unsigned int t)
{
	t *=100;
	
	while(t--);
}

/* gui task */
static void osc_thread(void)
{
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
	osc_trig_read(line_show[cnt_p%2],line_show2[cnt_p%2],TRIG_MODE_RISING,TRIG_SOURCE_CH1,clock_sta);
	

//  if( cnt_p >= 2  )
//	{
//		hide_line(line_show[cnt_p%2],cnt_p%2);
//		//hide_line(line_show[1],1);
//		//hide_line(line_show[2],2);
//	}

	//memcpy(line_show2[cnt_p%2],cache_dev[1],800*2);
	
	//create_sin_lines(line_show[cnt_p%2],&red_r0[pos_r-375+5],cnt_p%2);
	
	show_line(line_show[cnt_p%2],cnt_p%2,0);
  //show_line(line_show2[cnt_p%2],cnt_p%2,1);

  if( cnt_p >= 1  )
	{
		
		delay_us(200);
		
		if( cnt_p % 2 )
		{
			hide_line(line_show[0],0,0);
			//hide_line(line_show2[0],0,1);
		}
		else
		{
			hide_line(line_show[1],1,0);
			//hide_line(line_show2[0],0,1);
		}
		
		
		//hide_line(line_show[1],1);
		//hide_line(line_show[2],2);
	}	
	
	cnt_p++;
	/* read next seq */
	clock_sta = hal_read_gpio(DIO_CLOCK_STA) ? 1 : 0;
	/* enable tr */
	osc_fifo_clock(1);
	/* restart pwm */
	osc_start_adc_clock(0);//for test select the inter clock
}
/* the follow is for test */
#if 1


extern unsigned short gram[800*480*3];

unsigned short * osc_arr = &gram[800*600];

int LCD_DrawLine_ili(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned int color,gui_dev_def * dev,unsigned int got,unsigned char chn)
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
		if( chn == 0 )
		{
       dev->set_point(uRow,uCol,RGB(255,255,7));
		}else
		{
			 dev->set_point(uRow,uCol,RGB(7,227,231));
		}
		
	  if( chn == 0 )
		{
		  osc_arr[uRow + uCol*dev->width] |= (1<<to);
		}
		else
		{
			to += 5;
			osc_arr[uRow + uCol*dev->width] |= (1<<to);
		}
		
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


int LCD_DrawLine_cleat(unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned int color,gui_dev_def * dev,unsigned int got,unsigned char chn)
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
void show_line(unsigned short * line_d,unsigned short index,unsigned char chn )
{
  for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - 1 ; i ++ )
	{
		LCD_DrawLine_ili(msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[i] , msg_area->start_pos_x + i + 1 ,msg_area->start_pos_y + line_d[i+1],0,dev,index,chn);
	}
}

void hide_line(unsigned short * line_d,unsigned short index,unsigned char chn )
{
	msg_area = get_draw_area_msg();
	
	for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - 1 ; i ++ )
	{
		LCD_DrawLine_cleat(msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[i] , msg_area->start_pos_x + i + 1 ,msg_area->start_pos_y + line_d[i+1],0,dev,index,chn);
	}
}


#endif



































