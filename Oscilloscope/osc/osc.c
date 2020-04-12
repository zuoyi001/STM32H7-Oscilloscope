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
			win->dev->set_point(i, j, COLOR_BACK_GROUND);
		}

		for (int i = 0; i < ENR; i++)
		{
			win->dev->set_point(i + LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) +1 +1, j, COLOR_BACK_GROUND);
		}
	}
/* draw other grid data */
	for (unsigned int j = 0; j < win->dev->width; j++)
	{
		for (int i = 0; i < TOP_REMAIN_PIXEL; i++)
		{
			win->dev->set_point(j, i, COLOR_BACK_GROUND);
		}
/* draw other grid data */
		for (int i = 0; i < EMRR - 1; i++)
		{
			win->dev->set_point(j, i + TOP_REMAIN_PIXEL + 1 + 1 + 1 + EM * (VERTICAL_GRID_TOTAL), COLOR_BACK_GROUND);
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
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + j, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_AREA_BG);
		}
	}
#endif
  /* draw other grid data */
	for(int j = 0; j < ( VERTICAL_GRID_NUM + 1 ); j++ )
	{
		for (int i = 0 ; i < ( HORIZONTAL_GRID_TOTAL + 1 ); i++ )
		{
			win->dev->set_point( LEFT_REMAIN_PIXEL + 1 + i * EN, TOP_REMAIN_PIXEL + 1 + j * EM * LITTLE_GRIG_NUM , COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int j = 0 ; j < ( HORIZONTAL_GRID_NUM + 1 ) ; j++ )
	{
		for (int i = 0; i < (VERTICAL_GRID_TOTAL + 1); i++ )
		{
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + j * EN * LITTLE_GRIG_NUM , TOP_REMAIN_PIXEL + 1 + i * EM, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0 ; i < EN * ( HORIZONTAL_GRID_TOTAL ) ; i++ )
	{
	  /* draw other grid data */ 
		win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( i % EN ) == 0 )
		{
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 2, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 5, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EN)) == 0 )
		{
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 3, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 6, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL + 1  + i, COLOR_GRID_POINT);
		win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		/* draw other grid data */
		if((( i % EM )) == 0 )
		{
			/* draw other grid data */
			win->dev->set_point(LEFT_REMAIN_PIXEL + 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			/* draw other grid data */
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 2 , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for( int i = 0; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		/* draw other grid data */
		win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2), COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( (i % EN) ) == 0)
		{
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 2, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 5, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 1, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 4, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2), TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EM)) == 0)
		{
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 1, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 4, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
    /* draw other grid data */
		if (((i % (EM * LITTLE_GRIG_NUM))) == 0)
		{
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 8, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			win->dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 7, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	win->dev->set_point(LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
	win->dev->set_point(LEFT_REMAIN_PIXEL, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	win->dev->set_point(LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL, COLOR_GRID_POINT);
	win->dev->set_point(LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
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
		
		win->dev->set_point( LEFT_REMAIN_PIXEL + 1 + i , tm , RGB(255,255,7));
		
		win->dev->set_point( LEFT_REMAIN_PIXEL + 1 + i , tcm , RGB(7,227,231));
		
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
				win->dev->set_point( j + pos_x + 1 ,pos_y + i + 1,COLOR_GRID_AREA_BG);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			win->dev->set_point( i % 4 + pos_x ,i / 4 + pos_y,COLOR_GRID_POINT);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			win->dev->set_point( i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_GRID_POINT);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			win->dev->set_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_GRID_POINT);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			win->dev->set_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_GRID_POINT);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		win->dev->set_point(pos_x + 4 + i , pos_y , COLOR_GRID_POINT);
		win->dev->set_point(pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_GRID_POINT);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		win->dev->set_point(pos_x , pos_y + 4 + i , COLOR_GRID_POINT);
		win->dev->set_point(pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_GRID_POINT);
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
			win->dev->set_point(pos_x_m + i , pos_y_m + j, COLOR_BACK_GROUND);
		}
	}	
#endif
	/* top line */
	for (int i = 0; i < MENU_WIDTH; i++)
	{
		win->dev->set_point(pos_x_m + i, pos_y_m, COLOR_MENU_ONE);
		win->dev->set_point(pos_x_m + i, pos_y_m + 1, COLOR_MENU_ONE);
		win->dev->set_point(pos_x_m + i, pos_y_m + 2, COLOR_MENU_ONE);
    /* bottom line */
		win->dev->set_point(pos_x_m + i, pos_y_m + MENU_HEIGHT - 1, COLOR_MENU_ONE);
		win->dev->set_point(pos_x_m + i, pos_y_m + MENU_HEIGHT - 2 , COLOR_MENU_ONE);
		win->dev->set_point(pos_x_m + i, pos_y_m + MENU_HEIGHT - 3, COLOR_MENU_ONE);
	}
	/* left lines */
	for (int i = 0; i < MENU_HEIGHT - 6; i++)
	{
		win->dev->set_point(pos_x_m, pos_y_m + i + 3, COLOR_MENU_ONE);
		win->dev->set_point(pos_x_m + 1, pos_y_m + i + 3, COLOR_MENU_ONE);
		win->dev->set_point(pos_x_m + 2, pos_y_m + i + 3, COLOR_MENU_ONE);
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
			win->dev->set_point(j , i , ( color[i*40+j] == 0xffff ) ? COLOR_GRID_AREA_BG : COLOR_BACK_GROUND);
#else
			unsigned short tm = color[i*32+j];

			win->dev->set_point(parent_x + pos_x + j , parent_y + pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}
}
/* create icon */
static void osc_create_chn_icon(window_def * parent_win,unsigned short x,unsigned short y,unsigned short chn)
{
	/* color tmp */
	unsigned short * color;
	/* switch chn */
	if( chn == 1 )
	{
		color = (unsigned short *)CH1_ICON_M;
	}
	else
	{
		color = (unsigned short *)CH2_ICON_M;
	}
	/* get parent win pos */
	unsigned short parent_x = parent_win->msg.x;
	unsigned short parent_y = parent_win->msg.y;
	/* widget pos */
	unsigned short pos_x = x;
	unsigned short pos_y = y;
	/* create */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		for( int j = 0 ; j < 32 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			parent_win->dev->set_point(parent_x + pos_x + j , parent_y + pos_y + i , color[i*32+j]);
#else
			unsigned short tm = color[i*32+j];

			widget->dev->set_point(parent_x + pos_x + j , parent_y + pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
		}
	}
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
			widget->dev->set_point(pos_x_m + pos_btn_x + 2 + i, pos_y_m + pos_btn_y + j + 2, COLOR_BUTTON);
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
	win->msg.wflags = wf;
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
	win0[0].msg.wflags = 0x0100;
	/* set callback */
	win0[0].draw = draw_group_win;
	/* create the group */
	win0[1].msg.x = 1;//win0[0].msg.x + win0[0].msg.x_size + 2;
	win0[1].msg.y = win0[0].msg.y;
	win0[1].msg.x_size = ( dev->width - win0[0].msg.x_size ) * 2 / 3;
	win0[1].msg.y_size = (dev->height - vertical_pixel_total - TOP_REMAIN_PIXEL - 2 - 8) / 2 - 1;
	win0[1].dev = dev;
	win0[1].msg.wflags = 0x0200;
	/* set callback */
	win0[1].draw = draw_group_win;
	/* create the group */
	win0[2].msg.x = win0[1].msg.x;
	win0[2].msg.y = win0[1].msg.y + win0[1].msg.y_size + 1;
	win0[2].msg.x_size = dev->width - win0[0].msg.x_size - 5;
	win0[2].msg.y_size = win0[1].msg.y_size - 1;
	win0[2].dev = dev;
	win0[2].msg.wflags = 0x0400;
	/* set callback */
	win0[2].draw = draw_group_win;
	/* create the group */
	win0[3].msg.x = win0[1].msg.x + win0[1].msg.x_size + 2;
	win0[3].msg.y = win0[1].msg.y;
	win0[3].msg.x_size = dev->width - win0[0].msg.x_size - win0[1].msg.x_size - 6;
	win0[3].msg.y_size = win0[1].msg.y_size;
	win0[3].dev = dev;
	win0[3].msg.wflags = 0x0800;
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
	win->msg.wflags = wf;
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
/* */
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
		/* char default is 24 size */
		wd[i].msg.wflags |= 0x1000;
		/* set system title */
		if( i == 0 )
		{
			wd[i].msg.x = title_x;
			/* vertical */
			wd[i].msg.y = title_y;
		}
		else
		{
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
/* create the base voltage icon */
static void osc_draw_chn_arrow(widget_def * wd)
{
	/* color tmp */
	unsigned short * color;
	/* switch chn */
	if( wd->msg.wflags & 0x8000 )
	{
		color = (unsigned short *)CH1_ARROW;
	}
	else
	{
		color = (unsigned short *)CH2_ARROW;
	}
	/* widget pos */
	unsigned short pos_x = wd->msg.x;
	unsigned short pos_y = wd->msg.y;
	/* create */
	for( int i = 0 ; i < 12 ; i ++ )
	{
		for( int j = 0 ; j < 20 ; j ++ )
		{
#ifndef _VC_SIMULATOR_
			wd->dev->set_point(pos_x + j , pos_y + i , color[i*20+j]);
#else
			unsigned short tm = color[i*20+j];

			widget->dev->set_point(pos_x + j , pos_y + i , RGB((tm&0xF100) >> 8 ,(tm&0x7E0) >> 3 , (tm&0x1F) << 3 ));
#endif
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
	wd->msg.y = vertical_pixel_total / 2 + TOP_REMAIN_PIXEL - 6  + 2 + (chn - 1) * 100 ; /* 20 * 12 */
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






















