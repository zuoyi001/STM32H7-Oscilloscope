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
/* Private includes ----------------------------------------------------------*/
static display_info_def * dev_info;
static unsigned short * gram_buffer = NULL;
static unsigned short WIDTH = 0 , HEIGHT = 0;
/* Define all supported display panel information */

int middle_layer_init(void)
{
	/* get */
	dev_info = get_display_dev_info();
	/* get gram */
	if( dev_info == NULL )
	{
		/* get information error */
		return (-1);
	}
	/* get information OK */
	gram_buffer = (unsigned short *)dev_info->gram_addr;
	/* get display size */
	WIDTH =  dev_info->display_dev->pwidth;
	HEIGHT = dev_info->display_dev->pheight;
	/* return OK */
	return 0;
}
/* middle functions set point */
void set_point( unsigned short x , unsigned short y , unsigned int color )
{
	/* get GRAM */
	if( gram_buffer != NULL )
	{
		gram_buffer[ y * WIDTH + x ] = color;
	}
}
/* middle get point */
static unsigned short get_point( unsigned short x , unsigned short y )
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
static void fill_rect(unsigned short psx,unsigned short psy,unsigned short pex,unsigned short pey,unsigned int color)
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
//static void draw_point(unsigned short x,unsigned short y,unsigned int color)
//{ 
//#if LCD_PIXFORMAT==LCD_PIXFORMAT_ARGB8888||LCD_PIXFORMAT==LCD_PIXFORMAT_RGB888
//	if(lcdltdc.dir)	//∫·∆¡
//	{
//        *(unsigned int*)((unsigned int)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
//	}else 			// ˙∆¡
//	{
//        *(unsigned int*)((unsigned int)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x)+y))=color; 
//	}
//#else
//	if(lcdltdc.dir)	//∫·∆¡
//	{
//        *(unsigned short*)((unsigned int)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*y+x))=color;
//	}else 			// ˙∆¡
//	{
//        *(unsigned short*)((unsigned int)ltdc_framebuf[lcdltdc.activelayer]+lcdltdc.pixsize*(lcdltdc.pwidth*(lcdltdc.pheight-x-1)+y))=color; 
//	}
//#endif
//}




