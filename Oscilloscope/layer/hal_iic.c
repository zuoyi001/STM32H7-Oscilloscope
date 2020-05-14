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
#include "hal_gpio.h"
#include "fos.h"
#include "hal_iic.h"
/* register a init inode */
FOS_INODE_REGISTER("hal_iic",hal_iic_init,0,0,13);
/* hal iic init */
static int hal_iic_init(void)
{
	/* init iic hardware */
	iic_init();
	/* return OK */
	return FS_OK;
}
/* delay for iic */
static void delay_us_ic(unsigned int t)
{
	/* set a simple delay for iic bus */
	t *= 200;
	/* wait */
	while(t--);
}
/* iic init */
static void iic_init(void)
{
	GPIO_InitTypeDef GPIO_Initure;
  /* clock */
	__HAL_RCC_GPIOC_CLK_ENABLE();
  /* pin init */
	GPIO_Initure.Pin = GPIO_PIN_14|GPIO_PIN_15;
	GPIO_Initure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_Initure.Pull = GPIO_PULLUP;
	GPIO_Initure.Speed = GPIO_SPEED_FAST;
	HAL_GPIO_Init(GPIOC,&GPIO_Initure);
  /* set up */
	IIC_SDA(1);
	IIC_SCL(1);  
}
/* iic start */
void IIC_Start(void)
{
	SDA_OUT(); 
	IIC_SDA(1);	  	  
	IIC_SCL(1);
	delay_us_ic(4);
 	IIC_SDA(0);
	delay_us_ic(4);
	IIC_SCL(0);
}	  
/* iic stop */
void IIC_Stop(void)
{
	SDA_OUT();
	IIC_SCL(0);
	IIC_SDA(0);
 	delay_us_ic(4);
	IIC_SCL(1); 
	delay_us_ic(4);			
	IIC_SDA(1);		   	
}
/* iic wait ack */
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	/* set gpio in */
	SDA_IN();
	IIC_SDA(1);
	delay_us_ic(1);	   
	IIC_SCL(1);
	delay_us_ic(1);	 
	while(READ_SDA)
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL(0);
  /* return OK */	
	return 0;  
} 
/* iic ack */
void IIC_Ack(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(0);
	delay_us_ic(2);
	IIC_SCL(1);
	delay_us_ic(2);
	IIC_SCL(0);
}
/* NAVCK */		    
void IIC_NAck(void)
{
	IIC_SCL(0);
	SDA_OUT();
	IIC_SDA(1);
	delay_us_ic(2);
	IIC_SCL(1);
	delay_us_ic(2);
	IIC_SCL(0);
}					 				     
/* send data */	  
void IIC_Send_Byte(unsigned char txd)
{                        
	unsigned char t;   
	SDA_OUT(); 	    
	IIC_SCL(0);
	for(t = 0;t<8;t++)
	{              
		IIC_SDA((txd&0x80)>>7);
		txd<<=1; 	  
		delay_us_ic(2);
		IIC_SCL(1);
		delay_us_ic(2); 
		IIC_SCL(0);	
		delay_us_ic(2);
	}	 
} 	    
/* read data */
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive = 0;
	SDA_IN();
  for( i = 0;i<8;i++ )
	{
		IIC_SCL(0); 
		delay_us_ic(2);
		IIC_SCL(1);
		receive<<=1;
		if(READ_SDA)receive++;   
		delay_us_ic(1); 
  }					 
	if (!ack)
			IIC_NAck();
	else
			IIC_Ack();
	
    return receive;
}
/* dac create th */
void dac_update(unsigned short volA,unsigned short B,unsigned short C,unsigned short D)
{
	unsigned short outA = volA,outB = volA,outC = C,outD = C;
	
	IIC_Start();
	
	IIC_Send_Byte(0xC0);
	
	IIC_Wait_Ack();
	
	IIC_Send_Byte(0x40);

	IIC_Wait_Ack();
	
	IIC_Send_Byte(0x80 | (outA&0x0f00) >>8);
	
	IIC_Wait_Ack();
	
	IIC_Send_Byte( outA & 0xff );	

	IIC_Wait_Ack();	
	
	/* out B */
  IIC_Send_Byte(0x42);

	IIC_Wait_Ack();
	
	IIC_Send_Byte(0x80 | (outB&0x0f00) >>8);
	
	IIC_Wait_Ack();
	
	IIC_Send_Byte( outB & 0xff );	

	IIC_Wait_Ack();		
	
	/* outC */
  IIC_Send_Byte(0x44);

	IIC_Wait_Ack();
	
	IIC_Send_Byte(0x80 | (outC&0x0f00) >>8);
	
	IIC_Wait_Ack();
	
	IIC_Send_Byte( outC & 0xff );	

	IIC_Wait_Ack();	
	/* outD */
  IIC_Send_Byte(0x46);

	IIC_Wait_Ack();
	
	IIC_Send_Byte(0x80 | (outD&0x0f00) >>8);
	
	IIC_Wait_Ack();
	
	IIC_Send_Byte( outD & 0xff );	

	IIC_Wait_Ack();		
/* stop */	
	IIC_Stop();
}























