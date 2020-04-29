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
#include "osc_cfg.h"
#include "math.h"
#include "string.h"
#include "hal_exit.h"
#include "osc_line.h"
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

extern window_def win_group[4];

/* btn */
extern widget_def btn[5];
/* text */
extern widget_def voltage_ch[2];
/* time */
extern widget_def time_ch[2];
/* ch1 measure */
extern widget_def measure_ch1[6];
extern widget_def measure_ch2[6];
/* menu text */
extern widget_def menu_text[7];
/* tips */
extern widget_def tips_text;
/* fast tips */
extern widget_def fast_tips[5];
/* widget arrow */
extern widget_def base_vol_arrow[2];

void create_osc_grid_status(void);
void show_line(unsigned short * line_d,unsigned short index,unsigned char chn,unsigned short zm );
void hide_line(unsigned short * line_d,unsigned short index,unsigned char chn ,unsigned short zm);
/* gui dev */
gui_dev_def * dev;

draw_area_def * msg_area;

 static unsigned int cnt_p = 0;
	
unsigned short line_show[2][800];
unsigned short line_show2[2][800];
unsigned short line_show3[2][800];
unsigned short line_show4[2][800];
unsigned short line_show5[2][800];

unsigned short line_zm[5];

const osc_time_def * osc_time_sw;

int osc_ts_leng = 0;

/* upload default setting */
static int osc_thead_init(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	
	msg_area = get_draw_area_msg();
	
	create_osc_grid_status();
	
	osc_rot_set(OSC_TIME_ROT,7);
	osc_rot_set(OSC_VOL_OFFSET_SCALE,20);
	osc_rot_set(OSC_VOL_SCALE,20);
	osc_rot_set(OSC_TRIG_SCALE,200);
	
	/* for test */
	osc_voltage_output(1870,2000,0,20);//1870,2000,0,270
	
//	double be = 0;
//	unsigned short tri = 0;
//	unsigned short retc = 0;
//	
//	for( int i = 0 ; i < 800 ; i ++ )
//	{
//		line_show[0][i] = (unsigned short)(150.0f * sin(be) + 150);
//		line_show[1][i] = line_show[0][i];
//		//line_show[1][i] = (unsigned short)(150.0f * cos(be) + 150 + 50);
//		line_show[2][i] = (tri++) + 200;
//		line_show[3][i] = retc ? 380 : 100;
//		
//		if( tri >= 150 )
//		{
//			tri = 0;
//		}
//		
//		be += ((6.28) * 4 / 750);
//		
//		if( i % 100 == 0 )
//		{
//			retc ^= 1;
//		}
//	}
	
	
	/* return as usual */
	return FS_OK;
}

void delay_us(unsigned int t)
{
	t *=100;
	
	while(t--);
}

unsigned char fe,fw = 1;

unsigned char fees = 0;

extern window_def win_menu;

unsigned short osc_zm = 1;

//void osc_draw_lines(gui_dev_def * dev,unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned short mode,unsigned short chn,unsigned short index);

//void osc_create_lines(unsigned short * line_d,unsigned char mode,unsigned short index,unsigned char chn ,unsigned short zm);


