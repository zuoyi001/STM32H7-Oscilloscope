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
/* const char menu */
const char * menu_default_table[6] = {"系统菜单","通道选择\n CH2","耦合方式\n AC","触发菜单","关闭通道","隐藏菜单"};
/* chn select */
static const char * menu_chn[2] = {"通道选择\n CH1","通道选择\n CH2"};
static const char * menu_coupling[2] = {"耦合方式\n DC","耦合方式\n AC"};
static const char * menu_open_chn[2] = {"关闭通道","开启通道"};
/* static link */
static unsigned char menu_link[6];
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
	int menu_sta = osc_ui_menu_sta();
	/* show or hide */
	if( !menu_sta )
	{
		/* mode */
		if( 1 ) // chn menu
		{
			/* set xor */
			menu_link[1] ^= 1;
			/* set chn  */
			osc_ui_set_one_menu_text(1,menu_link[1] ? menu_chn[1] : menu_chn[0]);
			/* set coupling */
			if( menu_link[1] == 0 )
			{
				osc_ui_set_one_menu_text(2,(menu_link[2]&0x0f) ? menu_coupling[1] : menu_coupling[0]);
			}
			else
			{
				osc_ui_set_one_menu_text(2,(menu_link[2]&0xf0) ? menu_coupling[1] : menu_coupling[0]);
			}
		}
	}
	else
	{
		/* stop or run */
	}
}
/* runstop */
static void key_auto_callback(void)
{
	int menu_sta = osc_ui_menu_sta();
	/* show or hide */
	if( !menu_sta )
	{
		/* mode */
		if( 1 ) // chn menu
		{
			/* chn 1 */
			if( menu_link[1] == 0 )
			{
				/* check mode */
				if( menu_link[2] & 0x0f )
				{
					menu_link[2] &=~ 0x0f;
				}
				else
				{
					menu_link[2] |= 0x0f;
				}
				/* show text */
				osc_ui_set_one_menu_text(2,(menu_link[2]&0x0f) ? menu_coupling[1] : menu_coupling[0]);
				/* set couple mode ch1 */
				
				/*-----------------*/
			}
			else
			{
	      /* check ch2 */
				if( menu_link[2] & 0xf0 )
				{
					menu_link[2] &=~ 0xf0;
				}
				else
				{
					menu_link[2] |= 0xf0;
				}
				/* show text */
				osc_ui_set_one_menu_text(2,(menu_link[2]&0xf0) ? menu_coupling[1] : menu_coupling[0]);
				/* set couple mode ch2 */
				
				/*-----------------*/				
			}
		}
	}
	else
	{
		/* auto mode */
	}	
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
		/* set osc menu group */
		menu_update();
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
/* menu_update */
static void menu_update(void)
{
	/* menu 1 */
	menu_default_table[1] = menu_link[1] ? menu_chn[1] : menu_chn[0];
	/* two chn two coupling */
	if( menu_link[1] == 0 )
	{
		menu_default_table[2] = (menu_link[2]&0x0f) ? menu_coupling[1] : menu_coupling[0];
	}
	else
	{
		menu_default_table[2] = (menu_link[2]&0xf0) ? menu_coupling[1] : menu_coupling[0];
	}
	/* open or not */
	menu_default_table[4] = menu_link[4] ? menu_open_chn[1] : menu_open_chn[0];
	/* set update */
	/* set osc menu group */
	osc_ui_set_menu_text_group(menu_default_table,6);	
	/* end */
}





























