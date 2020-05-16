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
#include "string.h"
/* Private includes ----------------------------------------------------------*/
FOS_TSK_REGISTER(osc_menu_thread,PRIORITY_2,10);/* run as 10ms */
FOS_INODE_REGISTER("osc_menu",osc_menu_heep,osc_menu_init,0,15);
/* create cfg task gui detecter task run as 100 ms */
FOS_TSK_REGISTER(osc_menu_hide_thread,PRIORITY_4,1000);
/* const char menu */
const char * menu_default_table[6] = {"系统菜单","通道选择\n CH2","耦合方式\n AC","触发方式\nAuto","触发边沿\n 上升 ","关闭通道",};
/* chn select */
const char * menu_chn[2] = {"通道选择\n CH1","通道选择\n CH2"};
const char * menu_coupling[2] = {"耦合方式\n DC","耦合方式\n AC"};
const char * menu_open_chn[2] = {"关闭通道","开启通道"};
const char * fast_tips_ch1[2] = {"CH1:DC","CH1:AC"};
const char * fast_tips_ch2[2] = {"CH2:DC","CH2:AC"};
const char * trig_type_chn[4] = {"触发方式\nAuto  ","触发方式\nNormal","触发方式\nSmart ","触发方式\nsingle"};
const char * trig_fast[4] = {"Auto  ","Normal","Smart ","single"};
const char * trig_edge[2] = {"触发边沿\n 上升 ","触发边沿\n 下降 "};
const char * trig_source[2] = {"TRIG:CH1","TRIG:CH2"};
/* static link */
static osc_run_msg_def osc_run_msg __attribute__((at(0x24000000 + 800 * 600 + 64)));;
/* static key sta */
static unsigned char com2_sta[8];
static unsigned int cd_cnt = 0;
static int trig_ch1_sta;
static int trig_ch2_sta;
/* menu show or hide */
static unsigned char menu_hold_time_s = 0;
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
	/* init */
	memset(&osc_run_msg,0,sizeof(osc_run_msg));
	/* return */
	return FS_OK;
}
/* config */
static int osc_menu_init(void)
{
	/* set fast tips ch1 */
	osc_ui_set_chn_text(0,osc_run_msg.coupling_type[0] ? fast_tips_ch1[1] : fast_tips_ch1[0]);
	/* ch2 */
	osc_ui_set_chn_text(1,osc_run_msg.coupling_type[1] ? fast_tips_ch2[1] : fast_tips_ch2[0]);
	/* init */
	osc_ui_set_csh_show(0,!osc_run_msg.chn_enable[0]);
	osc_ui_set_csh_show(1,!osc_run_msg.chn_enable[1]);
	/* set fast tips */
	osc_ui_set_trig_text(trig_fast[osc_run_msg.trig_mode % 4]);
	/* set trig souce */
	osc_ui_set_trig_src(trig_source[osc_run_msg.chn_focus % 2]);
	/* return */
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
/* menu show */
static void osc_menu_hide_thread(void)
{
	/* check menu sta */
	if( osc_ui_menu_sta() == 0)
	{
		/* showen */
		if( menu_hold_time_s == 0 )
		{
			menu_hide_auto();
		}
		else
		{
			menu_hold_time_s --;
		}
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
					/* set hold time */
					menu_hold_time_s = 5;
					/* -------------- */
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
			osc_run_msg.chn_focus ^= 1;
			/* set chn  */
			osc_ui_set_one_menu_text(1,osc_run_msg.chn_focus ? menu_chn[1] : menu_chn[0]);
			/* set coupling */
			unsigned char foc = osc_run_msg.chn_focus ? 1 : 0;
			/* set */
			osc_ui_set_one_menu_text(2,(osc_run_msg.coupling_type[foc]) ? menu_coupling[1] : menu_coupling[0]);
			osc_ui_set_one_menu_text(5,(osc_run_msg.chn_enable[foc]) ? menu_open_chn[1] : menu_open_chn[0]);
			/* set trig source */
			osc_ui_set_trig_src(trig_source[foc]);
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
			if( osc_run_msg.chn_focus == 0 )
			{
				/* check mode */
				osc_run_msg.coupling_type[0] ^= 1;
				/* show text */
				osc_ui_set_one_menu_text(2,(osc_run_msg.coupling_type[0]) ? menu_coupling[1] : menu_coupling[0]);
				osc_ui_set_chn_text(0,(osc_run_msg.coupling_type[0]) ? fast_tips_ch1[1] : fast_tips_ch1[0] );
				/* set couple mode ch1 */
				osc_coupling_setting(0,osc_run_msg.coupling_type[0]);
				/*-----------------*/
			}
			else
			{
				/* check mode ch2 */
				osc_run_msg.coupling_type[1] ^= 1;
				/* show text */
				osc_ui_set_one_menu_text(2,(osc_run_msg.coupling_type[1]) ? menu_coupling[1] : menu_coupling[0]);
				osc_ui_set_chn_text(1,(osc_run_msg.coupling_type[1]) ? fast_tips_ch2[1] : fast_tips_ch2[0] );
				/* set couple mode ch2 */
				osc_coupling_setting(1,osc_run_msg.coupling_type[1]);
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
	int menu_sta = osc_ui_menu_sta();
	/* show or hide */
	if( !menu_sta )
	{
		/* mode */
		if( 1 ) // chn menu
		{
			/* nest link */
			osc_run_msg.trig_mode ++;
			/* limit */
			if( osc_run_msg.trig_mode >= 4 )
			{
				osc_run_msg.trig_mode = 0;
			}
			/* set text */
			osc_ui_set_one_menu_text(3,trig_type_chn[osc_run_msg.trig_mode]);
			/* set fast tips */
			osc_ui_set_trig_text(trig_fast[osc_run_msg.trig_mode]);
		}
	}
	else
	{
		/* open measure menu */
	}		
}
/* runstop */
static void key_single_callback(void)
{
	int menu_sta = osc_ui_menu_sta();
	/* show or hide */
	if( !menu_sta )
	{
		/* mode */
		if( 1 ) // chn menu
		{
			/* nest link */
			osc_run_msg.trig_type ++;
			/* limit */
			if( osc_run_msg.trig_type >= 2 )
			{
				osc_run_msg.trig_type = 0;
			}
			/* set text */
			osc_ui_set_one_menu_text(4,trig_edge[osc_run_msg.trig_type]);
		}
	}
	else
	{
		/* single */
	}		
}
/* static hide menu */
static void menu_hide_auto(void)
{
	/* hide win */
	osc_ui_menu_show(0);
	/* show trig windows */
	osc_ui_trig_arrow_show(0,trig_ch1_sta);
	osc_ui_trig_arrow_show(1,trig_ch2_sta);
	/* end of */	
}
/* key callback */
static void key_menu_callback(void)
{
	int menu_sta = osc_ui_menu_sta();
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
		/* mode */
		if( 1 ) // chn menu
		{
			/* chn 12 */
			/* check open */
			osc_run_msg.chn_enable[osc_run_msg.chn_focus] ^= 1;
			/* change data */
			osc_ui_set_one_menu_text(5,(osc_run_msg.chn_enable[osc_run_msg.chn_focus]) ? menu_open_chn[1] : menu_open_chn[0]);
			/* show or hide fast tips */
			osc_ui_set_csh_show(osc_run_msg.chn_focus,!osc_run_msg.chn_enable[osc_run_msg.chn_focus]);
			/*------------------------*/
		}		
	}
}
/* menu_update */
static void menu_update(void)
{
	/* menu 1 */
	menu_default_table[1] = osc_run_msg.chn_focus ? menu_chn[1] : menu_chn[0];
	/* two chn two coupling */
	if( osc_run_msg.chn_focus == 0 )
	{
		menu_default_table[2] = (osc_run_msg.coupling_type[0]) ? menu_coupling[1] : menu_coupling[0];
		menu_default_table[5] = (osc_run_msg.chn_enable[0]) ? menu_open_chn[1] : menu_open_chn[0];
	}
	else
	{
		menu_default_table[2] = (osc_run_msg.coupling_type[1]) ? menu_coupling[1] : menu_coupling[0];
		menu_default_table[5] = (osc_run_msg.chn_enable[1]) ? menu_open_chn[1] : menu_open_chn[0];
	}
	/* set update */
	menu_default_table[3] = trig_type_chn[osc_run_msg.trig_mode % 4];
	/* set osc menu group */
	osc_ui_set_menu_text_group(menu_default_table,6);	
	/* end */
}





























