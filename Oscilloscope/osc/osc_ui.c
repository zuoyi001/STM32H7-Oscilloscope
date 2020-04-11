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
#include "fos.h"
#include "osc.h"
#include "osc_ui.h"
#include "gui.h"
#include "string.h"
#include "math.h"
#include "display_dev.h"
/* Private includes ----------------------------------------------------------*/
FOS_INODE_REGISTER("osc_ui",osc_create_ui,0,0,1);
/* define win */
window_def win_main;
window_def win_group[4];
/* win menu */
window_def win_menu;
/* btn */
widget_def btn[5];
/* gui dev */
static gui_dev_def * dev;
/* osc create ui */
int osc_create_ui(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* calculate main size */
  osc_calculate_main_size(dev,&win_main,create_grid_data,0);
	/* create the group */
	osc_calculate_sg_size(dev,win_group,sizeof(win_group) / sizeof(win_group[0]),draw_group_win);	
	/* create the menu win ui */
  osc_calculate_menu_size(dev,&win_menu,draw_menu_win,0);
	/* btn */
  osc_calculate_btn_size(dev,&win_menu,btn,sizeof(btn) / sizeof(btn[0]));
	/* return */
	return FS_OK;
}


























