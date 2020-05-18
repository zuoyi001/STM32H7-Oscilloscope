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

#ifndef __OSC_UI_H__
#define __OSC_UI_H__

#include "gui.h"

/* Includes ------------------------------------------------------------------*/
int osc_create_ui(void);
void osc_create_button(struct widget * widget);
void osc_create_ch1_micon(struct widget * widget);
void osc_create_ch2_micon(struct widget * widget);
static void osc_main_draw(window_def * win);
static void osc_group_draw(window_def * win);
static void osc_menu_win_draw(window_def * win);
void osc_ui_time_str(char * str);
void osc_ui_vol_str(unsigned char chn,char * str);
void osc_ui_tips_str(char * str);
void osc_ui_move_offset_arrow(unsigned char chn,unsigned short pos);
void osc_calculate_trig_arrow(window_def * pwin,widget_def *wd,int chn);
void osc_ui_move_trig_arrow(unsigned char chn,unsigned short pos);
void osc_ui_vol_scale(unsigned char chn , char * str );
void osc_ui_move_trig_lines(unsigned char chn,unsigned short posy);
void osc_ui_trig_lines_show(unsigned char chn,unsigned char mode);
void osc_ui_measure_capital(unsigned char chn,unsigned char item,const char * capital);
void osc_ui_measure_data(unsigned char chn,unsigned char item,char * capital);
int osc_ui_menu_sta(void);
void osc_ui_menu_show(unsigned char mode);
void osc_ui_trig_arrow_show(unsigned char chn,unsigned char mode);
int osc_ui_trig_arrow_sta(unsigned char chn);
void osc_ui_set_menu_text_group(const char ** text,unsigned int len);
void osc_ui_set_one_menu_text(unsigned char item,const char * text);
void osc_ui_set_chn_text(unsigned char chn,const char * text);
void osc_ui_set_csh_show(unsigned char chn,unsigned char mode);
void osc_ui_set_trig_text(const char * text);
void osc_ui_set_trig_src(const char * text);
void osc_ui_show_offset_arrow(unsigned char chn , unsigned char mode);
/* Private includes ----------------------------------------------------------*/

#endif

