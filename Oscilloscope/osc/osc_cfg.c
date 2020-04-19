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
/* out dac part */
static unsigned short trig_dac_part_offset = 0;
static unsigned short trig_dac_part_rot = 0;
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
		.str = "750ns",
		.osc_time = 750 , /* 750 ns */
		.osc_clock_ex = 1,
		.osc_unit = OSC_UINT_NS,
		.osc_zoom_factor = 1,
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
	},
	{
		.str = "2.5ms",
		.osc_time = 2.5 , /* 2.5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "5ms  ",
		.osc_time = 5 , /* 5ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "10ms ",
		.osc_time = 10 , /* 10ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "25ms ",
		.osc_time = 25 , /* 25ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},	
	{
		.str = "50ms ",
		.osc_time = 50 , /* 50ms */
		.osc_clock_ex = 0,
		.osc_unit = OSC_UNIT_MS,
		.osc_zoom_factor = 1,
	},	
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
};
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
	unsigned short psc = 0;
	/* get the */
	if( osc_tim[index].osc_unit == OSC_UNIT_US )
	{
		/* calbrate the psc num */
		psc = (unsigned short)((float)OSC_BASE_CLOCK * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UNIT_MS )
	{
		psc = (unsigned short)((float)OSC_BASE_CLOCK * 1000 * osc_tim[index].osc_time / (float)area->pixel_horizontal) / 2;
	}
	else if( osc_tim[index].osc_unit == OSC_UINT_NS )
	{
		/* return */
    return &osc_tim[index];		
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
			osc_ui_tips_str("扫描时间已经到达最大值");
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
			osc_ui_tips_str("扫描时间已经到达最小值");
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
				osc_ui_tips_str("                       ");
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
void osc_vol_scale_thread(unsigned char chn)
{
	/* last */
	static signed short lsat_vol = 0xfff;
  /* void offset thread */
	signed short vol_scale = osc_rot_sta(OSC_VOL_SCALE);
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* little */
	unsigned short max_scale = area->num_vertical * area->little_grid;
	/* get data */
	if( vol_scale > max_scale )
	{
		/* set max offset */
		vol_scale = max_scale;
		/* set vol_scale */
		osc_rot_set(OSC_VOL_SCALE,vol_scale);
	}
	else if( vol_scale < 1 )
	{
		/* set max offset */
		vol_scale = 1;
		/* set vol_scale */
		osc_rot_set(OSC_VOL_SCALE,vol_scale);
	}
	else
	{
		/* to do nothing */
	}
	/* change */
	if( lsat_vol != vol_scale )
	{
		/* pos */
		unsigned short new_pos = vol_scale * area->pixel_vertiacl / area->little_grid  + area->start_pos_y - 6 ;
		/* channel 1 */
		osc_ui_move_offset_arrow(chn,new_pos );
		/* */
		if( vol_scale >= max_scale / 2 )
		{
			/* calbrate the offset voltage */
			unsigned short out_dac = ( vol_scale -  max_scale / 2 ) * 51 + 20;
			/* calbrate the trig_dac_part_offset */
			trig_dac_part_offset = out_dac / 2;//unit is mv
			/* out dac for test */
			osc_voltage_output(1870,2000,0,out_dac);
			/* update dac trig */
			osc_set_dac(trig_dac_part_offset + trig_dac_part_rot);
		}
	}
	/* ipdate */
	lsat_vol = vol_scale;
}
/* voltage thread */
void osc_trig_scale_thread(unsigned char chn)
{
	/* last */
	static signed short lsat_vol = 0xfff;
  /* void offset thread */
	signed short vol_scale = osc_rot_sta(OSC_TRIG_SCALE);
	/* get draw area */
	draw_area_def * area = get_draw_area_msg();
	/* little */
	unsigned short max_scale = area->total_pixel_v;
	/* get data */
	if( vol_scale > max_scale )
	{
		/* set max offset */
		vol_scale = max_scale;
		/* set vol_scale */
		osc_rot_set(OSC_TRIG_SCALE,vol_scale);
	}
	else if( vol_scale < 1 )
	{
		/* set max offset */
		vol_scale = 1;
		/* set vol_scale */
		osc_rot_set(OSC_TRIG_SCALE,vol_scale);
	}
	else
	{
		/* to do nothing */
	}
	/* change */
	if( lsat_vol != vol_scale )
	{
		/* pos */
		unsigned short new_pos = vol_scale  + area->start_pos_y - 6 ;
		/* channel 1 */
		osc_ui_move_trig_arrow(chn,new_pos );
		/* calbrate the offset voltage */
		signed  short out_dac = (float)( max_scale / 2 - vol_scale ) * 512.0f / (float)(max_scale / 2);
		/* only supply pos vol now */
		if( vol_scale <= max_scale / 2 )
		{
			/* calbrate trig_dac_part_rot */
			trig_dac_part_rot = out_dac;
			/* calbrate the offset voltage */
			osc_set_dac(trig_dac_part_offset + trig_dac_part_rot);
			/* out dac for test */
		}
	}
	/* ipdate */
	lsat_vol = vol_scale;
}


































































