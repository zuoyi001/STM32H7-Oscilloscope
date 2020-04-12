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

#ifndef __OSC_H__
#define __OSC_H__
/* Includes ------------------------------------------------------------------*/

/* Private includes ----------------------------------------------------------*/

#include "gui_dev.h"
#include "gui.h"
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
#define COLOR_BACK_GROUND      (RGB(63, 75, 151))
/* menu win color table */
#define COLOR_MENU_ONE         (RGB(183, 83, 7))	/* color table */
/* button color table */
#define COLOR_BUTTON           (RGB(87,131,231))
/* char and hz table */
#define COLOR_CHAR             (RGB(255,255,255))
/* lines color */
#define COLOR_CH1              (RGB(255,255,7))
#define COLOR_CH2              (RGB(7,227,231))
/* TIPS_COLOR */
#define COLOR_TIPS_ERROR       (RGB(255,0,0))
#define COLOR_TIPS_WARNING     (COLOR_CH1)
#define COLOR_TIPS_NORMAL      (COLOR_CHAR)
/* #define tips level */
#define TIPS_ERROR             (0x6000)
#define TIPS_WARNING           (0x8000)
#define TIPS_NORMAL            (0xA000)
/* trig mode */
#define TRIG_MODE_RISING       (0x0000)
#define TRIG_MODE_FALLING      (0x0001)
#define TRIG_MODE_PULSE        (0x0002)
/* trig source */
#define TRIG_SOURCE_CH1        (0x0000)
#define TRIG_SOURCE_CH2        (0x0001)
#define TRIG_SOURCE_BOTH       (0x0002)
/* Define the area information occupied by the drawing area of the current screen */
typedef struct
{
	unsigned short start_pos_x;
	unsigned short start_pos_y;
	unsigned short stop_pos_x;
	unsigned short stop_pos_y;  
	unsigned short num_vertical;
	unsigned short num_horizontal;
	unsigned short pixel_vertiacl;
	unsigned short pixel_horizontal;
	unsigned short little_grid;
	unsigned short total_pixel_h;
	unsigned short total_pixel_v;
	unsigned short rev0;
	unsigned short rev1;
}draw_area_def;

/* create grid data */
static void osc_create_chn_icon(window_def * parent_win,unsigned short x,unsigned short y,unsigned short chn);
void osc_calculate_main_size(gui_dev_def * dev,window_def * win,unsigned short wf);
void osc_calculate_sg_size(gui_dev_def * dev,window_def * win0,unsigned int num);
void osc_calculate_menu_size(gui_dev_def * dev,window_def * win,unsigned short wf);
void osc_calculate_btn_size(gui_dev_def * dev,window_def * win,widget_def *wd,unsigned short btn_num);
void osc_calculate_volage_string(window_def * pwin,widget_def *wd,int num,char * ch1,char * ch2);
void osc_calculate_time_string(window_def * pwin,widget_def *wd,int num,char * M,char * time);
void osc_calculate_measure_ch(window_def * pwin,widget_def *wd,int num,char ** item,unsigned char ch);
void osc_calculate_menu(window_def * pwin,widget_def *wd,int num,char ** item);
void osc_calculate_tips(window_def * pwin,widget_def *wd,unsigned short level,char * tip);
void osc_calculate_base_arrow(window_def * pwin,widget_def *wd,int chn);
void osc_calculate_title_string(window_def * pwin,widget_def *wd,int chn,char ** fast_title);
static void osc_create_TITLE(window_def * win);
draw_area_def * get_draw_area_msg(void);

#endif
