/* gui task */
static void osc_thread(void)
{
	double be = 0;
	static double dt = 0;
	unsigned short tri = 0;
	unsigned short retc = 0;
	
	for( int i = 0 ; i < 800 ; i ++ )
	{
		line_show[cnt_p%2][i] = (unsigned short)(150.0f * sin(be + dt) + 150);
  	line_show2[cnt_p%2][i] = (unsigned short)(150.0f * cos(be + dt) + 150 + 50);
		line_show3[cnt_p%2][i] = (tri++) + 200;
		line_show4[cnt_p%2][i] = retc ? 380 : 100;
		
		if( tri >= 150 )
		{
			tri = 0;
		}
		
		be += ((6.28) * 4 / 750);
		
		if( i % 100 == 0 )
		{
			retc ^= 1;
		}
	}	
	
	dt += 0.01;
	
	//osc_draw_lines(dev,0,0,750,400,0,0,0);
	//osc_draw_lines(dev,0,0,750,400,0,0,1);
	
	//osc_draw_lines(dev,0,0,750,400,1,0,0);
	
	osc_create_lines(dev,line_show[cnt_p%2],0,cnt_p%2,0,1);
	osc_create_lines(dev,line_show2[cnt_p%2],0,cnt_p%2,1,1);
	osc_create_lines(dev,line_show3[cnt_p%2],0,cnt_p%2,2,1);
	osc_create_lines(dev,line_show4[cnt_p%2],0,cnt_p%2,3,1);
//	osc_create_lines(line_show[0],0,1,0,1);
//	osc_create_lines(line_show[0],0,0,1,1);
//	osc_create_lines(line_show[1],0,0,1,1);
//	osc_create_lines(line_show[2],0,0,2,1);
//	osc_create_lines(line_show[3],0,0,3,1);
if( cnt_p >= 1  )
{
	if( cnt_p % 2 )
	{
		 osc_create_lines(dev,line_show[0],1,0,0,1);
		 osc_create_lines(dev,line_show2[0],1,0,1,1);
		osc_create_lines(dev,line_show3[0],1,0,2,1);
		osc_create_lines(dev,line_show4[0],1,0,3,1);
	}
	else
	{
		 osc_create_lines(dev,line_show[1],1,1,0,1);
		 osc_create_lines(dev,line_show2[1],1,1,1,1);
		osc_create_lines(dev,line_show3[1],1,1,2,1);
		osc_create_lines(dev,line_show4[1],1,1,3,1);
	}
}
	
//	osc_create_lines(line_show[1],0,0,1,1);
//	osc_create_lines(line_show[2],0,0,2,1);
//	osc_create_lines(line_show[3],0,0,3,1);
	
	cnt_p ++;
	
	unsigned short sta = osc_read_key_menu() ? 1 : 0;
	
	if( sta == 1 )
	{
		if( fw == 0 )
		{
			fw = 1;
			
			
			if( fees % 2 )
			{
				fe = 1;
			}
			else
			{
				fe = 2;
			}
			
			fees ++;
		}
	}else
	{
		if( fw == 1 )
		{
			fw = 0;
		}
	}
	
	if( fe == 1 )
	{	
		fe = 0;
		gui_hide_win(&win_menu);
	}
	else if( fe == 2 )
	{
		fe = 0;
		gui_show_win(&win_menu);
	}
	/* test */
  osc_offset_scale_thread(0);
	osc_trig_scale_thread(0);

	osc_vol_scale_thread(0);

	/* get scan time */
	osc_time_sw = osc_scan_thread();
	/* nothing to do */
	if( 1)//hal_read_gpio(FIFO_FULL0) != 0 )
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
	
	osc_zm = osc_time_sw->osc_zoom_factor;
	
	line_zm[cnt_p%2] = osc_zm;
	
	show_line(line_show[cnt_p%2],cnt_p%2,0,osc_zm);
  //show_line(line_show2[cnt_p%2],cnt_p%2,1);

  if( cnt_p >= 1  )
	{
		
		delay_us(20);
		
		if( cnt_p % 2 )
		{
			hide_line(line_show[0],0,0,line_zm[0]);
			//hide_line(line_show2[0],0,1);
		}
		else
		{
			hide_line(line_show[1],1,0,line_zm[1]);
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
	osc_start_adc_clock(osc_time_sw->osc_clock_ex);//for test select the inter clock
}
/* the follow is for test */
#if 1

//const chn_manage_def chn_m[] = 
//{
//	{
//		.BG_0 = COLOR_CH1_BG_0,
//		.BG_1 = COLOR_CH1_BG_1,
//		.BG_F = COLOR_CH1_BG_F,
//		.GR_0 = COLOR_CH1_GR_0,
//		.GR_1 = COLOR_CH1_GR_1,
//		.GR_F = COLOR_CH1_GR_F,
//	},
//	{
//		.BG_0 = COLOR_CH2_BG_0,
//		.BG_1 = COLOR_CH2_BG_1,
//		.BG_F = COLOR_CH2_BG_F,
//		.GR_0 = COLOR_CH2_GR_0,
//		.GR_1 = COLOR_CH2_GR_1,
//		.GR_F = COLOR_CH2_GR_F,
//	},	
//	{
//		.BG_0 = COLOR_MATH0_BG_0,
//		.BG_1 = COLOR_MATH0_BG_1,
//		.BG_F = COLOR_MATH0_BG_F,
//		.GR_0 = COLOR_MATH0_GR_0,
//		.GR_1 = COLOR_MATH0_GR_1,
//		.GR_F = COLOR_MATH0_GR_F,
//	},
//	{
//		.BG_0 = COLOR_MATH1_BG_0,
//		.BG_1 = COLOR_MATH1_BG_1,
//		.BG_F = COLOR_MATH1_BG_F,
//		.GR_0 = COLOR_MATH1_GR_0,
//		.GR_1 = COLOR_MATH1_GR_1,
//		.GR_F = COLOR_MATH1_GR_F,
//	},	
//	{
//		.BG_0 = COLOR_MATH2_BG_0,
//		.BG_1 = COLOR_MATH2_BG_1,
//		.BG_F = COLOR_MATH2_BG_F,
//		.GR_0 = COLOR_MATH2_GR_0,
//		.GR_1 = COLOR_MATH2_GR_1,
//		.GR_F = COLOR_MATH2_GR_F,
//	},	
//};
//unsigned int pointcnt = 0;
//unsigned int point_0 = 0,point_1 = 0;

//void osc_draw_lines(gui_dev_def * dev,unsigned short x1, unsigned short y1, unsigned short x2, unsigned short y2,unsigned short mode,unsigned short chn,unsigned short index)
//{
// /* defines */
// int xerr = 0 , yerr = 0 , delta_x , delta_y , distance; 
// int incx,incy,uRow,uCol; 
// /* dir */
// delta_x = x2 - x1;
// delta_y = y2 - y1; 
// uRow = x1; 
// uCol = y1; 
// /* dir */
// if( delta_x > 0 ) 
// { 
//  incx = 1; 
// }
// else if( delta_x == 0 )
// {
//  incx = 0;
// }
// else
// {
//  incx = -1 ; 
//  delta_x = -delta_x;
// }
// /* dir y delta */
// if( delta_y > 0 ) 
// { 
//  incy = 1; 
// }
// else if( delta_y ==0 )
// {
//  incy = 0;
// }
// else
//  {
//   incy = -1 ; 
//   delta_y = -delta_y;
// }
//  /* distance */ 
// if( delta_x > delta_y )
// { 
//  distance = delta_x;
// }
// else 
// {
//  distance = delta_y; 
// }
// /* get chn */
// const chn_manage_def * chn_d = &chn_m[chn];
//  /* draw points */
// for( int t = 0 ; t <= distance + 1 ; t++ ) 
// {
//	 pointcnt++;
//   /* draw point or clear point */	 
//	 if( mode == 0 ) /* draw */
//	 {
//		 /* get point */
//		 unsigned char piox = dev->read_point(uRow,uCol);
//		 /* chn */
//		 if( piox == COLOR_GRID_POINT ) // chn1
//		 {
//			 if( index == 0 )
//			 {
//			   dev->set_noload_point(uRow,uCol,chn_d->GR_0);
//				 
//				 point_0++;
//			 }
//			 else
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->GR_1);
//				 point_1 ++;
//			 }
//		 }
//		 else if( piox == COLOR_GRID_AREA_BG )
//		 {
//			 if( index == 0 )
//			 {
//			   dev->set_noload_point(uRow,uCol,chn_d->BG_0);
//				 point_0 ++;
//			 }
//			 else
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->BG_1);
//				 point_1 ++;
//			 }
//		 }
//		 else if( piox == chn_d->GR_0 )
//		 {
//			 if( index == 1 )
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->GR_F);
//				 point_1 ++;
//			 }
//		 }
//		 else if( piox == chn_d->GR_1 )
//		 {
//			 if( index == 0 )
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->GR_F);
//				 point_0 ++;
//			 }			 
//		 }
//		 else if( piox == chn_d->BG_0 )
//		 {
//			 if( index == 1 )
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->BG_F);
//				 point_1++;
//			 }
//		 }
//		 else if( piox == chn_d->BG_1 )
//		 {
//			 if( index == 0 )
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->BG_F);
//				 point_0 ++;
//			 }			 
//		 }		
//     else
//		 {
//			 /* other point */
//			 piox = 0;
//		 }	

