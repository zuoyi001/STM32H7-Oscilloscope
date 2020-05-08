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
#include "gui.h"
#include "osc.h"
#include "gui_ascii.h"
#include "hz_out.h"
/* Includes ------------------------------------------------------------------*/
static void gui_char(gui_dev_def * dev,unsigned short x,unsigned short y,char num,unsigned char size,unsigned int color,unsigned int backcolor,unsigned char show_m)
{
	/* define some function */
	unsigned char temp;
	unsigned short y0 = y;
	/* calbrate the size */
	unsigned char csize = ( size / 8 + ( ( size % 8 ) ? 1 : 0 ) ) * ( size / 2);
	/* get the offset */
	num = num - ' ';
	/* write the char */
	for(int i = 0 ; i < csize ; i ++)
	{   
		/* default is the 2412 font */
		if( size == 16 )
		{
			temp = asc2_1608[num][i];
		}
		else
		{
			temp = asc2_2412[num][i];
		}
		/* draw point */
		for( int j = 0 ; j < 8  ; j ++ )
		{
			/* one by one */
			if( temp & 0x80 )
			{
				dev->set_noload_point( x, y, color);
			}
			else 
			{
				if( show_m )
				{
					dev->set_noload_point( x, y, backcolor);
				}
			}
			/* shift */
			temp <<= 1;
			/* next point */
			y++;
			/* get changed */
			if( ( y - y0 ) == size )
			{
				y = y0;
				x++;
				/* break */
				break;
			}
		}  	 
	}  	    	   	 	  
}
/* find the hz */
static unsigned char * find_hz(unsigned char * hzw)
{
	/* get the hz pos fast*/
	unsigned short * hsz = (unsigned short *)hzw;
	/* get pos */
	for( int i = 0 ; i < sizeof(HZ_INDEX) / sizeof(HZ_INDEX[0]) ; i ++ )
	{
		/* match the data */
		if( HZ_INDEX[i] == *hsz )
		{
			/* ok */
			return (unsigned char *)HZ_DATAX[i];
		}
	}
	/* can not find the hz */
	return (unsigned char *)HZ_DATAX[0];
}
/* show one hz */
static void draw_hz(gui_dev_def * dev , unsigned char * hzd,unsigned short x,unsigned short y,unsigned int color,unsigned int backcolor,unsigned char show_m)
{
	/* find the hz */
	unsigned char * hzc = find_hz(hzd);
	/* show a hz */
	int ti = 0;
	/* draw a point  */
  for( int i = 0 ; i < 32 ; i ++ )
	{
		for( int j = 0 ; j < 8 ; j ++ )
		{
			if( ( hzc[i] << j ) & 0x80 )
			{
				dev->set_noload_point( x + ti / 16 , y + ti % 16 , color);
			}
			else
			{
				if( show_m )
				{
					dev->set_noload_point( x + ti / 16 , y + ti % 16 , backcolor);
				}
			}
			/* inc */
			ti++;
		}
	}
}
/* get color */
unsigned short gui_color(unsigned short colo)
{
	/* chea */
	unsigned short color;
	/* front color */
	switch(colo)
	{
		case 0x0000:
			color = COLOR_CHAR;
		break;
		case 0x2000:
			color = COLOR_CH1;
			break;
		case 0x4000:
			color = COLOR_CH2;
			break;		
		case 0x6000:
			color = COLOR_TIPS_ERROR;
			break;			
		case 0x8000:
			color = COLOR_TIPS_WARNING;
			break;
		case 0xA000:
			color = COLOR_TIPS_NORMAL;
			break;		
		case 0xC000:
			color = COLOR_GRID_AREA_BG;
			break;			
		case 0xE000:
			color = COLOR_BACK_GROUND;
			break;
		default:
			color = COLOR_GRID_AREA_BG;
			break;
	}
	/* return */
	return color;
}
/* show a mix string */
void gui_dynamic_string(struct widget * wid)
{
	/* start pos */
	unsigned short posx = wid->parent->msg.x + wid->msg.x;
	unsigned short posy = wid->parent->msg.y + wid->msg.y;
	/* get char pointer */
	unsigned char * hzc = wid->msg.pri_data;
	unsigned short x = 0;
	/* get back and font color and size */
	unsigned short color;
	unsigned char size = (wid->msg.wflags & 0x1000) ? 24 : 16;
	/* set color */
	unsigned short color_w,backcolor,back_w;
	unsigned char show_m = 0;
	int ret = FS_ERR;
	int retb = FS_ERR;
	/* check */
	if(CHECK_REHIDE(wid->msg.wflags))
	{
		/* get inline */
		ret = gui_wid_inline(wid,&color_w);
		/* ok or not */
		if( ret != FS_OK )
		{
			color_w = (wid->parent->msg.wflags & 0x00E0) << 8;
		}
	}
	else
	{
		color_w = wid->msg.wflags;
		/* set backcolor */
		if( CHECK_OVERM(wid->msg.wflags))
		{
			/* get inline */
			retb = gui_wid_inline(wid,&back_w);
			/* ok or not */
			if( retb != FS_OK )
			{
				back_w = (wid->parent->msg.wflags & 0x00E0) << 8;
			}
			/* show back */
			show_m = 1;
		}
	}
	/* front color */
	color = ( ret == FS_OK ) ? color_w : gui_color(color_w & 0xE000);
	/* back color  */
	backcolor = ( retb == FS_OK ) ? back_w : gui_color(back_w & 0xE000);
	/* judge Is this a char or hz */
	while( *hzc != 0 )
	{
		/* enter */
		if( *hzc == '\n' )
		{
			x = 0;
			posy += 16;
			/* skip it */
			hzc++;
		}
		/* show a hz or char */
		if( (unsigned char)(*hzc) > 0x80 )
		{
			/* show a hz */
			draw_hz(wid->dev,(unsigned char *)hzc,posx + x , posy, color,backcolor,show_m);
			/* channge pos */
			x += 16;
			hzc += 2;
		}
		else
		{
			/* show a char */
			gui_char(wid->dev,posx + x , posy,*hzc,size,color,backcolor,show_m);
		  /* channge pos */
			if( size == 16 )
			{
			  x += 8;
			}
			else
			{
				x += 12;
			}
			/* out */
			hzc += 1;
		}
	}
	/* clear */
	CLEAR_OVERM(wid->msg.wflags);
}






























