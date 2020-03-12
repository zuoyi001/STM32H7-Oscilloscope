/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : notify.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
	* BEEP TIM3 CHANNEL1 PWM Gerente
	* LED is TIM4 CH3 and CH4
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#ifndef __F_OPS_H__
#define __F_OPS_H__
/* ips */
#include "fs.h"
/* fs write */
static int fs_write(struct inode * filp,const void * buffer , unsigned int buflen)  
{ 
	/* judge */
	if( filp != 0 && filp->ops->write != 0 )
	{
		return filp->ops->write(filp,buffer,buflen);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs read */
static int fs_read(struct inode * filp,void *buffer , unsigned int buflen)   
{
	/* judge */
	if( filp != 0 && filp->ops->read != 0 )
	{
		return filp->ops->read(filp,buffer,buflen);
	}
	else
	{
		return 0;
	}
}
/* fs sync */
static int fs_sync(struct inode * filp)
{
	/* judge */
	if( filp != 0 && 
		filp->ops->storage_dev != 0 &&
		filp->ops->storage_dev->sync != 0 )
	{	
	  return filp->ops->storage_dev->sync(filp);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs close */
static int fs_close(struct inode * filp)
{
	/* judge */
	if( filp != 0 &&
		filp->ops->storage_dev != 0 &&
	  filp->ops->storage_dev->close != 0 )
	{	
	  return filp->ops->storage_dev->close(filp);
	}
	else
	{
		return FS_ERR;
	}	
}           
/* fs opendir */
static int fs_opendir( struct inode * filp , const char *path )     
{
	/* judge */
	if( filp != 0 && 
		filp->ops->storage_dev != 0 &&
		filp->ops->storage_dev->opendir != 0 )
	{		
	  return filp->ops->storage_dev->opendir(path);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs seek */
static int fs_seek(struct inode *filp, unsigned int offset, unsigned int whence)
{
	/* judge */
	if( filp != 0 && 
		filp->ops->storage_dev != 0 &&
		filp->ops->storage_dev->lseek != 0 )
	{		
	  return filp->ops->storage_dev->lseek(filp,offset,whence);
	}
	else
	{
		return FS_ERR;
	}
}
/*fs ioctrl */
static int fs_ioctl(struct inode *filp, int cmd, unsigned long arg,void *pri_data)
{
	/* judge */
	if( filp != 0 && filp->ops->ioctl != 0 )
	{	
	   return filp->ops->ioctl(filp,cmd,arg,pri_data);
	}
	else
	{
		return FS_ERR;
	}
}
/* fs readdir */
static int fs_readdir( FAR struct inode *filp, const char *path,readdir_entrance_def * buffer )     
{
	/* judge */
	if( filp != 0 && 
		filp->ops->storage_dev != 0 &&
		filp->ops->storage_dev->readir != 0 )
	{		
	  return filp->ops->storage_dev->readir(filp,path,buffer);
	}
	else
	{
		return FS_ERR;
	}
}
#endif /* __F_OPS_H__ */





