//		 piox = dev->read_point(uRow,uCol);
//		 
//		 if( index == 0 )
//		 {
//       if( !(piox == chn_d->GR_0 || piox == chn_d->BG_0) )
//			 {
//				 piox = 0;
//			 }
//		 }
//		 else
//		 {
//       if( !(piox == chn_d->GR_F || piox == chn_d->BG_F) )
//			 {
//				 piox = 0;
//			 }			 
//		 }
//	 }
//	 else
//	 {
//		 /* clear */
//		 unsigned char piox = dev->read_point(uRow,uCol);
//		 /* chn */
//		 if( piox == chn_d->BG_0 ) // chn1
//		 {	
//			 if( index == 0 )
//			 {
//				dev->set_noload_point(uRow,uCol,COLOR_GRID_AREA_BG);
//			 }
//		 }
//		 else if( piox == chn_d->BG_1 )			 
//		 {
//			 if( index == 1 )
//			 {
//			   dev->set_noload_point(uRow,uCol,COLOR_GRID_AREA_BG);
//			 }
//		 }		 
//		 else if( piox == chn_d->GR_0 )			 
//		 {
//			 if( index == 0 )
//			 {
//			   dev->set_noload_point(uRow,uCol,COLOR_GRID_POINT);
//			 }
//		 }
//		 else if( piox == chn_d->GR_1 )			 
//		 {
//			 if( index == 1 )
//			 {
//			   dev->set_noload_point(uRow,uCol,COLOR_GRID_POINT);
//			 }
//		 }		 
//		 else if( piox == chn_d->GR_F )		
//		 {
//			 if( index == 0 )
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->GR_1);
//			 }
//			 else
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->GR_0);
//			 }
//		 }
//		 else if( piox == chn_d->BG_F )		
//		 {
//			 if( index == 0 )
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->BG_1);
//			 }
//			 else
//			 {
//				 dev->set_noload_point(uRow,uCol,chn_d->BG_0);
//			 }
//		 }
//		 else
//		 {
//			 /* other point */
//			 piox = 0;
//		 }
//	 }
//  /* inc */
//  xerr += delta_x ; 
//  yerr += delta_y ;
//    /* next line */  
//  if( xerr > distance ) 
//  { 
//   xerr -= distance; 
//   uRow += incx; 
//  } 
//  /* next */
//  if( yerr > distance ) 
//  { 
//   yerr -= distance; 
//   uCol += incy; 
//  } 
// }
//}

