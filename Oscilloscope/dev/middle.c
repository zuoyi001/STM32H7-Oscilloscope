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
/* Private includes ----------------------------------------------------------*/
static display_info_def * dev_info;
static unsigned short * gram_buffer = NULL;
static unsigned short WIDTH = 0 , HEIGHT = 0;

void create_grid_date(unsigned short width_t,unsigned short height_t);
	
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
	
	create_grid_date(WIDTH,HEIGHT);
	/* return OK */
	return 0;
}
/* middle functions set point */
static void set_point( unsigned short x , unsigned short y , unsigned int color )
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
/* test */
static unsigned short N, M,EN,EM,ENN,EMM;

void calbrate_osc_ui(unsigned short width, unsigned short height)
{
	unsigned short v_grid = (width - 20 - 2) / (5 * 10);

	N = width - 20 - 1 - v_grid * (5 * 10) - 1;

	EN = v_grid * (5 * 10) / 10 / 5;

	ENN = width - v_grid * (5 * 10) - 20 - 1 - 1;

	unsigned short h_grid = (height - 16 - 2 - 45) / (5 * 8);

	M = height - 16 - 1 - h_grid * (5 * 8) - 1;

	EM = h_grid * (5 * 8) / 8 / 5;

	EMM = height - h_grid * (5 * 8) - 16 - 1 - 1;
}

#define RGB(r,g,b) (( (r&0xF8) << 8 ) | ((g&0xFC) << 3 ) | ((b&0xF8)>>3))

void create_grid_date(unsigned short width_t,unsigned short height_t)
{	
	unsigned short biWidth = width_t;
	unsigned short biHeight = height_t;

	calbrate_osc_ui(biWidth, biHeight);

/* sraw */

	for (int j = 0; j < EN * (5 * 10) ; j++)
	{
		for (int i = 0; i < EM * (5 * 8) + 1 ; i++)
		{
			set_point(20 + 1 + j, 16 + 1 + i, RGB(7, 3, 7));
		}
	}

	for (int j = 0; j < (8 + 1); j++)
	{
		for (int i = 0; i < (5*10 + 1); i++)
		{
			set_point(20 + 1 + i * EN, 16 + 1 + j*EM*5, RGB(199, 195, 199));
		}
	}

	for (int j = 0; j < (10 + 1); j++)
	{
		for (int i = 0; i < (5 * 8 + 1); i++)
		{
			set_point(20 + 1 + j * EN * 5 , 16 + 1 + i * EM, RGB(199, 195, 199));
		}
	}

	for (int i = 0; i < EN * (5 * 10); i++)
	{
		set_point(20 + 1 + i, 16 , RGB(199, 195, 199));

		if ((i % EN) == 0)
		{
			set_point(20 + 1 + i, 16 + 2, RGB(199, 195, 199));
			set_point(20 + 1 + i, 16 + 5, RGB(199, 195, 199));
		}
	}

	for (int i = 0; i < EN * (5 * 10); i++)
	{
		set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8) + 1, RGB(199, 195, 199));

		if (((i % EN)) == 0 )
		{
			set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8) + 1 - 3, RGB(199, 195, 199));
			set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8) + 1 - 6, RGB(199, 195, 199));
		}
	}

	for (int i = 0; i < EM * (5 * 8) + 1; i++)
	{
		set_point(20 , 16 + 1  + i, RGB(199, 195, 199));
		set_point(20 + 1 + EN * (5 * 10) , 16 + 1 + i, RGB(199, 195, 199));
		if (((i % EM)) == 0)
		{
			set_point(20 + 2, 16 + 1 + i, RGB(199, 195, 199));
			set_point(20 + 5, 16 + 1 + i, RGB(199, 195, 199));

			set_point(20 + 1 + EN * (5 * 10) - 2 , 16 + 1 + i, RGB(199, 195, 199));
			set_point(20 + 1 + EN * (5 * 10) - 5, 16 + 1 + i, RGB(199, 195, 199));
		}
	}

	for (int i = 0; i < EN * (5 * 10); i++)
	{
		set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8 / 2), RGB(199, 195, 199));

		if (((i % EN)) == 0)
		{
			set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8 / 2) - 2, RGB(199, 195, 199));
			set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8 / 2) - 5, RGB(199, 195, 199));
			set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8 / 2) + 1, RGB(199, 195, 199));
			set_point(20 + 1 + i, 16 + 1 + EM * (5 * 8 / 2) + 4, RGB(199, 195, 199));
		}
	}

	for (int i = 0; i < EM * (5 * 8) + 1; i++)
	{
		set_point(20 + 1 + EN * (5 * 10 / 2), 16 + 1 + i, RGB(199, 195, 199));

		if (((i % EM)) == 0)
		{
			set_point(20 + 1 + EN * (5 * 10 / 2) - 2, 16 + 1 + i, RGB(199, 195, 199));
			set_point(20 + 1 + EN * (5 * 10 / 2) - 5, 16 + 1 + i, RGB(199, 195, 199));
			set_point(20 + 1 + EN * (5 * 10 / 2) + 1, 16 + 1 + i, RGB(199, 195, 199));
			set_point(20 + 1 + EN * (5 * 10 / 2) + 4, 16 + 1 + i, RGB(199, 195, 199));
		}

		if (((i % (EM * 5))) == 0)
		{
			set_point(20 + 1 + EN * (5 * 10 / 2) - 8, 16 + 1 + i, RGB(199, 195, 199));
			set_point(20 + 1 + EN * (5 * 10 / 2) + 7, 16 + 1 + i, RGB(199, 195, 199));
		}
	}

	set_point(20 , 16 , RGB(199, 195, 199));
	set_point(20, 16 + EM * (5*8) + 1 + 1, RGB(199, 195, 199));
	set_point(20 + EN * (5 * 10) + 1, 16, RGB(199, 195, 199));

	set_point(20 + EN * (5 * 10) + 1, 16 + EM * (5 * 8) + 1 + 1, RGB(199, 195, 199));

	for (int j = 0; j < biHeight; j++)
	{
		for (int i = 0; i < 20; i++)
		{
			set_point(i, j, RGB(63, 75, 151));
		}

		for (int i = 0; i < ENN; i++)
		{
			set_point(i + 20 + EN * (5 * 10) +1 +1, j, RGB(63, 75, 151));
		}
	}

	for (int j = 0; j < biWidth; j++)
	{
		for (int i = 0; i < 16; i++)
		{
			set_point(j, i, RGB(63, 75, 151));
		}

		for (int i = 0; i < EMM - 1; i++)
		{
			set_point(j, i + 16 + 1 + 1 + 1 + EM * (5 * 8), RGB(63, 75, 151));
		}
	}
	double sin_x = 0;
	/* sinx */
	for( int i = 0 ; i < EN * (5 * 10) ; i ++ )
	{
	 double te = sin( sin_x );
		
   short tm = (short)( te * EM * (5 * 8 / 4) ) + EM * (5 * 8 / 2) + 16 + 1 - EM*5;
		
	 double tce = cos( sin_x );
		
   short tcm = (short)( tce * EM * (5 * 8 / 4) ) + EM * (5 * 8 / 2) + 16 + 1 + EM*5;	 
		
		set_point( 20 + 1 + i , tm , RGB(255,255,7));
		
		
		set_point( 20 + 1 + i , tcm , RGB(7,227,231));
		
		sin_x += (6.28) / (double)(EN * (5 * 10)) * 3;
		
	}
	
}


