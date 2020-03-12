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
#include "display_dev.h"
#include "math.h"
#include "osc.h"
/* Includes ------------------------------------------------------------------*/
#define VERTICAL_GRID_NUM      (8)
#define HORIZONTAL_GRID_NUM    (10)
#define LITTLE_GRIG_NUM        (5)
#define VERTICAL_GRID_TOTAL    (VERTICAL_GRID_NUM * LITTLE_GRIG_NUM)
#define HORIZONTAL_GRID_TOTAL  (HORIZONTAL_GRID_NUM * LITTLE_GRIG_NUM)
/* left remind and up */
#define TOP_REMAIN_PIXEL       (16)
#define LEFT_REMAIN_PIXEL      (20)
#define BOTTOM_REMAIN_PIXEL    (45)
/* THREE color */
#define COLOR_GRID_POINT       (RGB(199, 195, 199))
#define COLOR_GRID_AREA_BG     (RGB(7, 3, 7))
#define COLOR_BACKGROUND       (RGB(63, 75, 151))
/* draw area defines */
static draw_area_def draw_area;
/* define some nes message */
static unsigned short EN,EM;
#if !HARDWARE_ACCEL_SUPPLY /* no use without hardware accel */
static unsigned short ENR,EMR;
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
	EMR = height - EM * (VERTICAL_GRID_TOTAL) - TOP_REMAIN_PIXEL - 1 - 1;
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
	draw_area.start_pos_y = TOP_REMAIN_PIXEL  + 1 + EM * (VERTICAL_GRID_TOTAL);
	/* num grid of horizontal and vertical */
	draw_area.num_horizontal = HORIZONTAL_GRID_NUM;
	draw_area.num_vertical = VERTICAL_GRID_NUM;
	/* num pixel for all horizontal */
	draw_area.pixel_horizontal = LITTLE_GRIG_NUM * EN;
	draw_area.pixel_vertiacl = LITTLE_GRIG_NUM * EM;
	/* num of little grid */
	draw_area.little_grid = LITTLE_GRIG_NUM;
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
		return (void *)(0) ; // not init yet 
	}
	/* return bat */
	return &draw_area;
}
/* create grid data */
int create_grid_data(gui_dev_def * dev)
{	
  /* get gui dev */
	/* set grid data */
	int ret = grid_grobal_data( dev->width , dev->height );
	/* ok or not */
	if( ret != 0 )
	{
		/* oh no , feels not good , What seems to be the problem */
		return (-1);		
	}
  /* sraw */
#if HARDWARE_ACCEL_SUPPLY
	/* clear all area with one color */
	dev->clear_display_dev(COLOR_BACKGROUND);
	/* if doesn't has the hardware accel */
#else
	for (int j = 0; j < dev->width; j++)
	{
		for (int i = 0; i < LEFT_REMAIN_PIXEL; i++)
		{
			dev->set_point(i, j, COLOR_BACKGROUND);
		}

		for (int i = 0; i < ENR; i++)
		{
			dev->set_point(i + LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) +1 +1, j, COLOR_BACKGROUND);
		}
	}
/* draw other grid data */
	for (int j = 0; j < dev->height; j++)
	{
		for (int i = 0; i < TOP_REMAIN_PIXEL; i++)
		{
			dev->set_point(j, i, COLOR_BACKGROUND);
		}
/* draw other grid data */
		for (int i = 0; i < EMR - 1; i++)
		{
			dev->set_point(j, i + TOP_REMAIN_PIXEL + 1 + 1 + 1 + EM * (VERTICAL_GRID_TOTAL), COLOR_BACKGROUND);
		}
	}
#endif
/* create the data area background color */
#if HARDWARE_ACCEL_SUPPLY
/* full the rect area with some color */
  dev->fill_rect(LEFT_REMAIN_PIXEL + 1,TOP_REMAIN_PIXEL + 1,
	               LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) , 
								 TOP_REMAIN_PIXEL  + EM * (VERTICAL_GRID_TOTAL),COLOR_GRID_AREA_BG);
#else
	for (int j = 0; j < EN * (HORIZONTAL_GRID_TOTAL) ; j++)
	{
		for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1 ; i++)
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + j, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_AREA_BG);
		}
	}
#endif
/* draw other grid data */
	for(int j = 0; j < ( VERTICAL_GRID_NUM + 1 ); j++ )
	{
		for (int i = 0 ; i < ( HORIZONTAL_GRID_TOTAL + 1 ); i++ )
		{
			dev->set_point( LEFT_REMAIN_PIXEL + 1 + i * EN, TOP_REMAIN_PIXEL + 1 + j * EM * LITTLE_GRIG_NUM , COLOR_GRID_POINT);
		}
	}
