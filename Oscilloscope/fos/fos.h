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
#ifndef __FOS_H__
#define __FOS_H__	

/* Includes ------------------------------------------------------------------*/
#define FS_OK    (0)
#define FS_ERR   (-1)
#define FS_ECO   (-2)
/* #define */
#define FOS_PRIORITY  (0xF)
/* USER CODE END Header */
/* storage dev opd def */
typedef struct
{
	/* high-end function just for storage's files */
	int (*opendir)(const char *path);
	int (*mkdir)(const char *dir);
	int (*mkfs)(unsigned char drv,unsigned char sfd,unsigned int au);
	int (*lseek)(unsigned int offset, unsigned int whence);
	int (*sync)(void);	
	int (*close)(void);
}STORAGE_DEV;
/* inode init */
typedef struct
{
	/* The following methods must be identical in signature and position because
	 * the struct file_operations and struct mountp_operations are treated like
	 * unions.
	 */
	/* The device driver open method differs from the mountpoint open method */
	void * (*open) (void);
	/* write function */
	int (*write)( const void * buffer, unsigned int buflen);
	/* read function */
	unsigned int (*read)( void * buffer, unsigned int buflen);
  /* ioctrl */
	int (*ioctl)( int cmd, unsigned long arg,void *pri_data);
	/* storage device */
	STORAGE_DEV * storage_dev;
	/* end of data */
}file_ops_def;
/* fos node def */
typedef struct 
{
	/* node name for searching */
	char * name;
	/* heap init function */
	int (*head_init)(void);
	/* the function use for create task */
	int (*config)(void);
	/* file ops */
	file_ops_def * ops;
	/* set up seq and cmd type define */
	/* bit7~0 is the set up seq of set up */
	/* other bits is for other apps */
	unsigned int seq_type;
	/* other settings */
}inode_vmn;
/* Define the node usage of the current module */
typedef struct
{
	/* node start addr */
	inode_vmn * addr;
	/* inode num */
	unsigned int inode_num;
	/* end of struct */
}inode_msg_def;
/* ARM complies setting */
extern inode_vmn fs_vmn$$Base;
extern inode_vmn fs_vmn$$Limit;
/* define head and limit MAC */
#define FOS_VMN_BASE    ((inode_vmn *)&fs_vmn$$Base)
/* max addr for vmn */
#define FOS_VMN_LIMIT  ((inode_vmn *)&fs_vmn$$Limit)
/* num for limit */
#define FOS_VMN_NUM   (&fs_vmn$$Limit-&fs_vmn$$Base)
/*----------end---------*/
#define __INIT __attribute__((unused, section("fs_vmn")))
#define FOS_INODE_REGISTER(name,init,config,ops,st)                            \
const inode_vmn __FS_##init                                                    \
__INIT =                                                                       \
{                                                                              \
	name,   /* name for inode */                                                 \
	init,   /* heap init function that will first set up */                      \
  config, /* use for create task etc. */                                       \
	ops,    /* file ops */                                                       \
	st      /* setting set up seq and type */                                    \
}/* they will take 20 Byte */                                                  \
/* functions decleare */
int fs_system_initialization(void);
/* end of files */
#endif




























