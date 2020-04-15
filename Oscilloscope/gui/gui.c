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
#include "gui.h"
#include "gui_dev.h"
#include "string.h"
/* Includes ------------------------------------------------------------------*/
static window_def * original_win = 0;
/* int gui draw */
int gui_static_creater(void)
{
	/* from zero to end creating the pic */
	for( window_def * base = original_win ; base != 0 ; base = base->win_child )
	{
		/* if use the default draw function */
		if((!CHECH_DRAWED(base->msg.wflags)) && ( base->draw != 0 ) && (!CHECK_HIDE(base->msg.wflags)))
		{
			base->draw(base);
			/* set flag */
			SET_DRAWED(base->msg.wflags);
			/* def */
		}
		else
		{
			/* can not supply now*/
			continue;//skip this win
		}
		/* create widget */
		for( widget_def * wbase = base->wchild ; wbase != 0 ; wbase = wbase->peer_linker)
		{
			/* if use the default draw function */
			if((!CHECH_DRAWED(wbase->msg.wflags)) && ( wbase->draw != 0 ) && (!CHECK_HIDE(wbase->msg.wflags)))
			{
				wbase->draw(wbase);
				/* set flag */
				//SET_DRAWED(wbase->msg.wflags);
				/* def */
			}
			else
			{
				/* can not supply now*/
			}
		}
	}
	/* return */
	return FS_OK;
}
/* create and init a win */
int gui_win_creater(window_def * win)
{
	/* juddge wherere it is the ORIGINAL_PARENT */
	if( original_win == ORIGINAL_PARENT )
	{
		/* return error . can not find the original win */
		original_win = win;
		/* ok */
		return FS_OK;
	}
	/* link with parent */
	window_def * base;
	/* same mode or */
	/* set */
	for( base = original_win ; base->win_child != 0 ; base = base->win_child )
	{
		/* find the pos . nothing to do*/
	}		
	/* set */
	base->win_child = win;
	/* return OK */
	return FS_OK;
}
/* create widget */
int gui_widget_creater(widget_def * widget )
{
	/* juddge wherere it is the ORIGINAL_PARENT */
	if( widget->parent == ORIGINAL_PARENT )
	{
		/* return error */
		return FS_ERR;
	}
	/* set the original linker */
	if( widget->parent->wchild == ORIGINAL_PARENT )
	{
		/* set */
		widget_def ** base = &widget->parent->wchild;
		/* set base */
		*base = widget;
		/* return OK */
		return FS_OK;
	}
	/* get the linker tail */
	widget_def * wbase = widget->parent->wchild;
	/* set up widget */
	for(  ; wbase->peer_linker != 0 ; wbase = wbase->peer_linker )
	{
		/* find the pos . nothing to do*/
	}
	/* set linker */
	wbase->peer_linker = widget;
	/* return OK */
	return FS_OK;
}
/* clear all drawed */
int gui_clear_all(void)
{
	/* from zero to end creating the pic */
	for( window_def * base = original_win ; base != 0 ; base = base->win_child )
	{
		/* clear */
		CLEAR_DRAWED(base->msg.wflags);
		/* create widget */
		for( widget_def * wbase = base->wchild ; wbase != 0 ; wbase = wbase->peer_linker)
		{
			/* set flag */
			CLEAR_DRAWED(wbase->msg.wflags);
			/* def */
		}
	}
	/* return */
	return FS_OK;
}
/* find the connect */
int gui_find_connect(window_def * ori,unsigned short x,unsigned short y)
{
	/* from zero to end creating the pic */
	for( window_def * base = ori->win_child ; base != 0 ; base = base->win_child )
	{
		if( base->msg.wflags & GUI_HIDE )
		{
			continue;
		}
		if( x >= base->msg.x && x <= (base->msg.x + base->msg.x_size) )
		{
			if( y >= base->msg.y && y <= (base->msg.y + base->msg.y_size) )
			{
				return 1;
			}
		}
	}
	/* return */
	return 0;
}
/* gui hide window */
int gui_hide_win(window_def * win)
{
	/* get hide flag */
	if( CHECK_HIDE(win->msg.wflags) )
	{
		return FS_OK;//already hide
	}
	/* got xy */
	int gotxy = 0;
	/* from zero to end creating the pic */
	for( window_def * base = original_win ; base != 0 ; base = base->win_child )
	{
		/* skip the base */
		if( win == base )
		{
			continue;//skip this win
		}
		/* get the cross data */
		if((( win->msg.x <= base->msg.x ) && ( win->msg.x + win->msg.x_size ) >= base->msg.x ) || 
			 (( win->msg.x >= base->msg.x ) && ( win->msg.x <= ( base->msg.x + base->msg.x_size ))))
		{
		  /* got x */
			gotxy = 1;
		}
		/* get the cross data y */
		if((( win->msg.y <= base->msg.y ) && ( win->msg.y + win->msg.y_size ) >= base->msg.y ) || 
			 (( win->msg.y >= base->msg.y ) && ( win->msg.y <= ( base->msg.y + base->msg.y_size ))))
		{
		  /* got x */
			if( gotxy == 1 )
			{
			  /* got y */
				base->msg.mark_flag = 1;
				/* set other */
				base->msg.mx = win->msg.x;
				base->msg.my = win->msg.y;
				base->msg.mxstop = win->msg.x_size + win->msg.x;
				base->msg.mystop = win->msg.y_size + win->msg.y;
				/* clear the redraw flag */
				CLEAR_DRAWED(base->msg.wflags);
				/* set hide */
				SET_HIDE(win->msg.wflags);
				/* endo if */
			}
		}		
	}
	/* return ERROR */
	return FS_OK;
}
/* gui show a win*/
/* gui hide window */
int gui_show_win(window_def * win)
{
	/* get hide flag */
	if( !CHECK_HIDE(win->msg.wflags) )
	{
		return FS_OK;//already show
	}	
	/* clear */
	CLEAR_HIDE(win->msg.wflags);
	/* set loce */
	CLEAR_DRAWED(win->msg.wflags);
	/* return ERROR */
	return FS_OK;	
}

































































