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

#ifndef __OSC_MENU_H__
#define __OSC_MENU_H__
/* Includes ------------------------------------------------------------------*/
/* function */
static void osc_menu_thread(void);
static int osc_menu_heep(void);
static int osc_menu_init(void);
static void key_runstop_callback(void);
static void key_auto_callback(void);
static void key_measure_callback(void);
static void key_single_callback(void);
static void key_menu_callback(void);
static void check_COM2_event(unsigned char * sta_buf,unsigned int len);
static void menu_update(void);

#endif
























