/* drivers/input/touchscreen/gt9xx.h
 *
 * Copyright (c) 2013-2015, The Linux Foundation. All rights reserved.
 *
 * Linux Foundation chooses to take subject only to the GPLv2 license
 * terms, and distributes only under these terms.
 *
 * 2010 - 2013 Goodix Technology.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be a reference
 * to you, when you are integrating the GOODiX's CTP IC into your system,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 */

#ifndef _GOODIX_GT9XX_H_
#define _GOODIX_GT9XX_H_

#include <linux/kernel.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/uaccess.h>

#if defined(CONFIG_FB)
#include <linux/notifier.h>
#include <linux/fb.h>
#elif defined(CONFIG_HAS_EARLYSUSPEND)
#include <linux/earlysuspend.h>
#define GOODIX_SUSPEND_LEVEL 1
#endif

#define MAX_BUTTONS 4
#define GOODIX_MAX_CFG_GROUP	6
#define GTP_FW_NAME_MAXSIZE	50

#define GTP_REG_VERSION       0x8140

#define GTP_DRIVER_SEND_CFG   1
#define GTP_COMPATIBLE_MODE   0

//*************************** PART2:TODO define **********************************
// STEP_1(REQUIRED): Define Configuration Information Group(s)
// Sensor_ID Map:
/* sensor_opt1 sensor_opt2 Sensor_ID
    GND         GND         0 
    VDDIO       GND         1 
    NC          GND         2 
    GND         NC/300K     3 
    VDDIO       NC/300K     4 
    NC          NC/300K     5 
*/
// TODO: define your own default or for Sensor_ID == 0 config here. 
// The predefined one is just a sample config, which is not suitable for your tp in most cases.
#define CTP_CFG_GROUP1 {0x44,0xB0,0x04,0x80,0x07,0x0A,0x35,0x00,0x01,0x08,0x28,0x09,0x55,0x3C,0x03,0x00,0x00,0x00,0xFF,0x67,0x00,0x00,0x06,0x16,0x17,0x1C,0x14,0x90,0x30,0xAA,0x13,0x15,0x12,0x0C,0x00,0x00,0x00,0x9B,0x33,0x11,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x2B,0x12,0x3C,0x4A,0xC5,0x02,0x06,0x00,0x00,0x04,0x7A,0x14,0x00,0x61,0x1A,0x00,0x50,0x21,0x00,0x42,0x2A,0x00,0x38,0x35,0x00,0x38,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x00,0x19,0x0E,0x46,0x00,0x00,0x00,0x08,0x00,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x04,0x05,0x06,0x07,0x08,0x09,0x0C,0x0D,0x0E,0x0F,0x10,0x11,0x14,0x15,0x16,0x17,0x18,0x19,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0D,0x0C,0x0A,0x08,0x07,0x06,0x04,0x02,0x00,0x0F,0x10,0x11,0x12,0x13,0x14,0x19,0x1B,0x1C,0x1E,0x1F,0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x91,0x01}

    
// TODO: define your config for Sensor_ID == 1 here, if needed
#define CTP_CFG_GROUP2 {\
    }

// TODO: define your config for Sensor_ID == 2 here, if needed
#define CTP_CFG_GROUP3 {\
    }

// TODO: define your config for Sensor_ID == 3 here, if needed
#define CTP_CFG_GROUP4 {\
}
// TODO: define your config for Sensor_ID == 4 here, if needed
/*[DEBUG] modify by TCTNB tianhongwei 05/30 2014 ; update tp config param with version 0x42*/
/*[DEBUG] modify by TCTNB tianhongwei 09/12 2014 ; update tp config param with version V73*/
#define CTP_CFG_GROUP5 {\
   }

// TODO: define your config for Sensor_ID == 5 here, if needed
#define CTP_CFG_GROUP6 {\
    }

struct goodix_ts_platform_data {
	int irq_gpio;
	u32 irq_gpio_flags;
	int reset_gpio;
	u32 reset_gpio_flags;
	const char *product_id;
	const char *fw_name;
	u32 x_max;
	u32 y_max;
	u32 x_min;
	u32 y_min;
	u32 panel_minx;
	u32 panel_miny;
	u32 panel_maxx;
	u32 panel_maxy;
	bool force_update;
	bool i2c_pull_up;
	bool enable_power_off;
	size_t config_data_len[GOODIX_MAX_CFG_GROUP];
	u8 *config_data[GOODIX_MAX_CFG_GROUP];
	u32 button_map[MAX_BUTTONS];
	u8 num_button;
	bool have_touch_key;
	bool driver_send_cfg;
	bool change_x2y;
	bool with_pen;
	bool slide_wakeup;
	bool dbl_clk_wakeup;
};
struct goodix_ts_data {
	spinlock_t irq_lock;
	struct i2c_client *client;
	struct input_dev  *input_dev;
	struct goodix_ts_platform_data *pdata;
	struct hrtimer timer;
	struct workqueue_struct *goodix_wq;
	struct work_struct	work;
	char fw_name[GTP_FW_NAME_MAXSIZE];
	struct delayed_work goodix_update_work;
	s32 irq_is_disabled;
	s32 use_irq;
	u16 abs_x_max;
	u16 abs_y_max;
	u16 addr;
	u8  max_touch_num;
	u8  int_trigger_type;
	u8  green_wake_mode;
	u8  chip_type;
	u8 *config_data;
	u8  enter_update;
	u8  gtp_is_suspend;
	u8  gtp_rawdiff_mode;
	u8  gtp_cfg_len;
	u8  fixed_cfg;
	u8  esd_running;
	u8  fw_error;
	u8  pnl_init_error;
	bool power_on;
	struct mutex lock;
	bool fw_loading;
	bool force_update;
	struct regulator *avdd;
	struct regulator *vdd;
	struct regulator *vcc_i2c;
#if defined(CONFIG_FB)
	struct notifier_block fb_notif;
#elif defined(CONFIG_HAS_EARLYSUSPEND)
	struct early_suspend early_suspend;
#endif
	struct dentry *debug_base;
	u16 ic_vid;//[FEATURE]-Modify by TCTNB.TianHongwei 2014/5/19 show fw version for app.
	u8 ic_pid[5];//[FEATURE]-Modify by TCTNB.TianHongwei 2014/5/19 show fw version for app.
};

