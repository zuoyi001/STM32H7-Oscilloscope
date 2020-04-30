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
#include "display_dev.h"
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
/* L8 mode color table */
#if LCD_MODE_L8
/* RGB color to L8 color */
#define RGTB(r,g,b)  ((r << 16) | (g << 8) | (b))
/* */
#define COLOR_GRID_POINT       (0)//(RGB(199, 195, 199))
#define COLOR_GRID_AREA_BG     (1)//(RGB(7, 3, 7))
#define COLOR_BACK_GROUND      (2)//(RGB(63, 75, 151))
/* menu win color table */
#define COLOR_MENU_ONE         (3)//(RGB(183, 83, 7))	/* color table */
/* button color table */
#define COLOR_BUTTON           (4)//(RGB(87,131,231))
/* char and hz table */
#define COLOR_CHAR             (5)//(RGB(255,255,255))
/* lines color */
#define COLOR_CH1              (6)//(RGB(255,255,7))
#define COLOR_CH2              (7)//(RGB(7,227,231))
/* TIPS_COLOR */
#define COLOR_TIPS_ERROR       (8)//(RGB(255,0,0))
/* CHN lines color */
#define COLOR_CH1_BG_0         (9)//(RGB(255,255,7))
#define COLOR_CH1_BG_1         (10)//(RGB(255,255,7))
#define COLOR_CH1_BG_F         (11)//(RGB(255,255,7))
#define COLOR_CH1_GR_0         (12)//(RGB(255,255,7))
#define COLOR_CH1_GR_1         (13)//(RGB(255,255,7))
#define COLOR_CH1_GR_F         (14)//(RGB(255,255,7))
/* CHN lines color */
#define COLOR_CH2_BG_0         (15)//(RGB(7,227,231))
#define COLOR_CH2_BG_1         (16)//(RGB(7,227,231))
#define COLOR_CH2_BG_F         (17)//(RGB(7,227,231))
#define COLOR_CH2_GR_0         (18)//(RGB(7,227,231))
#define COLOR_CH2_GR_1         (19)//(RGB(7,227,231))
#define COLOR_CH2_GR_F         (20)//(RGB(7,227,231))
/* math0 lines color */
#define COLOR_MATH0_BG_0       (21)//(RGB(255,0,0))
#define COLOR_MATH0_BG_1       (22)//(RGB(255,0,0))
#define COLOR_MATH0_BG_F       (23)//(RGB(255,0,0))
#define COLOR_MATH0_GR_0       (24)//(RGB(255,0,0))
#define COLOR_MATH0_GR_1       (25)//(RGB(255,0,0))
#define COLOR_MATH0_GR_F       (26)//(RGB(255,0,0))
/* math1 lines color */
#define COLOR_MATH1_BG_0       (27)//(RGB(0,255,0))
#define COLOR_MATH1_BG_1       (28)//(RGB(0,255,0))
#define COLOR_MATH1_BG_F       (29)//(RGB(0,255,0))
#define COLOR_MATH1_GR_0       (30)//(RGB(0,255,0))
#define COLOR_MATH1_GR_1       (31)//(RGB(0,255,0))
#define COLOR_MATH1_GR_F       (32)//(RGB(0,255,0))
/* math2 lines color */
#define COLOR_MATH2_BG_0       (33)//(RGB(0,0,255))
#define COLOR_MATH2_BG_1       (34)//(RGB(0,0,255))
#define COLOR_MATH2_BG_F       (35)//(RGB(0,0,255))
#define COLOR_MATH2_GR_0       (36)//(RGB(0,0,255))
#define COLOR_MATH2_GR_1       (37)//(RGB(0,0,255))
#define COLOR_MATH2_GR_F       (38)//(RGB(0,0,255))
/* trig lines */
#define COLOR_CH1_GR_TRIG      (39)//(RGB(7,227,231))
#define COLOR_CH1_BG_TRIG      (40)//(RGB(7,227,231))
#define COLOR_CH2_GR_TRIG      (41)//(RGB(255,255,7))
#define COLOR_CH2_BG_TRIG      (42)//(RGB(255,255,7))
/* typdef chn line manage */
typedef struct
{
	unsigned char BG_0;
	unsigned char BG_1;
	unsigned char BG_F;
	unsigned char GR_0;
	unsigned char GR_1;
	unsigned char GR_F;
}chn_manage_def;
/* color table */
#define COLOR_TABLE_L8         \
{                              \
RGTB(199, 195, 199),   /* 0  */\
RGTB(7, 3, 7),         /* 1  */\
RGTB(63, 75, 151),     /* 2  */\
RGTB(183, 83, 7),      /* 3  */\
RGTB(87,131,231),      /* 4  */\
RGTB(255,255,255),     /* 5  */\
RGTB(255,255,7),       /* 6  */\
RGTB(7,227,231),       /* 7  */\
RGTB(255,0,0),         /* 8  */\
RGTB(255,255,7),       /* 9  */\
RGTB(255,255,7),       /* 10 */\
RGTB(255,255,7),       /* 11 */\
RGTB(255,255,7),       /* 12 */\
RGTB(255,255,7),       /* 13 */\
RGTB(255,255,7),       /* 14 */\
RGTB(7,227,231),       /* 15 */\
RGTB(7,227,231),       /* 16 */\
RGTB(7,227,231),       /* 17 */\
RGTB(7,227,231),       /* 18 */\
RGTB(7,227,231),       /* 19 */\
RGTB(7,227,231),       /* 20 */\
RGTB(255,0,0),         /* 21 */\
RGTB(255,0,0),         /* 22 */\
RGTB(255,0,0),         /* 23 */\
RGTB(255,0,0),         /* 24 */\
RGTB(255,0,0),         /* 25 */\
RGTB(255,0,0),         /* 26 */\
RGTB(0,255,0),         /* 27 */\
RGTB(0,255,0),         /* 28 */\
RGTB(0,255,0),         /* 29 */\
RGTB(0,255,0),         /* 30 */\
RGTB(0,255,0),         /* 31 */\
RGTB(0,255,0),         /* 32 */\
RGTB(0,0,255),         /* 33 */\
RGTB(0,0,255),         /* 34 */\
RGTB(0,0,255),         /* 35 */\
RGTB(0,0,255),         /* 36 */\
RGTB(0,0,255),         /* 37 */\
RGTB(0,0,255),         /* 38 */\
RGTB(255,255,0),       /* 39 */\
RGTB(255,255,0),       /* 40 */\
RGTB(7,227,231),       /* 41 */\
RGTB(7,227,231),       /* 42 */\
}
#else
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
#endif
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
static void osc_draw_trig_arrow(widget_def * wd);
void osc_calculate_trig_line(window_def * pwin,widget_def *wd,int chn);

#endif
































