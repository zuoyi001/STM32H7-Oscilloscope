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
#include "display_dev.h"
#include "math.h"
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "string.h"
/* Private includes ----------------------------------------------------------*/
#include "osc_icon.h"
/* draw area defines */
static draw_area_def draw_area;
/* define some nes message */
static unsigned short EN,EM;
#if !HARDWARE_ACCEL_SUPPLY /* no use without hardware accel */
static unsigned short ENR,EMRR;
#endif
/* this param is for test that will be deleted soon */
static unsigned char grid_init_flag = 0;
/* 
	create grid_grobal_data 
	Computing grid data according to different resolutions 
*/
static int grid_grobal_data(unsigned short width, unsigned short height)
{
	/* horizonal data Find the greatest common divisor for 5 and 10 */
	EN = ( width - LEFT_REMAIN_PIXEL - 2 ) / ( HORIZONTAL_GRID_TOTAL );
#if !HARDWARE_ACCEL_SUPPLY	
  /* The remaining pixels on the far right of the screen */
	ENR = width - LEFT_REMAIN_PIXEL - 1 - EN * (HORIZONTAL_GRID_TOTAL) - 1;
#endif
  /* for verital data Find the greatest common divisor for 5 and 8 */
	EM = ( height - TOP_REMAIN_PIXEL - 2 - BOTTOM_REMAIN_PIXEL ) / (VERTICAL_GRID_TOTAL);
  /* The remaining pixels on the far bottom of the screen */
#if !HARDWARE_ACCEL_SUPPLY	
	EMRR = height - EM * (VERTICAL_GRID_TOTAL) - TOP_REMAIN_PIXEL - 1 - 1;
#endif
	/* create ok or not */
	if( EN == 0 || EM == 0 )
	{
		/* oh no , feels not good , What seems to be the problem */
		return (-1);
	}
	/* set area data  , start pos */
	draw_area.start_pos_x = LEFT_REMAIN_PIXEL + 1;
	draw_area.start_pos_y = TOP_REMAIN_PIXEL + 1;
	draw_area.stop_pos_x  = LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL);
	draw_area.stop_pos_y = TOP_REMAIN_PIXEL  + 1 + EM * (VERTICAL_GRID_TOTAL);
	/* num grid of horizontal and vertical */
	draw_area.num_horizontal = HORIZONTAL_GRID_NUM;
	draw_area.num_vertical = VERTICAL_GRID_NUM;
	/* num pixel for all horizontal */
	draw_area.pixel_horizontal = LITTLE_GRIG_NUM * EN;
	draw_area.pixel_vertiacl = LITTLE_GRIG_NUM * EM;
	/* num of little grid */
	draw_area.little_grid = LITTLE_GRIG_NUM;
	/* total pixel */
	draw_area.total_pixel_h = HORIZONTAL_GRID_NUM * LITTLE_GRIG_NUM * EN;
	draw_area.total_pixel_v = VERTICAL_GRID_NUM * LITTLE_GRIG_NUM * EM;
	/* endding */
	return 0; // OK
}
/* get grid data struct for other apps and threads */
draw_area_def * get_draw_area_msg(void)
{
  /* create init or not */
	if( grid_init_flag == 0 )
	{
		/* reinit */
		return (draw_area_def *)(0) ; // not init yet 
	}
	/* return bat */
	return &draw_area;
}
/* create grid data */
static void create_grid_data(window_def * win)
{	
  /* get gui dev */
	/* set grid data */
	int ret = grid_grobal_data( win->dev->width , win->dev->height );
	/* ok or not */
	if( ret != 0 )
	{
		/* oh no , feels not good , What seems to be the problem */
		return;		
	}
  /* sraw */
#if HARDWARE_ACCEL_SUPPLY
	/* clear all area with one color */
	win->dev->clear_display_dev(COLOR_BACK_GROUND);
	/* if doesn't has the hardware accel */
#else
	for (unsigned int j = 0; j < win->dev->height; j++)
	{
		for (int i = 0; i < LEFT_REMAIN_PIXEL; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,i, j, COLOR_BACK_GROUND);
		}

		for (int i = 0; i < ENR; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,i + LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) +1 +1, j, COLOR_BACK_GROUND);
		}
	}
  /* draw other grid data */
	for (unsigned int j = 0; j < win->dev->width; j++)
	{
		for (int i = 0; i < TOP_REMAIN_PIXEL; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,j, i, COLOR_BACK_GROUND);
		}
    /* draw other grid data */
		for (int i = 0; i < EMRR - 1; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,j, i + TOP_REMAIN_PIXEL + 1 + 1 + 1 + EM * (VERTICAL_GRID_TOTAL), COLOR_BACK_GROUND);
		}
	}
#endif
/* create the data area background color */
#if HARDWARE_ACCEL_SUPPLY
/* full the rect area with some color */
  win->dev->fill_rect(LEFT_REMAIN_PIXEL + 1,TOP_REMAIN_PIXEL + 1,
	               LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) , 
								 TOP_REMAIN_PIXEL  + EM * (VERTICAL_GRID_TOTAL),COLOR_GRID_AREA_BG);
#else
	for (int j = 0; j < EN * (HORIZONTAL_GRID_TOTAL) ; j++)
	{
		for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1 ; i++)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + j, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_AREA_BG);
		}
	}
