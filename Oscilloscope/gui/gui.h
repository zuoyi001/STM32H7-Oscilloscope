/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
#ifndef __GUI_H__
#define __GUI_H__	

#include "gui_dev.h"

/* Includes ------------------------------------------------------------------*/
#define ORIGINAL_PARENT  (0)
/* gui level */
#define GUI_SAME_LEVEL   (0)
#define GUI_CHILD_LEVEL  (1)
/* decleare */
typedef struct win window_def;
/* Message type */
typedef enum
{
	/* Message */
	CLICK = 0,
	DOUCLICK = 1,
	REDRAW = 2,
	TEXTCHANGE = 3,
	POSCHANGE  = 4,
	SIZECHANGE = 5,
	/* end of */
}gui_msg_type;
/* gui msg */
typedef struct gui_info
{
	/* size and pos */
	unsigned short x;
	unsigned short y;
	unsigned short x_end;
	unsigned short y_end;
	/* widget flags */
	unsigned short wflags;
	/* draw methed */
	void (*draw)(struct gui_info *);
	/*  Message response */
	void (*msg_response)(gui_msg_type msg);
	/* end */
}gui_info_def;
/* widget strucr def */
typedef struct widget
{
	/* linker */
	struct widget * peer_linker;
	/* common define */
	gui_info_def msg;
	/* draw methed */
	window_def * parent;
	/* dev */
	gui_dev_def * dev;	
	/* end of func */
}widget_def;
/* win test */
typedef struct win 
{
	/* same win window */
	struct win * win_child;
	/* the linker for the hight def */
	widget_def * wchild;
	/* msg */
	gui_info_def msg;
	/* dev */
	gui_dev_def * dev;	
	/* window off */
}window_def;
/* end if file */

/* Function declaration */
int gui_static_creater(void);
int gui_win_creater(window_def * win);
int gui_widget_creater(window_def * parent , widget_def * widget );
/* end of files */
#endif













