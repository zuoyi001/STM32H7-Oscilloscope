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
#include "hal_tim.h"
#include "osc_cfg.h"
#include "osc.h"
#include "hal_exit.h"
#include "osc_ui.h"
#include "osc_api.h"
#include "hal_dac.h"
#include "stdio.h"
#include "osc_menu.h"
/* out dac part */
static signed short trig_dac_part_offset = 0;
static signed short trig_dac_part_rot = 0;
/* hold time */
static unsigned short trig_lines_hold_time_s = 0;
/* create cfg task gui detecter task run as 100 ms */
FOS_TSK_REGISTER(osc_cfg_task,PRIORITY_4,1000);
/* Private includes ----------------------------------------------------------*/
const osc_time_def osc_tim[] = 
{
	{
		.str = "50ns ",
		.osc_time = 50, /* 50 ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 15, /* default */
	},
	{
		.str = "150ns",
		.osc_time = 150, /* 150 ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 5,
	},	
	{
		.str = "250ns",
		.osc_time = 250, /* 250 ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 3,
	},
	{
		.str = "500ns",
		.osc_time = 500 , /*  base on 750ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 2,
		.osc_ins = 0x30, /* transfer to 500ns for 1us */
	},
#if 1	
	{
		.str = "750ns",
		.osc_time = 750 , /* 750 ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 1,
	},
#endif
	{
		.str = "1.0us",
		.osc_time = 1000 , /*  base on 750ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 1,
		.osc_ins = 0x30, /* transfer to 1us for 750ns */
	},
	{
		.str = "2.5us",
		.osc_time = 2.5f , /* 2.5us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},
	{
		.str = "5us  ",
		.osc_time = 5 , /* 5us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "10us ",
		.osc_time = 10 , /* 10us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},
	{
		.str = "25us ",
		.osc_time = 25 , /* 10us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},
	{
		.str = "50us ",
		.osc_time = 50 , /* 50us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},
	{
		.str = "100us",
		.osc_time = 100 , /* 100us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "250us",
		.osc_time = 250 , /* 250us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},
	{
		.str = "500us",
		.osc_time = 500 , /* 500us */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_US,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "1ms  ",
		.osc_time = 1 , /* 1ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_ins = 10, /* 100ms */
	},
	{
		.str = "2.5ms",
		.osc_time = 2.5 , /* 2.5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_ins = 5, /* 125ms */
	},	
	{
		.str = "5ms  ",
		.osc_time = 5 , /* 5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_ins = 5, /* 250ms */
	},	
	{
		.str = "10ms ",
		.osc_time = 10 , /* 10ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_ins = 3, /* 300ms */
	},	
	{
		.str = "25ms ",
		.osc_time = 25 , /* 25ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_ins = 2, /* 500ms */
	},	
	{
		.str = "50ms ",
		.osc_time = 50 , /* 50ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
		.osc_ins = 2, /* 1000ms */
	},	
	#if 1
	{
		.str = "100ms",
		.osc_time = 100 , /* 100ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "250ms",
		.osc_time = 250 , /* 250ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},
	{
		.str = "500ms",
		.osc_time = 500 , /* 500ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},
  {
		.str = "1.0s  ",
		.osc_time = 1 , /* 1ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},
	{
		.str = "2.5s  ",
		.osc_time = 2.5 , /* 2.5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "5s   ",
		.osc_time = 5 , /* 5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "10s  ",
		.osc_time = 10 , /* 10ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "25s ",
		.osc_time = 25 , /* 25ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "50s  ",
		.osc_time = 50 , /* 50ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_S,
		.osc_zoom_factor = 1,
	},		
	#endif
};
/* voltage gain */
osc_vol_scale_def osc_vol_offset_scale_ch1[] = 
{
	/* 0 */
	{
		.str = "5mV  ",
		.mv_int = 5,
		.gain_dac[0] = 500,
		.gain_dac[1] = 500,
		.gain_offset_ch[0] = -150,
		.gain_offset_ch[1] = -150,
		.gain_sel = 1,
	},
	/* 1 */
	{
		.str = "10mV ",
		.mv_int = 10,
		.gain_dac[0] = 920,
		.gain_dac[1] = 920,
		.gain_offset_ch[0] = -72,
		.gain_offset_ch[1] = -77,
		.gain_sel = 1,
	},
	/* 2 */
	{
		.str = "20mV ",
		.mv_int = 20,
		.gain_dac[0] = 1300,
		.gain_dac[1] = 1300,
		.gain_offset_ch[0] = -30,
		.gain_offset_ch[1] = -40,
		.gain_sel = 1,
	},
	/* 3 */
	{
		.str = "50mV ",
		.mv_int = 50,
		.gain_dac[0] = 1850,
		.gain_dac[1] = 1850,
		.gain_offset_ch[0] = -10,
		.gain_offset_ch[1] = -20,
		.gain_sel = 1,
	},
	/* 4 */
	{
		.str = "100mV",
		.mv_int = 100,
		.gain_dac[0] = 2270,
		.gain_dac[1] = 2270,
		.gain_offset_ch[0] = 0,
		.gain_offset_ch[1] = -13,
		.gain_sel = 1,
	},
	/* 5 */
	{
		.str = "200mV",
		.mv_int = 200,
		.gain_dac[0] = 2720,
		.gain_dac[1] = 2720,
		.gain_offset_ch[0] = 8,
		.gain_offset_ch[1] = -5,
		.gain_sel = 1,
	},
	/* 6 */
	{
		.str = "500mV",
		.mv_int = 500,
		.gain_dac[0] = 1460,
		.gain_dac[1] = 1460,
		.gain_offset_ch[0] = -28,
		.gain_offset_ch[1] = -38,
		.gain_sel = 0,
	},
	/* 7 */
	{
		.str = "1V   ",
		.mv_int = 1000,
		.gain_dac[0] = 1860,
		.gain_dac[1] = 1860,
		.gain_offset_ch[0] = -10,
		.gain_offset_ch[1] = -22,
		.gain_sel = 0,
	},
	/* 8 */
	{
		.str = "2V   ",
		.mv_int = 2000,
		.gain_dac[0] = 2300,
		.gain_dac[1] = 2300,
		.gain_offset_ch[0] = 0,
		.gain_offset_ch[1] = -12,
		.gain_sel = 0,
	},
	/* 9 */
	{
		.str = "5V   ",
		.mv_int = 5000,
		.gain_dac[0] = 2850,
		.gain_dac[1] = 2300,
		.gain_offset_ch[0] = 5,
		.gain_offset_ch[1] = -10,
		.gain_sel = 0,
	},
	/* 10 */
	{
		.str = "10V  ",
		.mv_int = 10000,
		.gain_dac[0] = 4095,
		.gain_dac[1] = 4095,
		.gain_offset_ch[0] = 5,
		.gain_offset_ch[1] = -10,
		.gain_sel = 0,
	},
};
/* */
static void osc_cfg_task(void)
{
	/* create */
	if( trig_lines_hold_time_s == 0 )
	{
		/* hide two lines */
		osc_ui_trig_lines_show(0,0);
		osc_ui_trig_lines_show(1,0);
		/* end */
	}
	else
	{
		trig_lines_hold_time_s --;
	}
}
/* set scan clock */
const osc_time_def * osc_scan_time(unsigned int index)
{
	/* over */
	if( index >= sizeof(osc_tim) / sizeof(osc_tim[0]) )
	{
		/* return */
		return 0;
	}
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* psc */
	unsigned int psc = 0;
	/* get the */
	if( osc_tim[index].osc_unit == OSC_UNIT_US )
	{
		/* calbrate the psc num */
		psc = (unsigned int)((float)OSC_BASE_CLOCK * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UNIT_MS )
	{
		psc = (unsigned int)((float)OSC_BASE_CLOCK * 1000 * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UINT_NS )
	{
		/* return */
    return &osc_tim[index];		
	}
	else if( osc_tim[index].osc_unit == OSC_UNIT_S )
	{
		psc = (unsigned int)((float)OSC_BASE_CLOCK * 1000000 * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else
	{
		/* unit = s , can not supply now */
		return 0;
	}
	/* set psc */
	hal_tim_psc(psc);
	/* return OK */
	return &osc_tim[index];
}
/* get scan_time leng */
static int osc_time_scan_leng(void)
{
	return sizeof(osc_tim) / sizeof(osc_tim[0]);
}
/* get */
static int osc_vol_scale_leng(void)
{
	return sizeof(osc_vol_offset_scale_ch1) / sizeof(osc_vol_offset_scale_ch1[0]);
} 
/* get */
const osc_vol_scale_def * osc_get_vol_scale(unsigned char chn)
{
	/* get run msg for osc */
	osc_run_msg_def * runmsg = get_run_msg();
	/* index */
	unsigned char index = runmsg->vol_scale_ch[chn];
	/* return OK */
	return &osc_vol_offset_scale_ch1[index];
}
/* get osc vol scale */
osc_vol_scale_def * osc_vol_scale_s(unsigned char index)
{
	return &osc_vol_offset_scale_ch1[index];
}
/* osc_cfg_thread */
const osc_time_def * osc_scan_thread(void)
{
	/* defines */
	static signed short last_scan_time = 0xff;
	static unsigned char ste = 0;	
	/* get rotation data */
	signed short osc_rot = osc_rot_sta(OSC_TIME_ROT);
	/* get leng */
	int osc_ts_leng = osc_time_scan_leng();
	/* check */
	if( osc_rot >= osc_ts_leng )
	{
		/* over the max */
		osc_rot = osc_ts_leng - 1;
		/* set max */
		osc_rot_set(OSC_TIME_ROT,osc_rot);
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("??????????????????????");
		}
	}
	else if( osc_rot < 0 )
	{
		/* min */
		osc_rot = 0;
		/* set min */
		osc_rot_set(OSC_TIME_ROT,osc_rot);	
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("??????????????????????");
		}
	}
	else
	{
		if( osc_rot != 0 && osc_rot != ( osc_ts_leng - 1) )
		{
			/* once flag */
			if( ste == 0 )
			{
				ste = 1;
				osc_ui_tips_show(0);//hide
			}
		}
	}
	/* scan time */
	const osc_time_def * osc_time_sw = osc_scan_time(osc_rot);
	/* set time text */
	if( osc_rot != last_scan_time )
	{
		osc_ui_time_str(osc_time_sw->str);
	}
  /* clear flags */
	last_scan_time = osc_rot;
	/* return OK */
	return osc_time_sw;
}
/* voltage thread */
void osc_offset_scale_thread(unsigned char chn)
{
	/* last */
	static signed short lsat_vol = 0xfff;
	static unsigned char last_chn = 0xff;
  /* void offset thread */
	signed short vol_offset_scale = osc_rot_sta(OSC_VOL_OFFSET_SCALE);
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* little */
	unsigned short max_scale = area->total_pixel_v;
	/* get data */
	if( vol_offset_scale > max_scale )
	{
		/* set max offset */
		vol_offset_scale = max_scale;
		/* set vol_offset_scale */
		osc_rot_set(OSC_VOL_OFFSET_SCALE,vol_offset_scale);
	}
	else if( vol_offset_scale < 6 )
	{
		/* set max offset */
		vol_offset_scale = 6;
		/* set vol_offset_scale */
		osc_rot_set(OSC_VOL_OFFSET_SCALE,vol_offset_scale);
	}
	else
	{
		/* to do nothing */
	}
	/* change */
	if( lsat_vol != vol_offset_scale || last_chn != chn )
	{
		/* pos */
		unsigned short new_pos = vol_offset_scale + area->start_pos_y - 6 ;
		/* channel 1 */
		osc_ui_move_offset_arrow(chn,new_pos );
		/* calbrate the offset voltage */
		short out_dac_mv = (float)( vol_offset_scale - max_scale / 2 ) * 512.0f / (float)(max_scale / 2);
		/* calbrate the trig_dac_part_offset */
		trig_dac_part_offset = out_dac_mv;//unit is mv
		/* out dac for test */
		osc_dac_offset(chn,out_dac_mv);
		/* update dac trig */
		osc_set_dac(trig_dac_part_offset + trig_dac_part_rot);
		/* show */
    osc_vertical_offset_tips(chn,out_dac_mv);
	}
	/* ipdate */
	lsat_vol = vol_offset_scale;
	last_chn = chn;
}
/* show vertical vol */
static void osc_vertical_offset_tips(unsigned char chn,signed short mv)
{
	/* test */
	static char buf[32];
	/* get rotation data */
	signed short osc_vs = osc_rot_sta(OSC_VOL_SCALE);
	/* transfer to vol */
	float fmv = (float)mv / (-128.0f) * (float)osc_vol_offset_scale_ch1[osc_vs].mv_int;
	/* change unit */
	if( fmv >= 1000 || fmv <= -1000 )
	{
		/* create data */
		sprintf(buf,"????%d ???????? % 4.2f%s",chn+1,fmv / 1000,"V     ");
	}
	else
	{
		/* create data */
		sprintf(buf,"????%d ???????? % 4.2f%s",chn+1,fmv,"mV    ");
	}
	/* show */
	osc_ui_tips_str_dir(buf);	
}
/* voltage thread */
void osc_trig_scale_thread(unsigned char chn)
{
	/* last */
	static signed short lsat_vol = 0xfff;
	static unsigned char last_chn = 0xff;
  /* void offset thread */
	signed short vol_trig_scale = osc_rot_sta(OSC_TRIG_SCALE);
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* little */
	unsigned short max_scale = area->total_pixel_v;
	/* get data */
	if( vol_trig_scale > max_scale )
	{
		/* set max offset */
		vol_trig_scale = max_scale;
		/* set vol_trig_scale */
		osc_rot_set(OSC_TRIG_SCALE,vol_trig_scale);
	}
	else if( vol_trig_scale < 1 )
	{
		/* set max offset */
		vol_trig_scale = 1;
		/* set vol_trig_scale */
		osc_rot_set(OSC_TRIG_SCALE,vol_trig_scale);
	}
	else
	{
		/* to do nothing */
	}
	/* change */
	if( lsat_vol != vol_trig_scale || last_chn != chn )
	{
		/* pos */
		unsigned short new_pos = vol_trig_scale  + area->start_pos_y - 6 ;
		/* channel 1 */
		osc_ui_move_trig_arrow(chn,new_pos );
		/* show lines */
		osc_ui_trig_lines_show(chn,1);
		/* show hide */
		if( chn == 0 )
		{
		  osc_ui_trig_lines_show(1,0);
		}
		else
		{
			osc_ui_trig_lines_show(0,0);
		}
		/* set lines hold time */
		trig_lines_hold_time_s = 3;
		/* mode trig lines */
		osc_ui_move_trig_lines(chn,new_pos + 6);
		/* calbrate the offset voltage */
		signed  short out_dac = (float)( max_scale / 2 - vol_trig_scale ) * 512.0f / (float)(max_scale / 2);
		/* calbrate trig_dac_part_rot */
		trig_dac_part_rot = out_dac;
		/* calbrate the offset voltage */
		osc_set_dac(trig_dac_part_offset + trig_dac_part_rot);
		/* out dac for test */
	}
	/* ipdate */
	lsat_vol = vol_trig_scale;
	last_chn = chn;
}
/* vol scale thread */
const osc_vol_scale_def * osc_vol_scale_thread(unsigned char chn)
{
	/* defines */
	static signed short last_vol_scale = 0xff;
	static unsigned char ste = 0;	
	static unsigned char last_chn = 0xff;
	/* get rotation data */
	signed short osc_vs_t = osc_rot_sta(OSC_VOL_SCALE);
	/* get leng */
	int osc_ts_leng = osc_vol_scale_leng();
	/* check */
	if( osc_vs_t >= osc_ts_leng )
	{
		/* over the max */
		osc_vs_t = osc_ts_leng - 1;
		/* set max */
		osc_rot_set(OSC_VOL_SCALE,osc_vs_t);
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("????????????????????");
		}
	}
	else if( osc_vs_t < 0 )
	{
		/* min */
		osc_vs_t = 0;
		/* set min */
		osc_rot_set(OSC_VOL_SCALE,osc_vs_t);	
		/* once flags */
		if( ste == 1 )
		{
			ste = 0;
			osc_ui_tips_str("????????????????????");
		}
	}
	else
	{
		if( osc_vs_t != 0 && osc_vs_t != ( osc_ts_leng - 1) )
		{
			/* once flag */
			if( ste == 0 )
			{
				ste = 1;
				osc_ui_tips_show(0);//hide
			}
		}
	}
	/* set time text */
	if( osc_vs_t != last_vol_scale || last_chn != chn )
	{
		/* set string */
		osc_ui_vol_scale(chn,osc_vol_offset_scale_ch1[osc_vs_t].str);
		/* set gain ctrl */
		osc_gain_ctrl(chn,osc_vol_offset_scale_ch1[osc_vs_t].gain_sel);
		/* set dac */
		osc_vol_dac(chn,osc_vol_offset_scale_ch1[osc_vs_t].gain_dac[chn],osc_vol_offset_scale_ch1[osc_vs_t].gain_offset_ch[chn]);
		/* end */
	}
  /* clear flags */
	last_vol_scale = osc_vs_t;
	last_chn = chn;
	/* return OK */
	return &osc_vol_offset_scale_ch1[osc_vs_t];
}































































