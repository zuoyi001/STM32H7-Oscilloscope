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
/* text */
widget_def voltage_ch[2];
/* time */
widget_def time_ch[2];
/* ch1 measure */
widget_def measure_ch1[6];
widget_def measure_ch2[6];
/* table */
char * mert[6] = {"频率","1.00K","峰峰值","200mV","最大值","3.38V"};
char * mert1[6] = {"频率","1.00K","正脉宽","200mV","平均值","3.38V"};
/* gui dev */
static gui_dev_def * dev;
/* osc create ui */
int osc_create_ui(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* calculate main size */
  osc_calculate_main_size(dev,&win_main,0);
	/* create the group */
	osc_calculate_sg_size(dev,win_group,sizeof(win_group) / sizeof(win_group[0]));	
	/* create the menu win ui */
  osc_calculate_menu_size(dev,&win_menu,0);
	/* btn */
  osc_calculate_btn_size(dev,&win_menu,btn,sizeof(btn) / sizeof(btn[0]));
	/* voltage */
	osc_calculate_volage_string(&win_group[1],voltage_ch,sizeof(voltage_ch) / sizeof(voltage_ch[0]) , "100mV","200mV");
	/* time */
	osc_calculate_time_string(&win_group[3],time_ch,2,"M","500ms");
	/* measure */
	osc_calculate_measure_ch(&win_group[0],measure_ch1,6,mert,1);
	osc_calculate_measure_ch(&win_group[0],measure_ch2,6,mert1,2);	
	/* return */
	return FS_OK;
}


























