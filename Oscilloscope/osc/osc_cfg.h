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

#ifndef __OSC_CFG_H__
#define __OSC_CFG_H__
/* Includes ------------------------------------------------------------------*/
#define OSC_UNIT_US   (0)
#define OSC_UNIT_MS   (1)
#define OSC_UNIT_S    (2)
#define OSC_UINT_NS   (3)

#define OSC_BASE_CLOCK  (180)

/* Private includes ----------------------------------------------------------*/

/* osc_ time config */
typedef struct{
	/* show title */
  char * str;
	/* scan time */
  float osc_time;
	/* unit */
	unsigned int osc_unit;
	/* source inter or ex */
	unsigned int osc_clock_ex;
	/* Zoom Factor */
	unsigned int osc_zoom_factor;
	/* end if */
}osc_time_def;

/* set scan time */
const osc_time_def * osc_scan_time(unsigned int index);

#endif
