/* draw other grid data */
	for( int j = 0 ; j < ( HORIZONTAL_GRID_NUM + 1 ) ; j++ )
	{
		for (int i = 0; i < (VERTICAL_GRID_TOTAL + 1); i++ )
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + j * EN * LITTLE_GRIG_NUM , TOP_REMAIN_PIXEL + 1 + i * EM, COLOR_GRID_POINT);
		}
	}
/* draw other grid data */
	for( int i = 0 ; i < EN * ( HORIZONTAL_GRID_TOTAL ) ; i++ )
	{
	  /* draw other grid data */ 
		dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( i % EN ) == 0 )
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 2, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 5, COLOR_GRID_POINT);
		}
	}
/* draw other grid data */
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EN)) == 0 )
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 3, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL) + 1 - 6, COLOR_GRID_POINT);
		}
	}
/* draw other grid data */
	for( int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		dev->set_point(LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL + 1  + i, COLOR_GRID_POINT);
		dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		/* draw other grid data */
		if((( i % EM )) == 0 )
		{
			/* draw other grid data */
			dev->set_point(LEFT_REMAIN_PIXEL + 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			/* draw other grid data */
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 2 , TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
/* draw other grid data */
	for( int i = 0; i < EN * (HORIZONTAL_GRID_TOTAL); i++)
	{
		/* draw other grid data */
		dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2), COLOR_GRID_POINT);
    /* draw other grid data */
		if( ( (i % EN) ) == 0)
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 2, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) - 5, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 1, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + i, TOP_REMAIN_PIXEL + 1 + EM * (VERTICAL_GRID_TOTAL / 2) + 4, COLOR_GRID_POINT);
		}
	}
/* draw other grid data */
	for (int i = 0; i < EM * (VERTICAL_GRID_TOTAL) + 1; i++)
	{
		/* draw other grid data */
		dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2), TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
    /* draw other grid data */
		if (((i % EM)) == 0)
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 2, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 5, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 1, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 4, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
    /* draw other grid data */
		if (((i % (EM * LITTLE_GRIG_NUM))) == 0)
		{
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) - 8, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
			dev->set_point(LEFT_REMAIN_PIXEL + 1 + EN * (HORIZONTAL_GRID_TOTAL / 2) + 7, TOP_REMAIN_PIXEL + 1 + i, COLOR_GRID_POINT);
		}
	}
  /* draw other grid data */
	dev->set_point(LEFT_REMAIN_PIXEL , TOP_REMAIN_PIXEL , COLOR_GRID_POINT);
	dev->set_point(LEFT_REMAIN_PIXEL, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	dev->set_point(LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL, COLOR_GRID_POINT);
	dev->set_point(LEFT_REMAIN_PIXEL + EN * (HORIZONTAL_GRID_TOTAL) + 1, TOP_REMAIN_PIXEL + EM * (VERTICAL_GRID_TOTAL) + 1 + 1, COLOR_GRID_POINT);
	/* for test */
#if 1
  /* draw other grid data */
	double sin_x = 0;
	/* sinx */
	for( int i = 0 ; i < EN * (HORIZONTAL_GRID_TOTAL) ; i ++ )
	{
	 double te = sin( sin_x );
		
   short tm = (short)( te * EM * (VERTICAL_GRID_TOTAL / 4) ) + EM * (VERTICAL_GRID_TOTAL / 2) + TOP_REMAIN_PIXEL + 1 - EM*LITTLE_GRIG_NUM;
		
	 double tce = cos( sin_x );
		
   short tcm = (short)( tce * EM * (VERTICAL_GRID_TOTAL / 4) ) + EM * (VERTICAL_GRID_TOTAL / 2) + TOP_REMAIN_PIXEL + 1 + EM*LITTLE_GRIG_NUM;	 
		
		dev->set_point( LEFT_REMAIN_PIXEL + 1 + i , tm , RGB(255,255,7));
		
		dev->set_point( LEFT_REMAIN_PIXEL + 1 + i , tcm , RGB(7,227,231));
		
		sin_x += (6.28) / (double)(EN * (HORIZONTAL_GRID_TOTAL)) * 3;
		
	}
#endif
/* set the flag */
	grid_init_flag = 1;
/* end of func */	
	return 0;
}

