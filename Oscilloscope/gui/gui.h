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

/* setting */
#define GUI_HIDE         (0x0001)
#define GUI_DRAWED       (0x0002)
/* check maros */
#define CHECK_HIDE(a)    ((a&GUI_HIDE)?1:0)
#define SET_HIDE(a)      ((a |= GUI_HIDE))
#define CLEAR_HIDE(a)    ((a &=~ (GUI_HIDE|GUI_DRAWED)))
/* set drawed */
#define CHECH_DRAWED(a)  ((a&GUI_DRAWED)?1:0)
#define SET_DRAWED(a)    ((a |= GUI_DRAWED))
#define CLEAR_DRAWED(a)  ((a &=~ GUI_DRAWED))
/* Includes ------------------------------------------------------------------*/
#define ORIGINAL_PARENT  (0)
/* gui level */
#define GUI_SAME_LEVEL   (0)
#define GUI_CHILD_LEVEL  (1)

#ifndef _VC_SIMULATOR_
/* RGB */
#define RGB(r,g,b) (( ( r & 0xF8 ) << 8 ) | ( ( g & 0xFC ) << 3 ) | ( ( b& 0xF8 ) >> 3 ))
#endif
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
	/* pri_data */
	void * pri_data;
	/* size and pos */
	unsigned short x;
	unsigned short y;
	unsigned short x_size;
	unsigned short y_size;
	/* widget flags */
	unsigned short wflags;
	/* memds */
	unsigned short upd;
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
	/* draw methed */
	void (*draw)(struct widget *);
	/*  Message response */
	void (*msg_response)(struct widget *,gui_msg_type msg);
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
	/* draw methed */
	void (*draw)(window_def *);
	/*  Message response */
	void (*msg_response)(window_def *,gui_msg_type msg);
	/* window off */
}window_def;
/* end if file */

/* Function declaration */
int gui_static_creater(void);
int gui_win_creater(window_def * win);
int gui_widget_creater( widget_def * widget );
int gui_find_connect(window_def * ori,unsigned short x,unsigned short y);

/* end of files */
#endif













