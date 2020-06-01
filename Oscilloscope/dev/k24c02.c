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
#include "fos.h"
#include "k24c02.h"
#include "hal_iic.h"
/* register node */
FOS_INODE_REGISTER("k24c02",k24c02_heap_init,k24c02_config_init,0,9);
/* heap init */
static int k24c02_heap_init(void)
{
	return FS_OK;
}
/* config */
static int k24c02_config_init(void)
{
#if 0
	/* write read test */
	unsigned char te[5] = {1,2,3,4,5};
	osc_k24c02_write(0,te,5);
	
	unsigned char rd[5];
	
	osc_k24C02_read(0,rd,5);
#endif	
	return FS_OK;
}
/* write delay */
static void osc_k24c02_delay(unsigned int t)
{
	/* am */
	t *= 200000;
	/* wait */
	while(t--);
}
/* 2401 read one byte */
static unsigned char AT24CXX_ReadOneByte(unsigned short ReadAddr)
{				  
	unsigned char temp=0;		
  /* iic start */	
	IIC_Start();  
  /* send addr */
	IIC_Send_Byte(0XA0+((ReadAddr/256)<<1));
	IIC_Wait_Ack(); 
	/* send addr */
	IIC_Send_Byte(ReadAddr%256);
	IIC_Wait_Ack();	
  /* send addr */	
	IIC_Start();  	 	   
	IIC_Send_Byte(0XA1);
  /* wait */	
	IIC_Wait_Ack();	 
	/* read */
	temp = IIC_Read_Byte(0);		  
  /* stop */	
	IIC_Stop();
	/* return */
	return temp;
}
/* write one byte */
static void AT24CXX_WriteOneByte(unsigned short WriteAddr,unsigned char DataToWrite)
{				   	 
   /* iic start */		
	IIC_Start();  
	/* send addr */
	IIC_Send_Byte(0XA0+((WriteAddr/256)<<1));
	IIC_Wait_Ack();	   
	/* send addr */
	IIC_Send_Byte(WriteAddr%256);
	IIC_Wait_Ack(); 	
  /* send addr */	
	IIC_Send_Byte(DataToWrite);						   
	IIC_Wait_Ack();  
	/* stop */
	IIC_Stop();
	osc_k24c02_delay(10);	 
}
/* read many bytes */
void osc_k24C02_read(unsigned short ReadAddr,void * pBuffer_v,unsigned short NumToRead)
{
	/* change */
	unsigned char * pBuffer = pBuffer_v;
	/*--------*/
	while(NumToRead)
	{
		*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++);	
		NumToRead--;
	}
}  
/* write many datas */
void osc_k24c02_write(unsigned short WriteAddr,void *pBuffer_v,unsigned short NumToWrite)
{
	/* change */
	unsigned char * pBuffer = pBuffer_v;
	/*--------*/	
	while(NumToWrite--)
	{
		AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
		WriteAddr++;
		pBuffer++;
	}
}























