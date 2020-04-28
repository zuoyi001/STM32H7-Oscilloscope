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

/* Private includes ----------------------------------------------------------*/

#ifndef __DISPLAY_DEV_H__
#define __DISPLAY_DEV_H__

/* define L8 mode or RGB mode */
#define LCD_MODE_L8    (1)
/* lcd and vga dev struct */
typedef struct
{
	/* Characters to display when setting */
	char * dev_capital;	
	/* 
		The default parameters may need to be set according to different screens. 
		Please refer to the data manual for details 
	*/
	unsigned short pwidth;
	unsigned short pheight;
	unsigned short hsw;
	unsigned short vsw;
	unsigned short hbp;
	unsigned short vbp;
	unsigned short hfp;
	unsigned short vfp;
	/* rcc clock settings */
	unsigned short PLLSAIN;
	unsigned short PLLSAIR;
	unsigned int PLLSAIDIVR;
	/* end */
}display_dev_def;
/* display info */
typedef struct 
{
	/* display dev */
	display_dev_def * display_dev;
	/* display gram addr */
	unsigned int gram_addr;
}display_info_def;
/* RCC_PLLSAIDIVR */
#define DIVR_2                0x00000000U
#define DIVR_4                0x00010000U
#define DIVR_8                0x00020000U
#define DIVR_16               0x00030000U

/* function delares */
display_info_def * get_display_dev_info(void);
static void LTDC_Init(display_dev_def * info);
static int dev_init(void);
/* end */
#endif
/* end of files */





















