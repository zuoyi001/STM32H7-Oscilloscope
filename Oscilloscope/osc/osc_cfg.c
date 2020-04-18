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
#include "hal_tim.h"
#include "osc_cfg.h"
#include "osc.h"
/* Private includes ----------------------------------------------------------*/
const osc_time_def osc_tim[] = 
{
	{
		.str = "50ns ",
		.osc_time = 50, /* 50 ns */
		.osc_clock_ex = 15,
		.osc_unit = OSC_UINT_NS,
	},
	{
		.str = "250ns",
		.osc_time = 250, /* 250 ns */
		.osc_clock_ex = 3,
		.osc_unit = OSC_UINT_NS,
	},
	{
		.str = "750ns",
		.osc_time = 750 , /* 750 ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
	},
	{
		.str = "2.5us",
		.osc_time = 2.5f , /* 2.5us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},
	{
		.str = "5us  ",
		.osc_time = 5 , /* 5us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},	
	{
		.str = "10us ",
		.osc_time = 10 , /* 10us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},
	{
		.str = "25us ",
		.osc_time = 25 , /* 10us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},
	{
		.str = "50us ",
		.osc_time = 50 , /* 50us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},
	{
		.str = "100us",
		.osc_time = 100 , /* 100us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},	
	{
		.str = "250us",
		.osc_time = 250 , /* 250us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},
	{
		.str = "500us",
		.osc_time = 500 , /* 500us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
	},	
	{
		.str = "1ms  ",
		.osc_time = 1 , /* 1ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},
	{
		.str = "2.5ms",
		.osc_time = 2.5 , /* 2.5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},	
	{
		.str = "5ms  ",
		.osc_time = 5 , /* 5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},	
	{
		.str = "10ms ",
		.osc_time = 10 , /* 10ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},	
	{
		.str = "25ms ",
		.osc_time = 25 , /* 25ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},	
	{
		.str = "50ms ",
		.osc_time = 50 , /* 50ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},	
	{
		.str = "100ms",
		.osc_time = 100 , /* 100ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},	
	{
		.str = "250ms",
		.osc_time = 250 , /* 250ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},
	{
		.str = "500ms",
		.osc_time = 500 , /* 500ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
	},
};
/* set scan clock */
const osc_time_def * osc_scan_time(unsigned int index)
{
	/* over */
	if( index >= sizeof(osc_tim) / sizeof(osc_tim[0]) )
	{
		/* return */
		return 0;
	}
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* psc */
	unsigned short psc = 0;
	/* get the */
	if( osc_tim[index].osc_unit == OSC_UNIT_US )
	{
		/* calbrate the psc num */
		psc = (unsigned short)((float)OSC_BASE_CLOCK * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UNIT_MS )
	{
		psc = (unsigned short)((float)OSC_BASE_CLOCK * 1000 * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UINT_NS )
	{
		/* return */
    return &osc_tim[index];		
	}
	else
	{
		/* unit = s , can not supply now */
		return 0;
	}
	/* set psc */
	hal_tim_psc(psc);
	/* return OK */
	return &osc_tim[index];
}







































































