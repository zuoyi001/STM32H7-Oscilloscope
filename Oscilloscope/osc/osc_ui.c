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
/* menu text */
widget_def menu_text[6];
/* tips */
widget_def tips_text;
/* fast tips */
widget_def fast_tips[5];
/* widget arrow */
widget_def base_vol_arrow[2];
widget_def trig_vol_arrow[2];
/* trig lines */
widget_def trig_lines[2];
/* table */
char * mert[6] = {"频率","1.0M","峰峰值","200mV","最大值","3.38V"};
char * mert1[6] = {"频率","1.0K","正脉宽","180mV","平均值","1.68V"};
char * menu_table[6] = {"系统菜单","通道选择\n CH1","耦合方式\n DC","触发方式\n 自动","触发边沿\n上升","关闭通道"};
char * mert43[5] = {"CH1:DC","CH2:AC","Auto","TRIG:CH1","TIME:0s"};
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
  osc_calculate_menu_size(dev,&win_menu,GUI_HIDE);
	/* btn */
  osc_calculate_btn_size(dev,&win_menu,btn,sizeof(btn) / sizeof(btn[0]));
	/* voltage */
	osc_calculate_volage_string(&win_group[1],voltage_ch,sizeof(voltage_ch) / sizeof(voltage_ch[0]) , "100mV","200mV");
	/* time */
	osc_calculate_time_string(&win_group[3],time_ch,2,"M","500ms");
	/* measure */
	osc_calculate_measure_ch(&win_group[0],measure_ch1,6,mert,1);
	osc_calculate_measure_ch(&win_group[0],measure_ch2,6,mert1,2);	
	/* menu */
	osc_calculate_menu(&win_menu,menu_text,6,menu_table);
	/* tips */
	osc_calculate_tips(&win_group[2],&tips_text,TIPS_NORMAL,"扫描时间已到达最大值");
	/* arrow */
	osc_calculate_base_arrow(&win_main,&base_vol_arrow[0],1);
	osc_calculate_base_arrow(&win_main,&base_vol_arrow[1],2);
	/* fast title */
	osc_calculate_title_string(&win_main,fast_tips,5,mert43);
	/* */
	osc_calculate_trig_arrow(&win_main,&trig_vol_arrow[0],1);
	osc_calculate_trig_arrow(&win_main,&trig_vol_arrow[1],2);
	/* lines */
	osc_calculate_trig_line(&win_main,&trig_lines[0],1);
	osc_calculate_trig_line(&win_main,&trig_lines[1],2);
	/* return */
	return FS_OK;
}
/* set time text */
void osc_ui_time_str(char * str)
{
	gui_set_wid_text(&time_ch[1],str);
}
/* set time text */
void osc_ui_tips_str(char * str)
{
	gui_set_wid_text(&tips_text,str);
}
/* move arrow */
void osc_ui_move_offset_arrow(unsigned char chn,unsigned short pos)
{
	/* limit */
	if( chn < 2 )
	{
		gui_move_wid(&base_vol_arrow[chn] ,base_vol_arrow[chn].msg.x , pos);
	}
}
/* move trig arrow */
void osc_ui_move_trig_arrow(unsigned char chn,unsigned short pos)
{
		/* limit */
	if( chn < 2 )
	{
		gui_move_wid(&trig_vol_arrow[chn] ,trig_vol_arrow[chn].msg.x , pos);
	}
}
/* move trig arrow */
void osc_ui_trig_arrow_show(unsigned char chn,unsigned char mode)
{
	/* limit */
	if( chn < 2 )
	{
		/* show or hide */
		if( mode )
		{
			gui_show_widget(&trig_vol_arrow[chn]);
		}
		else
		{
			gui_hide_widget(&trig_vol_arrow[chn]);
		}
	}
}
/* check trig arrow hide */
int osc_ui_trig_arrow_sta(unsigned char chn)
{
	/* limit */
	if( chn < 2 )
	{
		return CHECK_HIDE(trig_vol_arrow[chn].msg.wflags) ? 0 : 1;
	}	
	/* return error */
	return FS_ERR;
}
/* change */
void osc_ui_vol_scale(unsigned char chn , char * str )
{
	/* limit */
	if( chn < 2 )
	{	
	  gui_set_wid_text(&voltage_ch[chn],str);
	}
}
/* osc move trig lines */
void osc_ui_move_trig_lines(unsigned char chn,unsigned short posy)
{
	/* limit */
	if( chn < 2 )	
	{
		gui_move_wid(&trig_lines[chn],0,posy);
	}
}
/* void osc show hide trig lies */
void osc_ui_trig_lines_show(unsigned char chn,unsigned char mode)
{
	/* limit */
	if( chn < 2 )	
	{
		/* mode */
		if( mode == 0 )
		{
			gui_hide_widget(&trig_lines[chn]);
		}
		else
		{
			gui_show_widget(&trig_lines[chn]);
		}
	}	
}
/* void osc ui measure title */
void osc_ui_measure_capital(unsigned char chn,unsigned char item,const char * capital)
{
	/* limit */
	if( item > 2 )
	{
		/* cannot supply now */
		return;
	}		
	/* limit */
	if( chn == 0 )	
	{
		gui_set_wid_text(&measure_ch1[item*2],(char *)capital);
	}
  else
	{
		gui_set_wid_text(&measure_ch2[item*2],(char *)capital);
	}		
	/* end of func */
}
/* void osc ui measure data */
void osc_ui_measure_data(unsigned char chn,unsigned char item,char * capital)
{
	/* limit */
	if( item > 2 )
	{
		/* cannot supply now */
		return;
	}		
	/* limit */
	if( chn == 0 )	
	{
		gui_set_wid_text(&measure_ch1[item*2 + 1],capital);
	}
  else
	{
		gui_set_wid_text(&measure_ch2[item*2 + 1],capital);
	}		
	/* end of func */
}
/* int osc menu sta .show or hide */
int osc_ui_menu_sta(void)
{
	/* sta win_menu */
	return CHECK_HIDE(win_menu.msg.wflags) ? 1 : 0;
}
/* set menu sta */
void osc_ui_menu_show(unsigned char mode)
{
	/* show or hide */
	if( mode )
	{
		gui_show_win(&win_menu);
	}
	else
	{
		gui_hide_win(&win_menu);
	}
}
/* void osc ui set menu text */
void osc_ui_set_menu_text_group(const char ** text,unsigned int len)
{
	/* check len */
	if( len != sizeof(menu_table) / sizeof(menu_table[0]) )
	{
		return;/* cannot supply this format */
	}
	/* set */
	for( int i = 0 ; i < len ; i ++ )
	{
		gui_set_wid_text(&menu_text[i],(char *)text[i]);
	}
}
/* void osc set one text */
void osc_ui_set_one_menu_text(unsigned char item,const char * text)
{
	/* check len */
	if( item >= sizeof(menu_table) / sizeof(menu_table[0]) )
	{
		return;/* cannot supply this format */
	}	
	/* set */
	gui_set_wid_text(&menu_text[item],(char *)text);
	/* end of func */
}
/* void osc ui get fast tips text */
void osc_ui_set_chn_text(unsigned char chn,const char * text)
{
	/* select chn */
	if( chn == 0 )
	{
		gui_set_wid_text(&fast_tips[0],( char * )text);
	}
	else
	{
		gui_set_wid_text(&fast_tips[1],( char * )text);
	}
}
/* set osc ui fast coupling show or hide */
void osc_ui_set_csh_show(unsigned char chn,unsigned char mode)
{
	if( chn >= 2 )
	{
		return;
	}
	/* show or hide */
	if( mode == 0 )
	{
		gui_hide_widget(&fast_tips[chn]);
	}
	else
	{
		gui_show_widget(&fast_tips[chn]);
	}
}
/* set trig text */
/* void osc ui get fast tips text */
void osc_ui_set_trig_text(const char * text)
{
	/* select chn */
	gui_set_wid_text(&fast_tips[2],( char * )text);
}
/* set trig trig source text */
void osc_ui_set_trig_src(const char * text)
{
	/* select chn */
	gui_set_wid_text(&fast_tips[3],( char * )text);
}


























