extern u16 show_len;
extern u16 total_len;

/***************************PART1:ON/OFF define*******************************/
#define GTP_CUSTOM_CFG			0
#define GTP_ESD_PROTECT			0

#define GTP_IRQ_TAB            {\
				IRQ_TYPE_EDGE_RISING,\
				IRQ_TYPE_EDGE_FALLING,\
				IRQ_TYPE_LEVEL_LOW,\
				IRQ_TYPE_LEVEL_HIGH\
				}


#define GTP_IRQ_TAB_RISING	0
#define GTP_IRQ_TAB_FALLING	1
#if GTP_CUSTOM_CFG
#define GTP_MAX_HEIGHT		1920
#define GTP_MAX_WIDTH		1200
#define GTP_INT_TRIGGER		GTP_IRQ_TAB_RISING
#else
#define GTP_MAX_HEIGHT		1920
#define GTP_MAX_WIDTH		1200
#define GTP_INT_TRIGGER		GTP_IRQ_TAB_FALLING
#endif
#define GTP_PRODUCT_ID_MAXSIZE	5
#define GTP_PRODUCT_ID_BUFFER_MAXSIZE	6
#define GTP_FW_VERSION_BUFFER_MAXSIZE	4
#define GTP_MAX_TOUCH		5
#define GTP_ESD_CHECK_CIRCLE	2000      /* jiffy: ms */

/***************************PART3:OTHER define*********************************/
#define GTP_DRIVER_VERSION	"V1.8.1<2013/09/01>"
#define GTP_I2C_NAME		"Goodix-TS"
#define GTP_POLL_TIME		10     /* jiffy: ms*/
#define GTP_ADDR_LENGTH		2
#define GTP_CONFIG_MIN_LENGTH	186
#define GTP_CONFIG_MAX_LENGTH	240
#define FAIL			0
#define SUCCESS			1
#define SWITCH_OFF		0
#define SWITCH_ON		1

/* Registers define */
#define GTP_READ_COOR_ADDR	0x814E
#define GTP_REG_SLEEP		0x8040
#define GTP_REG_SENSOR_ID	0x814A
#define GTP_REG_CONFIG_DATA	0x8047
#define GTP_REG_FW_VERSION	0x8144
#define GTP_REG_PRODUCT_ID	0x8140

#define GTP_I2C_RETRY_3		3
#define GTP_I2C_RETRY_5		5
#define GTP_I2C_RETRY_10	10

#define RESOLUTION_LOC		3
#define TRIGGER_LOC		8

/* HIGH: 0x28/0x29, LOW: 0xBA/0xBB */
#define GTP_I2C_ADDRESS_HIGH	0x14
#define GTP_I2C_ADDRESS_LOW	0x5D
#define GTP_VALID_ADDR_START	0x8040
#define GTP_VALID_ADDR_END	0x8177

#define CFG_GROUP_LEN(p_cfg_grp) (sizeof(p_cfg_grp) / sizeof(p_cfg_grp[0]))

/* GTP CM_HEAD RW flags */
#define GTP_RW_READ			0
#define GTP_RW_WRITE			1
#define GTP_RW_READ_IC_TYPE		2
#define GTP_RW_WRITE_IC_TYPE		3
#define GTP_RW_FILL_INFO		4
#define GTP_RW_NO_WRITE			5
#define GTP_RW_READ_ERROR		6
#define GTP_RW_DISABLE_IRQ		7
#define GTP_RW_READ_VERSION		8
#define GTP_RW_ENABLE_IRQ		9
#define GTP_RW_ENTER_UPDATE_MODE	11
#define GTP_RW_LEAVE_UPDATE_MODE	13
#define GTP_RW_UPDATE_FW		15
#define GTP_RW_CHECK_RAWDIFF_MODE	17

/* GTP need flag or interrupt */
#define GTP_NO_NEED			0
#define GTP_NEED_FLAG			1
#define GTP_NEED_INTERRUPT		2

/*****************************End of Part III********************************/
#define usleep(x) usleep_range(x,x+500)
void gtp_esd_switch(struct i2c_client *client, int on);

int gtp_i2c_read_dbl_check(struct i2c_client *client, u16 addr,
					u8 *rxbuf, int len);
int gtp_send_cfg(struct i2c_client *client);
void gtp_reset_guitar(struct goodix_ts_data *ts, int ms);
void gtp_irq_disable(struct goodix_ts_data *ts);
void gtp_irq_enable(struct goodix_ts_data *ts);

#ifdef CONFIG_GT9XX_TOUCHPANEL_DEBUG
s32 init_wr_node(struct i2c_client *client);
void uninit_wr_node(void);
#endif

u8 gup_init_update_proc(struct goodix_ts_data *ts);
s32 gup_enter_update_mode(struct i2c_client *client);
void gup_leave_update_mode(struct i2c_client *client);
s32 gup_update_proc(void *dir);
extern struct i2c_client  *i2c_connect_client;
#endif /* _GOODIX_GT9XX_H_ */
