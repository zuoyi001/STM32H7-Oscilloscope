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
#include "hal_iic.h"
#include "gui.h"
/* function */
static int touch_init(void);
static int osc_touch_config(void);
static void touch_task_idle(void);
/* gui dev */
static gui_dev_def * dev;
/* touch sta */
static int touch_sta = FS_ERR;
/* register node */
FOS_INODE_REGISTER("touch",touch_init,osc_touch_config,0,15);
/* idle task */
FOS_TSK_REGISTER(touch_task_idle,PRIORITY_IDLE,100);
/* iic test cmd */
#define FT_CMD_WR 				      (0x70)
#define FT_CMD_RD 				      (0x71)
/* reg */
#define FT_DEVIDE_MODE 			    (0x00)
#define FT_REG_NUM_FINGER       (0x02)

#define FT_TP1_REG 				      (0x03)
#define FT_TP2_REG 				      (0x09)
#define FT_TP3_REG 				      (0x0F)
#define FT_TP4_REG 				      (0x15)
#define FT_TP5_REG 				      (0x1B) 
 
#define	FT_ID_G_LIB_VERSION		  (0xA1)
#define FT_ID_G_MODE 			      (0xA4)
#define FT_ID_G_THGROUP			    (0x80) 
#define FT_ID_G_PERIODACTIVE	  (0x88) 
/* tables */
const unsigned short FT5206_TPx_TBL[5] = 
{ FT_TP1_REG , FT_TP2_REG , FT_TP3_REG , 
  FT_TP4_REG , FT_TP5_REG };
/* point */
static unsigned short postx[5] , posty[5];
/* color table */
const unsigned int color[5] = {2,3,4,5,6};
/* base interface */
static unsigned char FT5206_WR_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
  /* wr reg */
	unsigned char ret=0;
	/* iic start */
	IIC_Start();
	/* send wr cmd */
	IIC_Send_Byte(FT_CMD_WR);
	/* wait ack */
	IIC_Wait_Ack(); 	 	
  /* send cmd */	
	IIC_Send_Byte(reg&0xFF);
	/* wait ack */
	IIC_Wait_Ack();  
	/* send data  */
	for(int i=0;i<len;i++)
	{	   
		/* sent one byte */
		IIC_Send_Byte(buf[i]);
		/* wait ack */
		ret = IIC_Wait_Ack();
		/* check */
		if(ret)
		{
			break; 
		} 
	}
	/* iic stop */
	IIC_Stop();   
  /* return */	
	return ret; 
}
/* read data */			  
static void FT5206_RD_Reg(unsigned short reg,unsigned char *buf,unsigned char len)
{
	/* iic start */ 
	IIC_Start();	
	/* send wr cmd */
	IIC_Send_Byte(FT_CMD_WR);
	/* wait ack */
	IIC_Wait_Ack(); 	 										  		   
	IIC_Send_Byte(reg&0xFF);
	/* wait ack */
	IIC_Wait_Ack();  
	IIC_Start();  	 	   
	IIC_Send_Byte(FT_CMD_RD);
	/* wait ack */	
	IIC_Wait_Ack();	   
	/* read data  */
	for( int i = 0 ; i < len ; i++ )
	{	   
		buf[i] = IIC_Read_Byte( i == ( len - 1 ) ? 0 : 1 );
	} 
	/* iic stop */
	IIC_Stop(); 
	/* end of */
} 
/* init */
static int touch_init(void)
{
	/* tmp */
	unsigned char temp[2];
	/* send cmd */
  FT5206_WR_Reg(FT_DEVIDE_MODE,temp,1);
	FT5206_WR_Reg(FT_ID_G_MODE,temp,1);
	/* sen */
	temp[0] = 22;
	/* valid */
	FT5206_WR_Reg(FT_ID_G_THGROUP,temp,1);
	/* time */
	temp[0]=12;
	/* active */
	FT5206_WR_Reg(FT_ID_G_PERIODACTIVE,temp,1); 
	/* read version */
	FT5206_RD_Reg(FT_ID_G_LIB_VERSION,&temp[0],2);  
	/* check */
	if( temp[0] == 0x30 && temp[1] == 0xF8 )
	{
		/* set touch ok */
		touch_sta = FS_OK;
		/* ok */
		return FS_OK;
	} 
	/* set touch ok */
	touch_sta = FS_ERR;	
	/* cannot find the touch pad*/	
	return FS_ERR;
}
/* static touch config */
static int osc_touch_config(void)
{
	/* gui dev get */
  dev = get_gui_dev();
	/* return */
	return FS_OK;
}
/* thread */
static void FT5206_Scan(unsigned char mode)
{
	unsigned char buf[4];
	unsigned char temp;
	unsigned char sta = 0;
	static unsigned char t = 0;
	/* freq */
	t++;
	/* enter */
	if( ( t % 10 ) == 0 || t < 10 )
	{
		/* read sta */
		FT5206_RD_Reg(FT_REG_NUM_FINGER,&mode,1);
		/* check */
		if( ( mode & 0xF ) && ( ( mode & 0xF ) < 6 ))
		{
			/* check */
			temp = 0xFF << ( mode & 0xF );
			/* sta */
			sta = (~temp)|0x80|0x40; 
			/* read point */
			for(int i = 0 ; i < 5 ; i++ )
			{
				if( sta & ( 1 << i ))
				{
					/* read x y */
					FT5206_RD_Reg(FT5206_TPx_TBL[i],buf,4);
					/* set point */
					postx[i]=((unsigned short)(buf[0]&0x0F)<<8)+buf[1];
					posty[i]=((unsigned short)(buf[2]&0x0F)<<8)+buf[3];
					/* set point */
					dev->set_noload_point(postx[i],posty[i],color[i]);
				}			
			} 
		  /* clear */
			t = 0;
		}
	}
	/* freq */	
	if( t > 240 )
	{
		t = 10;
	}
}
/* static idle task for test */
static void touch_task_idle(void)
{
	/* check  */
	if( touch_sta == FS_OK )
	{
		FT5206_Scan(0);
	}
}














