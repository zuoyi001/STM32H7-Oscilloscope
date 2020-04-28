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
#include "display_dev.h"
#include "main.h"
#include "math.h"
#include "middle.h"
/* Private includes ----------------------------------------------------------*/
#if LCD_MODE_L8
static unsigned char * gram_buffer = NULL;
#else
static unsigned short * gram_buffer = NULL;
#endif
static unsigned short WIDTH = 0 , HEIGHT = 0;
/* Define all supported display panel information */

/* muddle init that supply all pixel function */
int middle_layer_init(display_info_def * dev_info)
{
	/* get gram */
	if( dev_info == NULL )
	{
		/* get information error */
		return (-1);
	}
	/* get information OK */
#if LCD_MODE_L8	
	gram_buffer = (unsigned char *)dev_info->gram_addr;
#else
	gram_buffer = (unsigned short *)dev_info->gram_addr;
#endif
	/* get display size */
	WIDTH =  dev_info->display_dev->pwidth;
	HEIGHT = dev_info->display_dev->pheight;
	/* return OK */
	return 0;
}
/* middle functions set point */
void set_point( unsigned short * mark , unsigned short x , unsigned short y , unsigned int color )
{
	/* get GRAM */
	if( mark[0] == 0 )
	{
		gram_buffer[ y * WIDTH + x ] = color;
	}
	else
	{
		/* check data */
		if( ( x >= mark[1] && x <= mark[3] ) && 
			  ( y >= mark[2] && y <= mark[4] ))
		{
			gram_buffer[ y * WIDTH + x ] = color;
		}
	}
}
/* set noload point */
void set_noload_point( unsigned short x , unsigned short y , unsigned int color )
{
	if( gram_buffer != NULL )
	{
		/* get GRAM */
		gram_buffer[ y * WIDTH + x ] = color;
	}
}
/* middle get point */
unsigned short get_point( unsigned short x , unsigned short y )
{
	/* get GRAM */
	if( gram_buffer != NULL )
	{
		return gram_buffer[ y * WIDTH + x ]; 
	}
	/* error */
	return 0;
}
/* middle fill a */
void fill_rect(unsigned short psx,unsigned short psy,unsigned short pex,unsigned short pey,unsigned int color)
{ 
	/* define some parm */
	unsigned int timeout = 0; 
	unsigned short offline;
	unsigned int addr; 
	/* calbrate the offline */
	offline = WIDTH - ( pex - psx + 1 ); 
	/* addr */
	addr = ( ( unsigned int )gram_buffer + 2 * ( WIDTH * psy + psx ) );
	/* enable DMA2D clock */
	__HAL_RCC_DMA2D_CLK_ENABLE();
	/* stop the DMA2D first */
	DMA2D->CR &=~ ( DMA2D_CR_START );
	DMA2D->CR = DMA2D_R2M;
	DMA2D->OPFCCR = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->OOR = offline;
	/* set output addr */
	DMA2D->OMAR = addr;
	DMA2D->NLR = ( pey - psy + 1 ) | ( ( pex-psx + 1 ) << 16 );
	DMA2D->OCOLR = color;
	/* start the DMA2D */
	DMA2D->CR |= DMA2D_CR_START;
	/* waitting for complete */
	while( ( DMA2D->ISR & ( DMA2D_FLAG_TC ) ) == 0 )
	{
		/* timeout */
		timeout++;
		/* out */
		if( timeout > 0X1FFFFF ) 
		{
			break;
		}
	} 
	/* clear TC flag */
	DMA2D->IFCR |= DMA2D_FLAG_TC ;
}
/* fill color*/
void fill_color(unsigned short psx,unsigned short psy,unsigned short pex,unsigned short pey,unsigned short * color)
{
	/* define some parm */
	unsigned int timeout = 0; 
	unsigned short offline;
	unsigned int addr; 
	/* calbrate the offline */
	offline = WIDTH - ( pex - psx + 1 ); 
	/* addr */
	addr = ( ( unsigned int )gram_buffer + 2 * ( WIDTH * psy + psx ) );
	/* enable DMA2D clock */
	__HAL_RCC_DMA2D_CLK_ENABLE();
	/* stop the DMA2D first */
	DMA2D->CR &=~ ( DMA2D_CR_START );
	DMA2D->CR = DMA2D_M2M;
	DMA2D->FGPFCCR = LTDC_PIXEL_FORMAT_RGB565;
	DMA2D->FGOR = 0;
	DMA2D->OOR = offline;
	/* set output addr */
	DMA2D->FGMAR = (unsigned int)color;
	DMA2D->OMAR = addr;
	DMA2D->NLR = ( pey - psy + 1 ) | ( ( pex - psx + 1 ) << 16 );
	/* start the DMA2D */
	DMA2D->CR |= DMA2D_CR_START;
	/* waitting for complete */
	while( ( DMA2D->ISR & ( DMA2D_FLAG_TC ) ) == 0 )
	{
		/* timeout */
		timeout++;
		/* out */
		if( timeout > 0X1FFFFF ) 
		{
			break;
		}
	} 
	/* clear TC flag */
	DMA2D->IFCR |= DMA2D_FLAG_TC ;	
}
/* clear */
void clear_display_dev(unsigned short color)
{
	fill_rect( 0 , 0 , WIDTH - 1 , HEIGHT - 1 , color );
}
/* ----------- end of file ----------------*/













