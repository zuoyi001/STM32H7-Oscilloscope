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
#include "osc.h"
#include "gui.h"
#include "fos.h"
#include "hal_gpio.h"
#include "hal_tim.h"
#include "osc_api.h"
#include "string.h"
#include "hal_iic.h"
/* static temp cache */
static unsigned char cache_fifo[4][ FIFO_DEEP * 2 ]  __attribute__((at(0x30000000)));
/* dac */
static unsigned short osc_dac_buffer[4];
static unsigned short osc_chn_offset[2];
/* Private includes ----------------------------------------------------------*/
void osc_stop_adc_clock(void)
{
	/* select internal clock and disable the clock */
	hal_write_gpio(DIO_CLOCK_SEL,0);
	hal_pwm_stop();
	/* end */
}
/* start clock */
void osc_start_adc_clock(unsigned char internal)
{
	/* internal clock or not */
	if( internal == 0 )
	{
		/* internal clock */
		hal_write_gpio(DIO_CLOCK_SEL,0);
		/* restart pwm */
		hal_pwm_start();
		/* end */
	}
	else
	{
		/* restart pwm */
		hal_pwm_stop();	
		/* ex clock */
		hal_write_gpio(DIO_CLOCK_SEL,1);		
	}
}
/* static delay_us but maybe not usefull */
static void delay_us_fifo(unsigned int t)
{
//	t *= 100;
//	/* waitting */
//	while(t--);
}
/* read data from fifo */
void osc_read_fifo_data(unsigned char clock_sta)
{
	/* temp data */
	unsigned char bit_q[8];
	/* unsigned clock sta neg */
	unsigned char sct = ( clock_sta == 1 ) ? 0 : 1;
	unsigned char nsc = ( clock_sta == 1 ) ? 1 : 0;
	/* start read r0*/
	hal_write_gpio(DIO_R0,1);
	/* start read r0 */
	for( int i = 0 ; i < FIFO_DEEP ; i ++ )
	{
		/* create a falling edge */
		hal_write_gpio(DIO_R0,0);
		/* delay some time for read */
		delay_us_fifo(1);
		/* read data */
		unsigned char temp = hal_read_gpio(FIFO_DATA);
		/* transfer */
		bit_q[7] = ( temp & ( 1 << 3 )) >> 3;
		bit_q[6] = ( temp & ( 1 << 7 )) >> 7;
		bit_q[5] = ( temp & ( 1 << 2 )) >> 2;
		bit_q[4] = ( temp & ( 1 << 6 )) >> 6;
		bit_q[3] = ( temp & ( 1 << 1 )) >> 1;
		bit_q[2] = ( temp & ( 1 << 5 )) >> 5;
		bit_q[1] = ( temp & ( 1 << 0 )) >> 0;
		bit_q[0] = ( temp & ( 1 << 4 )) >> 4;
		/* transfer */
		cache_fifo[0][i*2+sct] = ( bit_q[7] << 7 ) | 
														 ( bit_q[6] << 6 ) |
														 ( bit_q[5] << 5 ) |
														 ( bit_q[4] << 4 ) |
														 ( bit_q[3] << 3 ) |
														 ( bit_q[2] << 2 ) |
														 ( bit_q[1] << 1 ) |
														 ( bit_q[0] << 0 );
		/* read data  */
		cache_fifo[2][i*2+sct] = hal_read_gpio(FIFO_DATA_D8) ? 92 : 0;
		/* ------ */
		hal_write_gpio(DIO_R0,1);
		/* delay some time for read */
		delay_us_fifo(1);
	}
	/* read channel a nes */
	/* start read r1 -----------------------------------------------------------------*/
	hal_write_gpio(DIO_R1,1);
	/* start read r1 */
	for( int i = 0 ; i < FIFO_DEEP ; i ++ )
	{
		/* create a falling edge */
		hal_write_gpio(DIO_R1,0);
		/* delay some time for read */
		delay_us_fifo(1);
		unsigned char temp =  hal_read_gpio(FIFO_DATA);
		/* transfer to corret firmat */
		cache_fifo[0][i*2+nsc] = ((temp & 0xf0) >> 4 ) | ((temp & 0x0f) << 4 );
    /* read data  */
		cache_fifo[2][i*2+nsc] = hal_read_gpio(FIFO_DATA_D8) ? 92 : 0;
		/* restart */
	  hal_write_gpio(DIO_R1,1);
		/* delay some time for read */
		delay_us_fifo(1);
	}
  /* start read r2 -----------------------------------------------------------------*/
	hal_write_gpio(DIO_R2,1);
	/* start read r2 */
	for( int i = 0 ; i < FIFO_DEEP ; i ++ )
	{
		/* create a falling edge */
		hal_write_gpio(DIO_R2,0);
		/* delay some time for read */
		delay_us_fifo(1);
		/* read data */
		unsigned char temp =  hal_read_gpio(FIFO_DATA);
		/* transfer to corret firmat */
		/* set */
		cache_fifo[1][i*2+sct] = (( temp & 0xF ) << 4 ) | ( temp >> 4 );
    /* read data  */
		cache_fifo[3][i*2+sct] = hal_read_gpio(FIFO_DATA_D8) ? 92 : 0;
		/* restart */
	  hal_write_gpio(DIO_R2,1);
		/* delay some time for read */
		delay_us_fifo(1);
	}	
  /* start read r3 -----------------------------------------------------------------*/
	hal_write_gpio(DIO_R3,1);
	/* start read r2 */
	for( int i = 0 ; i < FIFO_DEEP ; i ++ )
	{
		/* create a falling edge */
		hal_write_gpio(DIO_R3,0);
		/* delay some time for read */
		delay_us_fifo(1);
		/* read data */
		unsigned char temp =  hal_read_gpio(FIFO_DATA);
		/* transfer to corret firmat */
		bit_q[7] = ( temp & ( 1 << 3 )) >> 3;
		bit_q[6] = ( temp & ( 1 << 7 )) >> 7;
		bit_q[5] = ( temp & ( 1 << 2 )) >> 2;
		bit_q[4] = ( temp & ( 1 << 6 )) >> 6;
		bit_q[3] = ( temp & ( 1 << 1 )) >> 1;
		bit_q[2] = ( temp & ( 1 << 5 )) >> 5;
		bit_q[1] = ( temp & ( 1 << 0 )) >> 0;
		bit_q[0] = ( temp & ( 1 << 4 )) >> 4;
		/* read data */
		cache_fifo[1][i*2+nsc] = ( bit_q[7] << 7 ) | 
														 ( bit_q[6] << 6 ) |
														 ( bit_q[5] << 5 ) |
														 ( bit_q[4] << 4 ) |
														 ( bit_q[3] << 3 ) |
														 ( bit_q[2] << 2 ) |
														 ( bit_q[1] << 1 ) |
														 ( bit_q[0] << 0 );
    /* read data  */
		cache_fifo[3][i*2+nsc] = hal_read_gpio(FIFO_DATA_D8) ? 92 : 0;
		/* restart */
	  hal_write_gpio(DIO_R3,1);
		/* delay some time for read */
		delay_us_fifo(1);
	}
}
/* wave data select */
void osc_trig_read(unsigned short * ch1_m,unsigned short * ch2_m,int trig_type,int trig_source,int tflag)
{
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();	
	/* trig flag */
	unsigned char t0,t1;
	unsigned char * trig_source_p;
	unsigned short trig_pos =  area->total_pixel_h / 2;
  /* new only supply rising edge and falling ,and one source */
  if( trig_type == TRIG_MODE_RISING )
	{
		t0 = 0;
		t1 = 92;
	}
	else if ( trig_type == TRIG_MODE_FALLING )
	{
		t0 = 92;
		t1 = 0;
	}
	else
	{
		t0 = 0xff;
		t1 = 0xff;
	}
	/* get trig source */
	trig_source_p = ( trig_source == TRIG_SOURCE_CH1 ) ? cache_fifo[2] : cache_fifo[3];
	/* search data */
	for( int i = area->total_pixel_h / 2 ; i < FIFO_DEEP * 2 - area->total_pixel_h / 2 ; i ++  )
	{
		/* search */
		if( trig_source_p[i] == t0 && trig_source_p[i+1] == t1 )
		{
			/* set a sample filter */
			if( trig_source_p[i-5] == t0 && 
					trig_source_p[i-4] == t0 && 
					trig_source_p[i-3] == t0 && 
					trig_source_p[i-2] == t0 && 
					trig_source_p[i-1] == t0 && 
					trig_source_p[i+2] == t1 &&
					trig_source_p[i+3] == t1 &&
					trig_source_p[i+4] == t1 &&
					trig_source_p[i+5] == t1 &&
					trig_source_p[i+6] == t1 )
				  {
						/* ok we find the pos*/
						trig_pos = i - area->total_pixel_h / 2 + 5;
						/* break ,and copy data */
						break;
					}
			/*----------------------*/
		}
	}
	/* copy data */
	osc_create_analog_data((signed char *)&cache_fifo[0][trig_pos],(signed char *)&cache_fifo[1][trig_pos],ch1_m,ch2_m);
	/* end */
}
/* create analog data to display dev */
static void osc_create_analog_data(signed char * ch1_o,signed char * ch2_o,unsigned short * ch1_m,unsigned short * ch2_m )
{
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* create the data */
  for( int i = 0 ; i < area->total_pixel_h ; i ++ )
	{
		/* create the ddtd */
		ch1_m[i] = area->total_pixel_v - (float)(ch1_o[i] + 128) / 255.0f * area->total_pixel_v;
		ch2_m[i] = area->total_pixel_v - (float)(ch2_o[i] + 128) / 255.0f * area->total_pixel_v;
	}	
}
/* osc output dac */
void osc_voltage_output(unsigned short a,unsigned short b,unsigned short c,unsigned short d)
{
	/* save buffer */
	osc_dac_buffer[0] = a;
	osc_dac_buffer[1] = b;
	osc_dac_buffer[2] = c;
	osc_dac_buffer[3] = d;
	/* update dac */
  dac_update(a,b,c,d);
}
/* dac event */
static void osc_dac_update_buf(unsigned short * buf,unsigned short * offset)
{
	dac_update(buf[0],buf[1],buf[2] + offset[1],buf[3] + offset[0]);
}
/* set base dac */
void osc_vol_dac(unsigned char chn,unsigned short gain_dac,unsigned short offset_dac)
{
	/* chn */
	if( chn == 0 )
	{
		/* set buffer */
		osc_dac_buffer[0] = gain_dac;
		osc_dac_buffer[3] = offset_dac;
		/* update */
		osc_dac_update_buf(osc_dac_buffer,osc_chn_offset);
	}
	else
	{
		/* set buffer */
		osc_dac_buffer[1] = gain_dac;
		osc_dac_buffer[2] = offset_dac;
		/* update */
		osc_dac_update_buf(osc_dac_buffer,osc_chn_offset);		
	}
}
/* void osc_dac_offset */
void osc_dac_offset(unsigned char chn,unsigned short offset_mv)
{
	/* chn */
	if( chn == 0 )
	{
		/* set buffer */
		osc_chn_offset[0] = offset_mv;
		/* update */
		osc_dac_update_buf(osc_dac_buffer,osc_chn_offset);
	}
	else
	{
		/* set buffer */
		osc_chn_offset[1] = offset_mv;
		/* update */
		osc_dac_update_buf(osc_dac_buffer,osc_chn_offset);	
	}	
}
/* fifo clock enable */
void osc_fifo_clock(unsigned short sta)
{
	hal_write_gpio(FIFO_DIO_TR,sta);
}
/* unsigned read key menu */
unsigned short osc_read_key_menu(void)
{
	return hal_read_gpio(KEY_MENU);
}
/* coune */
int osc_read_rot_idle(unsigned index)
{
	unsigned short ret = 0;
	/* index */
	switch(index)
	{
		case 0:
			ret = hal_read_gpio(DIO_ROT_TIME_UP) || hal_read_gpio(DIO_ROT_TIME_DM);
		break;
		case 1:
			ret = hal_read_gpio(DIO_ROT_VOL_UP) || hal_read_gpio(DIO_ROT_VOL_DM);
		break;
		case 2:
			ret = hal_read_gpio(DIO_ROT_TRIG_UP) || hal_read_gpio(DIO_ROT_TRIG_DM);
		break;		
		case 3:
			ret = hal_read_gpio(DIO_ROT_HORI_UP) || hal_read_gpio(DIO_ROT_HORI_DM);
		break;		
		default:
			ret = 1;
		break;
	}
	/* get osc data */
	return ret;
}
/* gain ctrl */
void osc_gain_ctrl(unsigned char chn , unsigned char sta)
{
	/* gain ctrl */
	if( chn == 0 )
	{
		hal_write_gpio(DIO_CH1_GAIN_CTRL,sta);
	}
	else
	{
		hal_write_gpio(DIO_CH2_GAIN_CTRL,sta);
	}
}









