#endif
  /* draw other grid data */
	for(int j = 0; j < ( VERTICAL_GRID_NUM + 1 ); j++ )
	{
		for (int i = 0 ; i < ( HORIZONTAL_GRID_TOTAL + 1 ); i++ )
		{
			win->dev->set_point(&win->msg.mark_flag, LEFT_REMAIN_PIXEL + 1 + i * EN, TOP_REMAIN_PIXEL + 1 + j * EM * LITTLE_GRIG_NUM , COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int j = 0 ; j < ( HORIZONTAL_GRID_NUM + 1 ) ; j++ )
	{
		for (int i = 0; i < (VERTICAL_GRID_TOTAL + 1); i++ )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + j * EN * LITTLE_GRIG_NUM , TOP_REMAIN_PIXEL + 1 + i * EM, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0 ; i < EN * ( HORIZONTAL_GRID_TOTAL ) ; i++ )
	{
	  /* draw other grid data */ 
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( i % EN ) == 0 )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 2, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 5, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EN)) == 0 )
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 3, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 6, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL + 1  + i, COLOR_GRID_POINT);
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		/* draw other grid data */
		if((( i % EM )) == 0 )
		{
			/* draw other grid data */
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			/* draw other grid data */
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 2 , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2), COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( (i % EN) ) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 2, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 5, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 1, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 4, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2), TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EM)) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 1, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 4, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
    /* draw other grid data */
		if (((i % (EM * LITTLE_GRIG_NUM))) == 0)
		{
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 8, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 7, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL, COLOR_GRID_POINT);
	win->dev->set_point(&win->msg.mark_flag,LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	/* create the title */
	osc_create_TITLE(win);
	/* for test */
#if 0
  /* draw other grid data */
	double sin_x = 0;
	/* sinx */
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL) ; i ++ )
	{
	 double te = sin( sin_x );
		
   short tm = (short)( te * EM * (VERTICAL_GRID_TOTAL / 4) ) + EM * (VERTICAL_GRID_TOTAL / 2) + TOP_REMAIN_PIXEL + 1 - EM*LITTLE_GRIG_NUM;
		
	 double tce = cos( sin_x );
		
   short tcm = (short)( tce * EM * (VERTICAL_GRID_TOTAL / 4) ) + EM * (VERTICAL_GRID_TOTAL / 2) + TOP_REMAIN_PIXEL + 1 + EM*LITTLE_GRIG_NUM;	 
		
		win->dev->set_point(&win->msg.mark_flag, LEFT_REMAIN_PIXEL + 1 + i , tm , RGB(255,255,7));
		
		win->dev->set_point(&win->msg.mark_flag, LEFT_REMAIN_PIXEL + 1 + i , tcm , RGB(7,227,231));
		
		sin_x += (6.28) / (double)(EN * (HORIZONTAL_GRID_TOTAL)) * 3;
		
	}
#endif
/* set the flag */
	grid_init_flag = 1;
