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
#include "gui.h"
#include "fos.h"
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
/* menu win color table */
#define COLOR_MENU_ONE         (RGB(183, 83, 7))	/* color table */
#define COLOR_MENU_TWO         (RGB(255, 155, 7))
#define COLOR_MENU_THR         (RGB(47, 35, 23))
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
/* draw group win */
int draw_group_win(gui_dev_def * dev,gui_info_def * info)
{
	/* size and corner dis */
	const unsigned short mod0 = 0x0364;
	const unsigned short mod1 = 0x4630;
	const unsigned short mod2 = 0x0C62;
	const unsigned short mod3 = 0x26C0;
  /* calutie and ending pos */
	unsigned short  pos_x = info->x , pos_y = info->y;
  /* calutie and ending pos */
	unsigned short pos_x_end = info->x_end , pos_end_y = info->y_end;
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
				dev->set_point( j + pos_x + 1 ,pos_y + i + 1,COLOR_GRID_AREA_BG);
			}
		}
	}
  /* corner size */
	for( int i = 0 ; i < 16 ; i ++ )
	{
		/* left */
		if( ( mod0  << i ) & 0x8000 )
		{
			dev->set_point( i % 4 + pos_x ,i / 4 + pos_y,COLOR_GRID_POINT);
		}
		/* left down */
		if( ( mod1  << i ) & 0x8000 )
		{
			dev->set_point( i % 4 + pos_x ,i / 4 + pos_y + pos_end_y - 4 ,COLOR_GRID_POINT);
		}
    /* right up */
		if( ( mod2  << i ) & 0x8000 )
		{
			dev->set_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y,COLOR_GRID_POINT);
		}
		/* right down */
		if( ( mod3  << i ) & 0x8000 )
		{
			dev->set_point( i % 4 + pos_x + pos_x_end - 4 ,i / 4 + pos_y + pos_end_y - 4,COLOR_GRID_POINT);
		}
	}
	/* line up and down */
	for( int i = 0 ; i < pos_x_end - 8 ; i ++ )
	{
		dev->set_point(pos_x + 4 + i , pos_y , COLOR_GRID_POINT);
		dev->set_point(pos_x + 4 + i , pos_y + pos_end_y - 1, COLOR_GRID_POINT);
	}
  /* line left and right */
	for( int i = 0 ; i < pos_end_y - 8 ; i ++ )
	{
		dev->set_point(pos_x , pos_y + 4 + i , COLOR_GRID_POINT);
		dev->set_point(pos_x + pos_x_end - 1, pos_y + 4 + i , COLOR_GRID_POINT);
	}	
	/* return */
	return FS_OK;
}
/* draw the menu window */
int draw_menu_win( gui_dev_def * dev,gui_info_def * info )
{
	/* transfer the menu size */
	unsigned short MENU_WIDTH = info->x_end;//107;
	unsigned short MENU_HEIGHT = info->y_end;//430;
	/* start pos */
	unsigned short pos_x_m = info->x; //dev->width - MENU_WIDTH;
	unsigned short pos_y_m = info->y; //0
	/* draw point data */
	const unsigned char MENU_LT_0[5] = { 0xC0,0x80,0x00,0x07,0x0f };/*63.75.151*/
	const unsigned char MENU_LT_1[5] = { 0x3f,0x60,0xD0,0xA0,0x80 };/* 183,83,7 */
	const unsigned char MENU_LT_2[5] = { 0x00,0x1F,0x20,0x40,0x40 };/* 255,155,7*/
	const unsigned char MENU_LT_3[5] = { 0x00,0x00,0x0F,0x18,0x30 };/* 47,35,23 */
	/* draw point data */
	const unsigned short MENU_LB_0 = 0x008C;
	const unsigned short MENU_LB_1 = 0x8C63;
	const unsigned short MENU_LB_2 = 0x6310;
	const unsigned short MENU_LB_3 = 0x1000;
	 /*fill the rect */
#if HARDWARE_ACCEL_SUPPLY
  /* fill rect */
	dev->fill_rect( pos_x_m , pos_y_m , 
	                pos_x_m + MENU_WIDTH - 1 , pos_y_m + MENU_HEIGHT - 1,
	                COLOR_BACKGROUND );//background color
#else
	/* draw points one by one */
	for (int i = 0; i < MENU_WIDTH; i++)
	{
		for (int j = 0; j < MENU_HEIGHT; j++)
		{
			dev->set_point(pos_x_m + i , pos_y_m + j, COLOR_BACKGROUND);
		}
	}	
#endif
	/* left top */
	for( int i = 0; i < 5*8; i++ )
	{
		if ((MENU_LT_0[i / 8] << (i % 8)) & 0x80)
		{
			dev->set_point(pos_x_m + i % 8, pos_y_m + i / 8, COLOR_BACKGROUND);
		}
		if ((MENU_LT_1[i / 8] << (i % 8)) & 0x80)
		{
			dev->set_point(pos_x_m + i % 8, pos_y_m + i / 8, COLOR_MENU_ONE);
		}
		if ((MENU_LT_2[i / 8] << (i % 8)) & 0x80)
		{
			dev->set_point(pos_x_m + i % 8, pos_y_m + i / 8, COLOR_MENU_TWO);
		}
		if ((MENU_LT_3[i / 8] << (i % 8)) & 0x80)
		{
			dev->set_point(pos_x_m + i % 8, pos_y_m + i / 8, COLOR_MENU_THR);
		}
	}
	/* left bottom */
	for( int i = 0; i < 4 * 4; i++ )
	{
		if ((MENU_LB_0 << i) & 0x8000)
		{
			dev->set_point(pos_x_m + i % 4, pos_y_m + i / 4 + MENU_HEIGHT - 4, COLOR_BACKGROUND);
		}
		if ((MENU_LB_1 << i) & 0x8000)
		{
			dev->set_point(pos_x_m + i % 4, pos_y_m + i / 4 + MENU_HEIGHT - 4, COLOR_MENU_ONE);
		}
		if ((MENU_LB_2 << i) & 0x8000)
		{
			dev->set_point(pos_x_m + i % 4, pos_y_m + i / 4 + MENU_HEIGHT - 4, COLOR_MENU_TWO);
		}
		if ((MENU_LB_3 << i) & 0x8000)
		{
			dev->set_point(pos_x_m + i % 4, pos_y_m + i / 4 + MENU_HEIGHT - 4, COLOR_MENU_THR);
		}
	}
	/* top line */
	for (int i = 0; i < MENU_WIDTH - 8; i++)
	{
		dev->set_point(pos_x_m + i + 8, pos_y_m, COLOR_MENU_ONE);
		dev->set_point(pos_x_m + i + 8, pos_y_m + 1, COLOR_MENU_TWO);
		dev->set_point(pos_x_m + i + 8, pos_y_m + 2, COLOR_MENU_THR);
	}
	/* bottom line */
	for (int i = 0; i < MENU_WIDTH - 4; i++)
	{
		dev->set_point(pos_x_m + i + 4, pos_y_m + MENU_HEIGHT - 1, COLOR_MENU_ONE);
		dev->set_point(pos_x_m + i + 4, pos_y_m + MENU_HEIGHT - 2 , COLOR_MENU_TWO);
		dev->set_point(pos_x_m + i + 4, pos_y_m + MENU_HEIGHT - 3, COLOR_MENU_THR);
	}
	/* left lines */
	for (int i = 0; i < MENU_HEIGHT - 4 - 5; i++)
	{
		dev->set_point(pos_x_m, pos_y_m + i + 5, COLOR_MENU_ONE);
		dev->set_point(pos_x_m + 1, pos_y_m + i + 5, COLOR_MENU_TWO);
		dev->set_point(pos_x_m + 2, pos_y_m + i + 5, COLOR_MENU_THR);
	}
	/* ex two point */
	dev->set_point(pos_x_m + 3, pos_y_m + MENU_HEIGHT - 5, COLOR_MENU_THR);
	dev->set_point(pos_x_m + 3 + 1, pos_y_m + MENU_HEIGHT - 5 + 1, COLOR_MENU_THR);	
	/* return */
	return FS_OK;
}





















