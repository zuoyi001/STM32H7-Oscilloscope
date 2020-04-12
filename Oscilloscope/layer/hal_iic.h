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
#ifndef _MYIIC_H
#define _MYIIC_H

#define SDA_IN()  {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=0<<11*2;}
#define SDA_OUT() {GPIOB->MODER&=~(3<<(11*2));GPIOB->MODER|=1<<11*2;}

#define IIC_SCL   PBout(10) //SCL
#define IIC_SDA   PBout(11) //SDA
#define READ_SDA  PBin(11)  //

static void iic_init(void);             				 
void IIC_Start(void);
void IIC_Stop(void);	 
void IIC_Send_Byte(unsigned char txd);			
unsigned char IIC_Read_Byte(unsigned char ack);
unsigned char IIC_Wait_Ack(void); 
void IIC_Ack(void);
void IIC_NAck(void);

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned char IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	 
static int hal_iic_init(void);
void dac_update(unsigned short volA,unsigned short B,unsigned short C,unsigned short D);

#endif