/* end of func */	
}
/* draw group win */
static void draw_group_win(window_def * win)
{
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
  /* calutie and ending pos */
	unsigned short  pos_x = win->msg.x , pos_y = win->msg.y;
  /* calutie and ending pos */
	unsigned short pos_x_end = win->msg.x_size , pos_end_y = win->msg.y_size;
  /* at circle mode create gui */
	for( int i = 0 ; i < pos_end_y - 2 ; i ++ )
	{
		for( int j = 0 ; j < pos_x_end - 2 ; j ++ )
		{
			if( i == 0 && j == 0 )
			{
			}
			else if( i == 0 && (j == pos_x_end - 2 - 1) )
			{

			}
			else if( (i == pos_end_y - 2 - 1) && j == 0 )
			{

			}
			else if( (i == pos_end_y - 2 - 1) && (j == pos_x_end - 2 - 1) )
			{
			}
			else
			{
				win->dev->set_point(&win->msg.mark_flag, j + pos_x + 1 ,pos_y + i + 1,COLOR_GRID_AREA_BG);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x ,i / 4 + pos_y,COLOR_GRID_POINT);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_GRID_POINT);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_GRID_POINT);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			win->dev->set_point(&win->msg.mark_flag, i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_GRID_POINT);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x + 4 + i , pos_y , COLOR_GRID_POINT);
		win->dev->set_point(&win->msg.mark_flag,pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_GRID_POINT);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x , pos_y + 4 + i , COLOR_GRID_POINT);
		win->dev->set_point(&win->msg.mark_flag,pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_GRID_POINT);
	}	
	/* create icon */
	if( win->msg.wflags & 0x0100 )
	{
		/* calbrate pos */
		unsigned short eh = ( win->msg.x_size - 4 ) / 3;
		unsigned short v_start = ( win->msg.y_size - 16 * 2 ) / 3;
		
		/* create icon */
		osc_create_chn_icon(win,2,v_start,1);
		osc_create_chn_icon(win,2 + eh,v_start,1);
		osc_create_chn_icon(win,2 + eh*2,v_start,1);
		/* ch2 */
		osc_create_chn_icon(win,2,v_start * 2 + 16,2);
		osc_create_chn_icon(win,2 + eh,v_start * 2 + 16,2);
		osc_create_chn_icon(win,2 + eh*2,v_start * 2 + 16,2);	
	}
	else if( win->msg.wflags & 0x0200 )
	{
		/* calbrate pos */
		unsigned short eh = ( win->msg.x_size - 4 ) / 2;
		unsigned short v_start = ( win->msg.y_size - 16 ) / 2;		
		/* create icon */
		osc_create_chn_icon(win,2,v_start,1);	
		osc_create_chn_icon(win,2 + eh ,v_start,2);	
	}
	else if( win->msg.wflags & 0x0800 )
	{
		/* nothing to do */
	}
}
/* draw the menu window */
static void draw_menu_win(window_def * win)
{
	/* transfer the menu size */
	unsigned short MENU_WIDTH = win->msg.x_size;//107;
	unsigned short MENU_HEIGHT = win->msg.y_size;//430;
	/* start pos */
	unsigned short pos_x_m = win->msg.x; //dev->width - MENU_WIDTH;
	unsigned short pos_y_m = win->msg.y; //0
	 /*fill the rect */
#if HARDWARE_ACCEL_SUPPLY
  /* fill rect */
	win->dev->fill_rect( pos_x_m , pos_y_m , 
	                pos_x_m + MENU_WIDTH - 1 , pos_y_m + MENU_HEIGHT - 1,
	                COLOR_BACK_GROUND );//background color
#else
	/* draw points one by one */
	for (int i = 0; i < MENU_WIDTH; i++)
	{
		for (int j = 0; j < MENU_HEIGHT; j++)
		{
			win->dev->set_point(&win->msg.mark_flag,pos_x_m + i , pos_y_m + j, COLOR_BACK_GROUND);
		}
	}	
#endif
	/* top line */
	for (int i = 0; i < MENU_WIDTH; i++)
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + 1, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + 2, COLOR_MENU_ONE);
    /* bottom line */
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + MENU_HEIGHT - 1, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + MENU_HEIGHT - 2 , COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + i, pos_y_m + MENU_HEIGHT - 3, COLOR_MENU_ONE);
	}
	/* left lines */
	for (int i = 0; i < MENU_HEIGHT - 6; i++)
	{
		win->dev->set_point(&win->msg.mark_flag,pos_x_m, pos_y_m + i + 3, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + 1, pos_y_m + i + 3, COLOR_MENU_ONE);
		win->dev->set_point(&win->msg.mark_flag,pos_x_m + 2, pos_y_m + i + 3, COLOR_MENU_ONE);
	}
}
/* create icon */
static void osc_create_TITLE(window_def * win)
{
	/* color tmp */
	unsigned short * color = (unsigned short *)TITLE_OSC;
	/* create */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		for( int j = 0 ; j < 40 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			win->dev->set_point(&win->msg.mark_flag,j , i , ( color[i*40+j] == 0xffff ) ? COLOR_GRID_AREA_BG : COLOR_BACK_GROUND);
#else
			unsigned short tm = color[i*32+j];

			win->dev->set_point(&win->msg.mark_flag,parent_x + pos_x + j , parent_y + pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}
}
/* draw offset arrow */
static void osc_draw_offset_arrow(gui_dev_def * dev,unsigned short pos_x,unsigned short pos_y,unsigned short chn)
{
	/* color table */
	const unsigned char ch_common_table[16] = {0x01,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x3f,0x3f,0x3f,0x1f,0x1f,0x0f,0x07,0x01};
	const unsigned char chn1_tab[10] = {0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x00};
	const unsigned char chn2_tab[10] = {0x78,0xFC,0x8C,0x0C,0x1C,0x38,0x70,0xE0,0xfC,0xFC};
	/* chn */
	unsigned short color = ( chn == 1 ) ? COLOR_CH1 : COLOR_CH2;
	const unsigned char * chnn = ( chn == 1 ) ? chn1_tab : chn2_tab;
	/*-------------*/
	for( int i = 0 ; i < 16 ; i ++ )
	{
		for( int j = 0 ; j < 32 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			if( j < 8 )
			{
				if( ( ch_common_table[i] << j ) & 0x80 )
				{
					dev->set_noload_point( pos_x + j , pos_y + i , color);
				}
			}
			else if( j >= 24 )
			{
       if( ( ch_common_table[i] >> (j-24) ) & 0x01 )
			 {
				 dev->set_noload_point( pos_x + j , pos_y + i , color);
			 }
			}
		  else
			{
				dev->set_noload_point( pos_x + j , pos_y + i , color);
			}			
		  /* 1 or 2 */
			if( i >= 3 && i < 13)
			{
	       if( j >= 13 && j < 21 )
				 {
					 if( (chnn[i-3] << (j-13)) & 0x80 )
					 {
						 dev->set_noload_point( pos_x + j , pos_y + i , COLOR_GRID_AREA_BG);
					 }
				 }					 
			}
#else
			unsigned short tm = color[i*32+j];

			widget->dev->set_noload_point(parent_x + pos_x + j , parent_y + pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}
}
/* create icon */
static void osc_create_chn_icon(window_def * parent_win,unsigned short x,unsigned short y,unsigned short chn)
{
	/* widget pos */
	unsigned short pos_x = x + parent_win->msg.x;
	unsigned short pos_y = y + parent_win->msg.y;
	/* create */
  osc_draw_offset_arrow(parent_win->dev,pos_x,pos_y,chn);
	/* end if */
}
/* create button */
static void osc_create_button(struct widget * widget)
{
	/* create btn */
	/* define the btn size */
  unsigned short MENU_BTN_WIDTH = widget->msg.x_size;//102;
	unsigned short MENU_BTN_HEIGHT = widget->msg.y_size;//79;
	/* define btn start pos */
	unsigned short pos_btn_x = widget->msg.x ;//5
	unsigned short pos_btn_y = widget->msg.y ;//24;
	/* parent win size */
	unsigned short pos_x_m = widget->parent->msg.x;
	unsigned short pos_y_m = widget->parent->msg.y;
	/* parent win size */
#if 0
	unsigned short parent_size_x = widget->parent->msg.x_size;
	unsigned short parent_size_y = widget->parent->msg.y_size;
#endif
	/* create the background */
#if HARDWARE_ACCEL_SUPPLY
 /* fill rect */
	widget->dev->fill_rect( pos_x_m + pos_btn_x + 2 , pos_y_m + pos_btn_y + 2 , 
	                pos_x_m + pos_btn_x + 2 + MENU_BTN_WIDTH - 1 - 2,
					        pos_y_m + pos_btn_y + MENU_BTN_HEIGHT + 2 - 1 - 4,
	                COLOR_BUTTON );
#else
	/* create one by one */
	for (int i = 0; i < MENU_BTN_WIDTH - 2; i++)
	{
		for (int j = 0; j < MENU_BTN_HEIGHT - 4; j++)
		{
			widget->dev->set_noload_point(pos_x_m + pos_btn_x + 2 + i, pos_y_m + pos_btn_y + j + 2, COLOR_BUTTON);
		}
	}
#endif
}
/* create win_main_pos */
void osc_calculate_main_size(gui_dev_def * dev,window_def * win,unsigned short wf)
{
	/* create the win */
	win->msg.x = 0;
	win->msg.y = 0;
	win->msg.x_size = dev->width;
	win->msg.y_size = dev->height;
	win->dev = dev;
	/* set callback */
	win->draw = create_grid_data;
	/* wf */
	win->msg.wflags = wf | 0xE0;
	/* ------- */
	/* create win creater */
	gui_win_creater(win);	
}
/* static cal the osc grid fast fast */
static int osc_grid_fast(gui_dev_def * tdev,unsigned short * total_ph,unsigned short * total_pv)
{
	/* vertical */
	unsigned short em_t = ( tdev->height - TOP_REMAIN_PIXEL - 2 - BOTTOM_REMAIN_PIXEL ) / (VERTICAL_GRID_TOTAL);
	/* horizonal */
	unsigned short en_t = ( tdev->width - LEFT_REMAIN_PIXEL - 2 ) / ( HORIZONTAL_GRID_TOTAL );
	
	/* if */
	if( !em_t || !en_t )
	{
		return FS_ERR;
	}
	/* cal hori */
	if( total_ph != 0 )
	{
		*total_ph = en_t * HORIZONTAL_GRID_NUM * LITTLE_GRIG_NUM;
	}
	/* vertical */
	if( total_pv != 0 )
	{
		*total_pv = em_t * VERTICAL_GRID_NUM * LITTLE_GRIG_NUM;
	}
	/* return ok */
	return FS_OK;
}
/* static group pos */
void osc_calculate_sg_size(gui_dev_def * dev,window_def * win0,unsigned int num)
{
	/* def*/
	unsigned short vertical_pixel_total;
	/* get msg */
	if( osc_grid_fast(dev,0,&vertical_pixel_total) == FS_ERR )
	{
		/* creater fail */
		return;
	}
	/* bad data */
	if( num < 4 )
	{
		return;
	}
	/* create the win */
	win0[0].msg.x = dev->width - 430 - 1 ;
	win0[0].msg.y = vertical_pixel_total + TOP_REMAIN_PIXEL + 2 + 5;
	win0[0].msg.x_size = 430;
	win0[0].msg.y_size = dev->height - vertical_pixel_total - TOP_REMAIN_PIXEL - 2 - 10;
	win0[0].dev = dev;
	win0[0].msg.wflags = 0x0100 | 0xC0;
	/* set callback */
	win0[0].draw = draw_group_win;
	/* create the group */
	win0[1].msg.x = 1;//win0[0].msg.x + win0[0].msg.x_size + 2;
	win0[1].msg.y = win0[0].msg.y;
	win0[1].msg.x_size = ( dev->width - win0[0].msg.x_size ) * 2 / 3;
	win0[1].msg.y_size = (dev->height - vertical_pixel_total - TOP_REMAIN_PIXEL - 2 - 8) / 2 - 1;
	win0[1].dev = dev;
	win0[1].msg.wflags = 0x0200 | 0xC0;
	/* set callback */
	win0[1].draw = draw_group_win;
	/* create the group */
	win0[2].msg.x = win0[1].msg.x;
	win0[2].msg.y = win0[1].msg.y + win0[1].msg.y_size + 1;
	win0[2].msg.x_size = dev->width - win0[0].msg.x_size - 5;
	win0[2].msg.y_size = win0[1].msg.y_size - 1;
	win0[2].dev = dev;
	win0[2].msg.wflags = 0x0400 | 0xC0;
	/* set callback */
	win0[2].draw = draw_group_win;
	/* create the group */
	win0[3].msg.x = win0[1].msg.x + win0[1].msg.x_size + 2;
	win0[3].msg.y = win0[1].msg.y;
	win0[3].msg.x_size = dev->width - win0[0].msg.x_size - win0[1].msg.x_size - 6;
	win0[3].msg.y_size = win0[1].msg.y_size;
	win0[3].dev = dev;
	win0[3].msg.wflags = 0x0800 | 0xC0;
	/* set callback */
	win0[3].draw = draw_group_win;
  /* ok create the win */
	gui_win_creater(&win0[0]);
	gui_win_creater(&win0[1]);
  gui_win_creater(&win0[2]);
  gui_win_creater(&win0[3]);	
}
/* create win_menu_pos */
void osc_calculate_menu_size(gui_dev_def * dev,window_def * win,unsigned short wf)
{
	/* def*/
	unsigned short vertical_pixel_total;
	/* get msg */
	if( osc_grid_fast(dev,0,&vertical_pixel_total) == FS_ERR )
	{
		/* creater fail */
		return;
	}	
	/* create the win */
	win->msg.x_size = (unsigned short)(((float)107 / (float)800)*dev->width);
	win->msg.y_size = vertical_pixel_total + TOP_REMAIN_PIXEL + 3;
	win->msg.x = dev->width - win->msg.x_size;
	win->msg.y = 0;
	win->dev = dev;
	/* set wflags d*/
	win->msg.wflags = wf | 0xE0;
	/* set callback */
	win->draw = draw_menu_win;
	/* ok */
	/* create */
	gui_win_creater(win);		
}
/* create win_btn_pos */
void osc_calculate_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,unsigned short btn_num)
{
	/* button ypos */
	unsigned short btn_tpos = 0;
	/* calculate x size and y size */
	unsigned short x_size_g = win->msg.x_size - 5;
	unsigned short y_size_g = (win->msg.y_size - 24 - 1 ) / btn_num - 2;
	/* create btn */
	for( int i = 0 ; i < btn_num ; i ++ )
	{
		/* calculate the y size */
		btn_tpos = ( y_size_g + 2 ) * i + 24;
		/* set det */
		wd[i].msg.x = 5;
		wd[i].msg.y = btn_tpos;
		wd[i].msg.x_size = x_size_g;
		wd[i].msg.y_size = y_size_g;
		wd[i].dev = dev;
		wd[i].draw = osc_create_button;
		wd[i].parent = win;
		wd[i].msg.upd = COLOR_BUTTON;
		/* create btn */
		gui_widget_creater(&wd[i]);
	}
}
/* create the voltage title */
void osc_calculate_volage_string(window_def * pwin,widget_def *wd,int num,char * ch1,char * ch2)
{
	/* if num is not epue */
	if( num != 2 )
	{
		return;//reutrn,bad data
	}
	/* calbrate the pox */
	unsigned short ev = ( pwin->msg.y_size - 24/* char height */) / 2 ; 
	unsigned short eh = pwin->msg.x_size / 2 ;
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* default is 24 size */
		wd[i].msg.wflags |= 0x1000;
		/* channel */
		wd[i].msg.wflags |= ( 0x2000 << i );	
		/* set ch1 */
		wd[i].msg.x = eh * i + 32 + 5;
		wd[i].msg.y = ev;
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		if( i == 0 )
		{
		  wd[i].msg.pri_data = ch1;
		}
		else
		{
			wd[i].msg.pri_data = ch2;
		}
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create the voltage title */
void osc_calculate_time_string(window_def * pwin,widget_def *wd,int num,char * M,char * time)
{
	/* if num is not epue */
	if( num != 2 )
	{
		return;//reutrn,bad data
	}
	/* calbrate the pox */
	unsigned short ev = ( pwin->msg.y_size - 24/* char height */) / 2 ; 
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* default is 24 size */
		wd[i].msg.wflags |= 0x1000;
		/* set ch1 */
		wd[i].msg.x = 12 * i + i * 5 + 10;
		wd[i].msg.y = ev;
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		if( i == 0 )
		{
		  wd[i].msg.pri_data = M;
		}
		else
		{
			wd[i].msg.pri_data = time;
		}
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create the voltage title */
void osc_calculate_measure_ch(window_def * pwin,widget_def *wd,int num,char ** item,unsigned char ch)
{
	/* if num is not epue */
	if( num != 6 )
	{
		return;//reutrn,bad data
	}
	/* calbrate the pox */
	unsigned short eh = pwin->msg.x_size / 3;
	unsigned short v_start = ( pwin->msg.y_size - 16 * 2 ) / 3;
	/* strlen */
	int strl = 0;
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* default is 16 size */
		wd[i].msg.wflags &=~ 0x1000;
		/* channel */
		if( ch == 1 )
		{
			wd[i].msg.wflags |= 0x2000;
		}
		else
		{
			wd[i].msg.wflags |= 0x4000;
		}
		/* set ch1 */
		if( i % 2 )
		{
			wd[i].msg.x = eh * (i/2) + 32 + 16 * strl / 2 + 16;
		}
		else
		{
			wd[i].msg.x = eh * i / 2 + 32 + 2;
			/* get strlen len */
			strl = strlen(item[i]);
		}
		/* vertical */
		if( ch == 1 )
		{
			wd[i].msg.y = v_start;
		}
		else
		{
			wd[i].msg.y = v_start * 2 + 16;
		}
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		/* item */
		wd[i].msg.pri_data = item[i];
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create menu text size */
static int osc_hz_len(char * chd,int * enter)
{
	/* match \n or \0*/
	int i = 0;
	/* get */
	while(*chd != 0 )
	{
		if( *chd == '\n' )
		{
			/* en*/
			*enter = 1;
			/* return */
			return i;
		}
		/* next */
		i++;
		chd++;
	}
	/* reuturn */
	return i;
}
/* create menu text size */
void osc_calculate_menu(window_def * pwin,widget_def *wd,int num,char ** item)
{
	/* if num is not epue */
	if( num == 0 )
	{
		return;//reutrn,bad data
	}
	/* strlen */
	int strl = strlen(item[0]);
	/* font size */
	unsigned char size = (wd[0].msg.wflags & 0x1000) ? 24 : 16;
	/* calbrate the pox */
	unsigned short title_x = ( pwin->msg.x_size - strl / 2 * size ) / 2;
	unsigned short title_y = ( 24 - size ) / 2 + 3; 
	/* other */
	unsigned short eh = pwin->msg.x_size - 5;
	unsigned short v_start = ( pwin->msg.y_size - 24 - 1 ) / ( num - 1 ) - 2;
	/* setting */
  for( int i = 0 ; i < num ; i ++ )
	{
		/* set system title */
		if( i == 0 )
		{
			wd[i].msg.x = title_x;
			/* vertical */
			wd[i].msg.y = title_y;
		  /* char default is 16 size */
		  wd[i].msg.wflags &=~ 0x1000;			
		}
		else
		{
			/* char default is 24 size */
			wd[i].msg.wflags |= 0x1000;
			/* get len */
			int enter_flag = 0;
			int lenc = osc_hz_len(item[i],&enter_flag);
			/* get len */
			wd[i].msg.x = ( eh - lenc / 2 * 16 ) / 2 + 5;
			/* calculate the y size */
      wd[i].msg.y = ( v_start + 2 ) * ( i - 1 ) + 24 + v_start / 2 - ( enter_flag ? 16 : 8 );
		}
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		/* item */
		wd[i].msg.pri_data = item[i];
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create msg group */
void osc_calculate_tips(window_def * pwin,widget_def *wd,unsigned short level,char * tip)
{
	/* calbrate the pox */
	unsigned short ev = ( pwin->msg.y_size - 16/* char height */) / 2 ; 
	/* setting */
	/* default is 16 size */
	wd->msg.wflags &=~ 0x1000;
	/* wd */
	wd->msg.wflags &=~ 0xE000;
	/* get color */
	wd->msg.wflags |= level;
	/* set ch1 */
	wd->msg.x = pwin->msg.x + 5;
	wd->msg.y = ev;
	/* not supply size now */
#if 0
	wd->msg.x_size = 0;
	wd->msg.y_size = 0;
#endif
	/* size */
	wd->msg.pri_data = tip;
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = gui_dynamic_string;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);
}
/* draw a chown */
static void osc_draw_arrow_noload(gui_dev_def * dev,unsigned short pos_x,unsigned short pos_y,unsigned short reh,unsigned short chn,unsigned short backcolor)
{
	/* osc table */
	const unsigned char arrow_table[12] = { 0xC0,0xE0,0xf0,0xF8,0xFC,0xfE,0xFE,0xFC,0xF8,0xf0,0xE0,0xC0};
	const unsigned char chn1_tab[10] = {0x18,0x38,0x78,0x18,0x18,0x18,0x18,0x18,0x18,0x00};
	const unsigned char chn2_tab[10] = {0x78,0xFC,0x8C,0x0C,0x1C,0x38,0x70,0xE0,0xfC,0xFC};
	/* chn */
	unsigned short color = ( chn == 1 ) ? COLOR_CH1 : COLOR_CH2;
	const unsigned char * chnn = ( chn == 1 ) ? chn1_tab : chn2_tab;
	unsigned short color_num = ( chn == 1 ) ? COLOR_TEXT_ARROW_CH1 : COLOR_TEXT_ARROW_CH2;
	/* set check */
	for( int i = 0 ; i < 12 ; i ++ )
	{
		/* onde */
		for( int j = 0 ; j < 20 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			if( reh == 0 )
			{
				/* check */
				if( j >= 12 )
				{
					/* arrow table */
					if( (arrow_table[i] << (j-12)) & 0x80 )
					{
						if( dev->read_point(pos_x + j , pos_y + i) == backcolor )
						{
							dev->set_noload_point(pos_x + j , pos_y + i , color);
						}
					}
				}
				else
				{
					if( dev->read_point(pos_x + j , pos_y + i) == backcolor )
					{
						dev->set_noload_point(pos_x + j , pos_y + i , color);
					}
				}
				/* chnn */
				if( i >= 1 && i < 11 )
				{
					/* check */
					if( j >= 5 && j < 13 )
					{
						/* set color */
						if( (chnn[i-1] << (j-5)) & 0x80 )
						{
							if( dev->read_point(pos_x + j , pos_y + i) == backcolor || dev->read_point(pos_x + j , pos_y + i) == color )
							{
								dev->set_noload_point(pos_x + j , pos_y + i , color_num);
							}
						}						
					}
				}
			}
			else
			{
				/* chn */
				if( dev->read_point(pos_x + j , pos_y + i) == color )
				{
					dev->set_noload_point(pos_x + j , pos_y + i , backcolor);
				}
				else if( dev->read_point(pos_x + j , pos_y + i) == color_num )
				{
					dev->set_noload_point(pos_x + j , pos_y + i , backcolor);
				}					
			}
#else
				unsigned short tm = color[i*20+j];

				widget->dev->set_noload_point(pos_x + j , pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}	
}
/* create the base voltage icon */
static void osc_draw_chn_arrow(widget_def * wd)
{
	/* color tmp */
	unsigned char chn;
	unsigned short backcolor;
	unsigned char rehide_flag = 0;
	/* switch chn */
	if( wd->msg.wflags & 0x8000 )
	{
		chn = 1;
	}
	else
	{
		chn = 2;
	}
	/* check rehide */
	if( CHECK_REHIDE(wd->msg.wflags) || wd->msg.mark_flag == 2 )
	{
		/* get parent color */
		backcolor = gui_color((wd->parent->msg.wflags & 0x00E0) << 8);
		/* set */
		rehide_flag = 1;
		/* set deft */
		if( CHECK_REHIDE(wd->msg.wflags) )
		{
			wd->msg.mark_flag = 1;
		}
		/* clear */
		CLEAR_REHIDE(wd->msg.wflags);		
	}
	else
	{
		wd->msg.mark_flag = 1;
	}
	/* move */
	while(wd->msg.mark_flag)
	{
		/* widget pos */
		unsigned short pos_x = wd->msg.x;
		unsigned short pos_y = wd->msg.y;
		/* create */
		osc_draw_arrow_noload(wd->dev,pos_x,pos_y,rehide_flag,chn,backcolor);
		/* create */
		wd->msg.mark_flag --;
		/* check */
		if( wd->msg.mark_flag )
		{
			/* repos */
			wd->msg.x = wd->msg.mx;
			wd->msg.y = wd->msg.my;
			/* pos */
			pos_x = wd->msg.x;
			pos_x = wd->msg.y;
			/* redraw */
			rehide_flag = 0;
			/* reduce */
		}
  }
}
/* create the base voltage icon */
static void osc_draw_trig_arrow(widget_def * wd)
{
	/* color tmp */
	const unsigned char trig_shape[13] = 
	{ 0x02,0x06,0x0E,0x1E,0x3E,0x7f,0xff,0x7f,0x3E,0x1E,0x0E,0x06,0x02};
	unsigned short color;
	unsigned short backcolor;
	unsigned char rehide_flag = 0;
	/* switch chn */
	if( wd->msg.wflags & 0x8000 )
	{
		color = COLOR_CH1;
	}
	else
	{
		color = COLOR_CH2;
	}
	/* check rehide */
	if( CHECK_REHIDE(wd->msg.wflags) || wd->msg.mark_flag == 2 )
	{
		/* get parent color */
		backcolor = gui_color((wd->parent->msg.wflags & 0x00E0) << 8);
		/* set */
		rehide_flag = 1;
		/* set deft */
		if( CHECK_REHIDE(wd->msg.wflags) )
		{
			wd->msg.mark_flag = 1;
		}
		/* clear */
		CLEAR_REHIDE(wd->msg.wflags);		
	}
	else
	{
		wd->msg.mark_flag = 1;
	}
	/* move */
	while(wd->msg.mark_flag)
	{
		/* widget pos */
		unsigned short pos_x = wd->msg.x;
		unsigned short pos_y = wd->msg.y;
		/* create */
		for( int i = 0 ; i < 13 ; i ++ )
		{
			for( int j = 0 ; j < 8 ; j ++ )
			{
#ifndef _VC_SIMULATOR_
				if( rehide_flag == 0 )
				{
					/* check point */
					if( ( trig_shape[i] << j ) & 0x80 )
					{
						wd->dev->set_noload_point(pos_x + j , pos_y + i , color);
					}
				}
				else
				{
					wd->dev->set_noload_point(pos_x + j , pos_y + i , backcolor);
				}
#else
				unsigned short tm = color[i*20+j];

				widget->dev->set_noload_point(pos_x + j , pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
			}
		}
		/* fe */
		wd->msg.mark_flag --;
		/* check */
		if( wd->msg.mark_flag )
		{
			/* repos */
			wd->msg.x = wd->msg.mx;
			wd->msg.y = wd->msg.my;
			/* pos */
			pos_x = wd->msg.x;
			pos_x = wd->msg.y;
			/* redraw */
			rehide_flag = 0;
			/* reduce */
		}
  }
}
/* */
void osc_calculate_base_arrow(window_def * pwin,widget_def *wd,int chn)
{
	/* set param */
	/* def*/
	unsigned short vertical_pixel_total;
	/* get msg */
	if( osc_grid_fast(pwin->dev,0,&vertical_pixel_total) == FS_ERR )
	{
		return;
	}		
	/* calbrate the pox */
	/* get chn */
	if( chn == 1 )
	{
	  wd->msg.wflags |= 0x8000;
	}
	else
	{
		wd->msg.wflags &=~ 0x8000;
	}
	/* set ch1 */
	wd->msg.x = pwin->msg.x;
	wd->msg.y = vertical_pixel_total / 2 + TOP_REMAIN_PIXEL - 6  + 2 + (chn - 1) * 100 + 50; /* 20 * 12 */
	/* not supply size now */
#if 0
	wd->msg.x_size = 0;
	wd->msg.y_size = 0;
#endif
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = osc_draw_chn_arrow;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);	
}
void osc_calculate_trig_arrow(window_def * pwin,widget_def *wd,int chn)
{
	/* set param */
	/* def*/
	unsigned short vertical_pixel_total,hori_pixel;
	/* get msg */
	if( osc_grid_fast(pwin->dev,&hori_pixel,&vertical_pixel_total) == FS_ERR )
	{
		return;
	}		
	/* calbrate the pox */
	/* get chn */
	if( chn == 1 )
	{
	  wd->msg.wflags |= 0x8000;
	}
	else
	{
		wd->msg.wflags &=~ 0x8000;
	}
	/* set ch1 */
	wd->msg.x = pwin->msg.x + hori_pixel + LEFT_REMAIN_PIXEL + 3;
	wd->msg.y = vertical_pixel_total / 2 + TOP_REMAIN_PIXEL - 6 + 1; /* 20 * 12 */
	/* not supply size now */
#if 0
	wd->msg.x_size = 0;
	wd->msg.y_size = 0;
#endif
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = osc_draw_trig_arrow;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);	
}
/* create the voltage title */
void osc_calculate_title_string(window_def * pwin,widget_def *wd,int chn,char ** fast_title)
{
	/* if num is not epue */
	if( chn == 0 )
	{
		return;//reutrn,bad data
	}	
	/* set param */
	/* def*/
	for( int i = 0 ; i < chn ; i ++ )
	{
		/* default is 16 size */
		wd[i].msg.wflags &=~ 0x1000;
	  /* color */
	  if( i == 0 )
		{
			wd[i].msg.wflags |= 0x2000;
		}
		else if( i == 1 )
		{
			wd[i].msg.wflags |= 0x4000;
		}
		else
		{
			wd[i].msg.wflags |= 0x0000;
		}
		/* set ch1 */
		wd[i].msg.x = pwin->msg.x_size / 8 * ( i + 1 );
		wd[i].msg.y = 0;
		/* not supply size now */
#if 0
		wd[i].msg.x_size = 0;
		wd[i].msg.y_size = 0;
#endif
		wd[i].msg.pri_data = fast_title[i];
		/* parent */
		wd[i].dev = pwin->dev;
		wd[i].parent = pwin;
		wd[i].draw = gui_dynamic_string;
		/* other */
		wd[i].peer_linker = 0;
		/* create the wisget */
		gui_widget_creater(&wd[i]);		
	}
}
/* create the base voltage icon */
static void osc_draw_trig_line(widget_def * wd)
{
	/* get area */
	unsigned char rehide_flag = 0;
	draw_area_def * area = get_draw_area_msg();
	/* color table */
	const unsigned char color_table_trig_lines[2][2] = 
	{
		{
			COLOR_CH1_GR_TRIG,
			COLOR_CH1_BG_TRIG,
		},
		{
			COLOR_CH2_GR_TRIG,
			COLOR_CH2_BG_TRIG,			
		}
	};
	/* check rehide */
	if( CHECK_REHIDE(wd->msg.wflags) || wd->msg.mark_flag == 2 )
	{
		/* set */
		rehide_flag = 1;
		/* set deft */
		if( CHECK_REHIDE(wd->msg.wflags) )
		{
			wd->msg.mark_flag = 1;
		}
		/* clear */
		CLEAR_REHIDE(wd->msg.wflags);		
	}
	else
	{
		wd->msg.mark_flag = 1;
	}	
	/* get color */
  const unsigned char * chn = ( wd->msg.wflags & 0x8000 ) ? color_table_trig_lines[0] : color_table_trig_lines[1];
	/* while */
	while(wd->msg.mark_flag)
	{
		/* set point */
		for( int i = area->start_pos_x ; i < area->stop_pos_x ; i +=2 )
		{
			/* get point */
			unsigned char piox = wd->dev->read_point(i,wd->msg.y);
			/* rehide */
			if( rehide_flag == 0 )
			{
				/* chn */
				if( piox == COLOR_GRID_POINT ) // chn1
				{
					wd->dev->set_noload_point(i,wd->msg.y,chn[0]);
				}
				else if( piox == COLOR_GRID_AREA_BG )
				{
					wd->dev->set_noload_point(i,wd->msg.y,chn[1]);
				}
				else
				{
					/* draw no point */
				}
			}
			else
			{
				/* clear */
				if( piox == chn[0] )
				{
					wd->dev->set_noload_point(i,wd->msg.y,COLOR_GRID_POINT);
				}
				else if( piox == chn[1] )
				{
					wd->dev->set_noload_point(i,wd->msg.y,COLOR_GRID_AREA_BG);
				}
				else
				{
					/* draw no point */
				}			
			}
			/* set point */
		}
		/* fe */
		wd->msg.mark_flag --;
		/* check */
		if( wd->msg.mark_flag )
		{
			/* repos */
			wd->msg.y = wd->msg.my;
			/* redraw */
			rehide_flag = 0;
			/* reduce */
		}	
	}
}
/* create the trig lines */
void osc_calculate_trig_line(window_def * pwin,widget_def *wd,int chn)
{
	/* get chn */
	if( chn == 1 )
	{
	  wd->msg.wflags |= 0x8000;
		wd->msg.y = 100; /* for test */
	}
	else
	{
		wd->msg.wflags &=~ 0x8000;
		wd->msg.y = 0x16A + 6; /* for test */
	}
	/* set default is hide */
	SET_HIDE(wd->msg.wflags);
	/* set ch1 */
	wd->msg.x = 0;
	/* parent */
	wd->dev = pwin->dev;
	wd->parent = pwin;
	wd->draw = osc_draw_trig_line;
	/* other */
	wd->peer_linker = 0;
	/* create the wisget */
	gui_widget_creater(wd);	
}
















