//void osc_create_lines(unsigned short * line_d,unsigned char mode,unsigned short index,unsigned char chn ,unsigned short zm)
//{
//	int it = 0;
//	
//	if( zm == 3 )
//	{
//		it = 250;
//	}
//	else if(  zm == 5 )
//	{
//		it = 300;
//	}
//	else if(  zm == 15 )
//	{
//		it = 350;
//	}
//	
//  for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - zm ; i += zm )
//	{
//		osc_draw_lines(dev,msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[it] , msg_area->start_pos_x + i + zm ,msg_area->start_pos_y + line_d[it+1],mode,chn,index);
//		
//		it ++;
//	}
//}

extern unsigned char gram[800*480*3];

unsigned short * osc_arr = (unsigned short *)&gram[800*600];

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
       dev->set_noload_point(uRow,uCol,COLOR_CH1);
		}else
		{
			 dev->set_noload_point(uRow,uCol,COLOR_CH2);
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
				dev->set_noload_point(uRow,uCol,COLOR_GRID_AREA_BG);
			}
			else
			{
				dev->set_noload_point(uRow,uCol,COLOR_GRID_POINT);
			}
		}
		else
		{
			//dev->set_noload_point(uRow,uCol,RGB(0, 255, 0));
		}
	}
	else
	{
//    dev->set_noload_point(uRow,uCol,color);
//		osc_arr[uRow + uCol*dev->width] |= (1<<to);
		//dev->set_noload_point(uRow,uCol,RGB(0, 0, 255));
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
			
			if( gram[i*dev->width + j] == COLOR_GRID_AREA_BG )
			{
				osc_arr[i*dev->width + j] |= 0x0400;
			}
			else if( gram[i*dev->width + j] == COLOR_GRID_POINT )
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
void show_line(unsigned short * line_d,unsigned short index,unsigned char chn ,unsigned short zm)
{
	int it = 0;
	
	if( zm == 3 )
	{
		it = 250;
	}
	else if(  zm == 5 )
	{
		it = 300;
	}
	else if(  zm == 15 )
	{
		it = 350;
	}
	
  for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - zm ; i += zm )
	{
		LCD_DrawLine_ili(msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[it] , msg_area->start_pos_x + i + zm ,msg_area->start_pos_y + line_d[it+1],0,dev,index,chn);
		
		it ++;
	}
}

void hide_line(unsigned short * line_d,unsigned short index,unsigned char chn,unsigned short zm )
{
	int it = 0;
	
	if( zm == 3 )
	{
		it = 250;
	}
	else if(  zm == 5 )
	{
		it = 300;
	}
	else if(  zm == 15 )
	{
		it = 350;
	}	
	
  for( int i = 0 ; i < msg_area->pixel_horizontal * msg_area->num_horizontal - zm ; i += zm )
	{
		LCD_DrawLine_cleat(msg_area->start_pos_x + i,msg_area->start_pos_y + line_d[it] , msg_area->start_pos_x + i + zm ,msg_area->start_pos_y + line_d[it+1],0,dev,index,chn);
		
		it ++;
	}	
}


#endif



































