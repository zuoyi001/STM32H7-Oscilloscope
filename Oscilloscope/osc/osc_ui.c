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
#include "fos.h"
#include "osc.h"
#include "osc_ui.h"
#include "gui.h"
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("osc_ui",osc_create_ui,0,0,1);
/* define win */
window_def win_main;
/* gui dev */
static gui_dev_def * dev;
/* osc create ui */
static int osc_create_ui(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* create the win */
	win_main.msg.x = 0;
	win_main.msg.y = 0;
	win_main.msg.x_end = dev->width;
	win_main.msg.y_end = dev->height;
	/* set callback */
	win_main.msg.draw = osc_main_draw;
	/*--------------*/
	gui_win_creater(&win_main);
	/* return */
	return FS_OK;
}
/* draw function */
static void osc_main_draw(struct gui_info *info)
{
	/* ignore the complier warnning */
	(void)info;
	/* create lisn */
	create_grid_data(dev);
	/* end if data */
}




































