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
/* Includes ------------------------------------------------------------------*/
#include "fos.h"
#include "osc_ui.h"
#include "osc_menu.h"
#include "osc_api.h"
/* Private includes ----------------------------------------------------------*/
FOS_TSK_REGISTER(osc_menu_thread,PRIORITY_2,10);/* run as 10ms */
FOS_INODE_REGISTER("osc_menu",osc_menu_heep,osc_menu_init,0,15);
/* static key sta */
static unsigned char com2_sta[8];
static unsigned int cd_cnt = 0;
/* function */
static void (*com_callbacks[8])(void);
/* heap init */
static int osc_menu_heep(void)
{
	/* fill the data */
	com_callbacks[2] = key_menu_callback;;
	com_callbacks[4] = key_auto_callback;
	com_callbacks[5] = key_single_callback;
	com_callbacks[6] = key_measure_callback;
	com_callbacks[7] = key_runstop_callback;
	/* return */
	return FS_OK;
}
/* config */
static int osc_menu_init(void)
{
	return FS_OK;
}
/* thread */
static void osc_menu_thread(void)
{
	/* set addr */
	osc_set_key_addr(cd_cnt);
	/* read data */
	com2_sta[cd_cnt] = osc_read_com2();
	/* next and limit */
	cd_cnt ++ ;
	/* set limit */
	if( cd_cnt >= 8 )
	{
		/* check event */
		check_COM2_event(com2_sta,8);
		/* clear */
		cd_cnt = 0;
	}
}
/* check key envent */
static void check_COM2_event(unsigned char * sta_buf,unsigned int len)
{
	/* key flags */
	static unsigned char key_flags[8];
	/* flags */
	for( int i = 0 ; i < len ; i ++ )
	{
		/* key check */
		if( sta_buf[i] == 1 )
		{
			/* check cde */
			if( key_flags[i] == 1 )
			{
				/* clear */
				key_flags[i] = 0;
				/* send key event */
				if( com_callbacks[i] != 0 )
				{
					/* run callback functions */
					com_callbacks[i]();
				}
				/* -------------- */
			}
		}
		else
		{
			/* match low level */
			key_flags[i] = 1;
			/* match low level */
		}
  }
}
/* runstop */
static void key_runstop_callback(void)
{
	
}
/* runstop */
static void key_auto_callback(void)
{
	
}
/* runstop */
static void key_measure_callback(void)
{
	
}
/* runstop */
static void key_single_callback(void)
{
	
}
/* key callback */
static void key_menu_callback(void)
{
	int menu_sta = osc_ui_menu_sta();
	/* trig arrow sta */
	static int trig_ch1_sta;
	static int trig_ch2_sta;
	/* show or hide */
	if( menu_sta )
	{
		/* get trig arrow sta */
		trig_ch1_sta = osc_ui_trig_arrow_sta(0);
		trig_ch2_sta = osc_ui_trig_arrow_sta(1);
		/* show window */
		osc_ui_menu_show(1);
		/* hide trig arrows  */
		osc_ui_trig_arrow_show(0,0);
		osc_ui_trig_arrow_show(1,0);
		/* end of */
	}
	else
	{
		/* hide win */
		osc_ui_menu_show(0);
		/* show trig windows */
		osc_ui_trig_arrow_show(0,trig_ch1_sta);
		osc_ui_trig_arrow_show(1,trig_ch2_sta);
		/* end of */		
	}
}






























