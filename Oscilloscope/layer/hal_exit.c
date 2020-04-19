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
#include "main.h"
#include "osc_api.h"
#include "fos.h"
/* exit init */
static int osc_exit_rot_init(void);
static void exit_thread(void);
static void osc_rot_thread(unsigned char index);
/* exit init  */
FOS_INODE_REGISTER("exit_rot",osc_exit_rot_init,0,0,1);
/*----------------------------------------------------------------------------*/
FOS_TSK_REGISTER(exit_thread,PRIORITY_1,2); /* gui detecter task run as 100 ms*/
/* static rot_flag */
static unsigned char rot_flag[4];
static signed short rot_updm[4];
/* static exit thread */
static void exit_thread(void)
{
	/* check */
	osc_rot_thread(0);
	osc_rot_thread(1);
	osc_rot_thread(2);
	osc_rot_thread(3);
}
/* exit init */
static int osc_exit_rot_init(void)
{
	/* exit the it */
	HAL_NVIC_SetPriority(EXTI1_IRQn,0,0);       
	HAL_NVIC_EnableIRQ(EXTI1_IRQn);            
  /* exit the it */
	HAL_NVIC_SetPriority(EXTI2_IRQn,0,0);       
	HAL_NVIC_EnableIRQ(EXTI2_IRQn); 
  /* exit the it */
	HAL_NVIC_SetPriority(EXTI4_IRQn,0,0);       
	HAL_NVIC_EnableIRQ(EXTI4_IRQn); 
	/* exit the it */
	HAL_NVIC_SetPriority(EXTI9_5_IRQn,0,0);       
	HAL_NVIC_EnableIRQ(EXTI9_5_IRQn); 
  /* exit the it */
	HAL_NVIC_SetPriority(EXTI15_10_IRQn,0,0);  
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn); 
	/* init return OK */
	return FS_OK;
}
/* get chanbe */
static void osc_rot_isr(unsigned char index,unsigned char up_dm)
{
	/* get clear */
	if( rot_flag[index] == 0 )
	{
		/* set flag */
		rot_flag[index] = 1;
		/* def */
		if( up_dm == 0 )
		{
			rot_updm[index] ++;
		}
		else
		{
			rot_updm[index] --;
		}
	}
}
/* osc rot read */
signed short osc_rot_sta(unsigned char index)
{
	return rot_updm[index];
}
/* void set osc_rot */
void osc_rot_set(unsigned char index,signed short da)
{
	rot_updm[index] = da;
}
/* get osc */
static void osc_rot_thread(unsigned char index)
{
	/* get rot idle */
	if( rot_flag[index] == 1 )
	{
		/* read data */
		if( osc_read_rot_idle(index) == 0 )
		{
			rot_flag[index] = 0;
		}	
	}
}
/* set exit */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	/* get IO */
	switch(GPIO_Pin)
	{
		case GPIO_PIN_1:
			/* rot0 */
		  osc_rot_isr(0,0);
			break;
		case GPIO_PIN_2:
			/* rot1 */
		  osc_rot_isr(1,0);			
			break;
		case GPIO_PIN_4:
			/* rot3 */
		  osc_rot_isr(3,1);			
			break;
		case GPIO_PIN_5:
			/* rot3 */
		  osc_rot_isr(3,0);			
			break;
		case GPIO_PIN_6:
			/* rot0 */
		  osc_rot_isr(0,1);			
			break;
		case GPIO_PIN_13:
			/* rot2 */
		  osc_rot_isr(2,1);			
			break;
		case GPIO_PIN_14:
			/* rot2 */
		  osc_rot_isr(2,0);			
			break;
		case GPIO_PIN_15:
			/* rot1 */
		  osc_rot_isr(1,1);			
			break;		
		default :
			break;
	}
	/* end of data */
}
/* exit isr */
void EXTI1_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}
/* exit isr */
void EXTI2_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}
/* exit isr */
void EXTI4_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_4);
}
/* exit isr */
void EXTI9_5_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_6);
}
/* exit isr */
void EXTI15_10_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_13);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_14);
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_15);
}













