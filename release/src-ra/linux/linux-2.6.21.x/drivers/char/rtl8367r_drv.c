#include <linux/init.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/config.h>
#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

#include "ralink_gpio.h"
#include <smi.h>
#include <myrtk.h>
#include <rtk_api.h>
#include <rtk_api_ext.h>

#define	PHY_CONTROL_REG			0
#define	CONTROL_REG_PORT_POWER_BIT	0x800

#ifdef  CONFIG_DEVFS_FS
#include <linux/devfs_fs_kernel.h>
static  devfs_handle_t devfs_handle;
#endif

#include <linux/delay.h>	// msleep()

#define SR3			1

#define NAME			"rtl8367r"
#define RTL8367R_DEVNAME	"rtkswitch"
int rtl8367r_major = 206;

MODULE_DESCRIPTION("Realtek RTL8367R-VB support");
MODULE_AUTHOR("ASUS");
MODULE_LICENSE("GPL");

//#define DELAY			10000
//#define DELAY			166
//#define CLK_DURATION(clk)	{ int i; for(i=0; i<clk; i++); }
#define DELAY			10
#define CLK_DURATION(clk) udelay(clk)
#define _SMI_ACK_RESPONSE(ok)	{ /*if (!(ok)) return RT_ERR_FAILED; */}

typedef int gpioID;

#define GPIO_DIR_OUT	1
#define GPIO_DIR_IN	0

#define GPIO_PERI_GPIO 0
#define GPIO_INT_DISABLE 0


gpioID smi_SCK = 23;	/* GPIO used for SMI Clock Generation */
gpioID smi_SDA = 22;	/* GPIO used for SMI Data signal */

#define ack_timer	5
#define max_register	0x018A

#define CONTROL_REG_PORT_POWER_BIT	0x800
#define LAN_PORT_1			1
#define LAN_PORT_2			2
#define LAN_PORT_3			3
#define LAN_PORT_4			4

// those setting are for 3662/3883
#define RALINK_PIO_BASE			0xB0000600
#define RT3662_GPIOMODE_REG (0xB0000000+0x60)
#define PIODIR_R  (RALINK_PIO_BASE + 0X24)
#define PIODATA_R (RALINK_PIO_BASE + 0X20)

void _rtl865x_initGpioPin(int gpioIdx, int dummyParam0, int gpioDir, int dummyParam1)
{
	unsigned long bit_one = 1;	

//	printk("init %d %d\n",gpioIdx,gpioDir);

	unsigned long value;
	if (GPIO_DIR_OUT == gpioDir)
	{
		value = le32_to_cpu(*(volatile u_long *)PIODIR_R);
		value |= (bit_one << gpioIdx);	
		*(volatile u_long *)(PIODIR_R) = cpu_to_le32(value);			
	}
	else
	{
		value = le32_to_cpu(*(volatile u_long *)PIODIR_R);
		value &= ~(bit_one << gpioIdx);	
		*(volatile u_long *)(PIODIR_R) = cpu_to_le32(value);				
	}
}

void _rtl865x_setGpioDataBit(int gpioIdx, unsigned int val)
{
	unsigned long value;
	unsigned long bit_one = 1;	

	if (val & 1)
	{
		value = le32_to_cpu(*(volatile u_long *)PIODATA_R);
		value |= (bit_one << gpioIdx);
		*(volatile u_long *)(PIODATA_R) = cpu_to_le32(value);	
	}
	else
	{
		value = le32_to_cpu(*(volatile u_long *)PIODATA_R);
		value &= ~(bit_one << gpioIdx);
		*(volatile u_long *)(PIODATA_R) = cpu_to_le32(value);		
	}
}

void _rtl865x_getGpioDataBit(int gpioIdx, unsigned int* val)
{	
	unsigned long value;
	unsigned long bit_one = 1;	

	value = le32_to_cpu(*(volatile u_long *)PIODATA_R);

	if (value & (bit_one << gpioIdx))
	{
		*val = 1;
	}
	else
	{
		*val = 0;
	}
}


void _smi_start(void)
{
    /* change GPIO pin to Output only */
    _rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);
    _rtl865x_initGpioPin(smi_SCK, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);

    /* Initial state: SCK: 0, SDA: 1 */
    _rtl865x_setGpioDataBit(smi_SCK, 0);
    _rtl865x_setGpioDataBit(smi_SDA, 1);
    CLK_DURATION(DELAY);

    /* CLK 1: 0 -> 1, 1 -> 0 */
    _rtl865x_setGpioDataBit(smi_SCK, 1);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 0);
    CLK_DURATION(DELAY);

    /* CLK 2: */
    _rtl865x_setGpioDataBit(smi_SCK, 1);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SDA, 0);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 0);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SDA, 1);

}



void _smi_writeBit(uint16 signal, uint32 bitLen)
{
    for( ; bitLen > 0; bitLen--)
    {
        CLK_DURATION(DELAY);

        /* prepare data */
        if ( signal & (1<<(bitLen-1)) )
            _rtl865x_setGpioDataBit(smi_SDA, 1);
        else
            _rtl865x_setGpioDataBit(smi_SDA, 0);
        CLK_DURATION(DELAY);

        /* clocking */
        _rtl865x_setGpioDataBit(smi_SCK, 1);
        CLK_DURATION(DELAY);
        _rtl865x_setGpioDataBit(smi_SCK, 0);
    }

}



void _smi_readBit(uint32 bitLen, uint32 *rData) 
{
    rtk_uint32 u;

    /* change GPIO pin to Input only */
    _rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);

    for (*rData = 0; bitLen > 0; bitLen--)
    {
        CLK_DURATION(DELAY);

        /* clocking */
        _rtl865x_setGpioDataBit(smi_SCK, 1);
        CLK_DURATION(DELAY);
        _rtl865x_getGpioDataBit(smi_SDA, &u);
        _rtl865x_setGpioDataBit(smi_SCK, 0);

        *rData |= (u << (bitLen - 1));
    }

    /* change GPIO pin to Output only */
    _rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_OUT, GPIO_INT_DISABLE);

}



void _smi_stop(void)
{

    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SDA, 0);
    _rtl865x_setGpioDataBit(smi_SCK, 1);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SDA, 1);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 1);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 0);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 1);

    /* add a click */
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 0);
    CLK_DURATION(DELAY);
    _rtl865x_setGpioDataBit(smi_SCK, 1);


    /* change GPIO pin to Output only */
    _rtl865x_initGpioPin(smi_SDA, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);
    _rtl865x_initGpioPin(smi_SCK, GPIO_PERI_GPIO, GPIO_DIR_IN, GPIO_INT_DISABLE);


}



int32 smi_read(uint32 mAddrs, uint32 *rData)
{
    uint32 rawData=0, ACK;
    uint8  con;
    uint32 ret = RT_ERR_OK;
/*
    if ((mAddrs > max_register) || (rData == NULL))  return    RT_ERR_FAILED;
*/

    /*Disable CPU interrupt to ensure that the SMI operation is atomic. 
      The API is based on RTL865X, rewrite the API if porting to other platform.*/
//	Jiahao++
//       rtlglue_drvMutexLock();

    _smi_start();                                /* Start SMI */

    _smi_writeBit(0x0b, 4);                     /* CTRL code: 4'b1011 for RTL8370 */

    _smi_writeBit(0x4, 3);                        /* CTRL code: 3'b100 */

    _smi_writeBit(0x1, 1);                        /* 1: issue READ command */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for issuing READ command*/
    } while ((ACK != 0) && (con < ack_timer));

    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs&0xff), 8);             /* Set reg_addr[7:0] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for setting reg_addr[7:0] */    
    } while ((ACK != 0) && (con < ack_timer));

    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs>>8), 8);                 /* Set reg_addr[15:8] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK by RTL8369 */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_readBit(8, &rawData);                    /* Read DATA [7:0] */
    *rData = rawData&0xff; 

    _smi_writeBit(0x00, 1);                        /* ACK by CPU */

    _smi_readBit(8, &rawData);                    /* Read DATA [15: 8] */

    _smi_writeBit(0x01, 1);                        /* ACK by CPU */
    *rData |= (rawData<<8);

    _smi_stop();
//	Jiahao++
//    rtlglue_drvMutexUnlock();/*enable CPU interrupt*/

    return ret;
}



int32 smi_write(uint32 mAddrs, uint32 rData)
{
/*
    if ((mAddrs > 0x018A) || (rData > 0xFFFF))  return    RT_ERR_FAILED;
*/
    int8 con;
    uint32 ACK;
    uint32 ret = RT_ERR_OK;    

    /*Disable CPU interrupt to ensure that the SMI operation is atomic. 
      The API is based on RTL865X, rewrite the API if porting to other platform.*/
//	Jiahao++
//       rtlglue_drvMutexLock();

    _smi_start();                                /* Start SMI */

    _smi_writeBit(0x0b, 4);                     /* CTRL code: 4'b1011 for RTL8370*/

    _smi_writeBit(0x4, 3);                        /* CTRL code: 3'b100 */

    _smi_writeBit(0x0, 1);                        /* 0: issue WRITE command */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for issuing WRITE command*/
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs&0xff), 8);             /* Set reg_addr[7:0] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for setting reg_addr[7:0] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit((mAddrs>>8), 8);                 /* Set reg_addr[15:8] */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for setting reg_addr[15:8] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit(rData&0xff, 8);                /* Write Data [7:0] out */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                    /* ACK for writting data [7:0] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_writeBit(rData>>8, 8);                    /* Write Data [15:8] out */

    con = 0;
    do {
        con++;
        _smi_readBit(1, &ACK);                        /* ACK for writting data [15:8] */
    } while ((ACK != 0) && (con < ack_timer));
    if (ACK != 0) ret = RT_ERR_FAILED;

    _smi_stop();    
//	Jiahao++
//    rtlglue_drvMutexUnlock();/*enable CPU interrupt*/
    
    return ret;
}

int rt_gpio_ioctl(unsigned int req, int idx, unsigned long arg)
{
	unsigned long tmp;
/*
	unsigned long PIODATA0;
	unsigned long PIODATA1;
	unsigned long PIODATA2;
*/
	req &= RALINK_GPIO_DATA_MASK;

	switch(req) {
	case RALINK_GPIO_READ_BIT:
/*
		printk("RALINK_REG_PIODATA: %x\n",  RALINK_REG_PIODATA);
		printk("RALINK_REG_PIO3924DATA: %x\n",  RALINK_REG_PIO3924DATA);
		printk("RALINK_REG_PIO5140DATA: %x\n",  RALINK_REG_PIO5140DATA);
		PIODATA0 = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
		PIODATA1 = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DATA));
		PIODATA2 = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DATA));
		printk("*RALINK_REG_PIODATA: %x\n",	PIODATA0);
		printk("*RALINK_REG_PIO3924DATA: %x\n",	PIODATA1);
		printk("*RALINK_REG_PIO5140DATA: %x\n",	PIODATA2);
*/

		if (idx <= 23)
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
		else if (idx <= 39)
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DATA));
		else
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DATA));

		if (idx <= 23)
			tmp = (tmp >> idx) & 1L;
		else if (idx <= 39)
			tmp = (tmp >> (idx-24)) & 1L;
		else
			tmp = (tmp >> (idx-40)) & 1L;
		return tmp;

		break;
	case RALINK_GPIO_WRITE_BIT:
		if (idx <= 23) {
			tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
			if (arg & 1L)
				tmp |= (1L << idx);
			else
				tmp &= ~(1L << idx);
			*(volatile u32 *)(RALINK_REG_PIODATA)= cpu_to_le32(tmp);
		}
		else if (idx <= 39) {
			tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DATA));
			if (arg & 1L)
				tmp |= (1L << (idx-24));
			else
				tmp &= ~(1L << (idx-24));
			*(volatile u32 *)(RALINK_REG_PIO3924DATA)= cpu_to_le32(tmp);
		}
		else {
			tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DATA));
			if (arg & 1L)
				tmp |= (1L << (idx-40));
			else
				tmp &= ~(1L << (idx-40));
			*(volatile u32 *)(RALINK_REG_PIO5140DATA)= cpu_to_le32(tmp);
		}

		break;
	default:
		return -1;
	}
	return 0;
}

int
ralink_gpio_write_bit(int idx, int value)
{
	unsigned int req;
	value &= 1;
       
	if (0L <= idx && idx < RALINK_GPIO_NUMBER)
		req = RALINK_GPIO_WRITE_BIT;
	else
		return -1;

	return rt_gpio_ioctl(req, idx, value);
}

int
ralink_gpio_read_bit(int idx, int *value)
{
	unsigned int req;
	*value = 0;

	if (0L <= idx && idx < RALINK_GPIO_NUMBER)
		req = RALINK_GPIO_READ_BIT;
	else
		return -1;

	if ((*value = rt_gpio_ioctl(req, idx, value)) < 0)
		return -1;

	return *value;
}

int
ralink_initGpioPin(unsigned int idx, int dir)
{
	unsigned long tmp;

	if (idx < 0 || RALINK_GPIO_NUMBER <= idx)
		return -1;

	if (dir == GPIO_DIR_OUT)
	{
		if (idx <= 23) {
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
			tmp |= (1L << idx);
		}
		else if (idx <= 39) {
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DIR));
			tmp |= (1L << (idx-24));
		}
		else {
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DIR));
			tmp |= (1L << (idx-40));
		}
	}
	else if (dir == GPIO_DIR_IN)
	{		
		if (idx <= 23) {
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
			tmp &= ~(1L << idx);
		}
		else if (idx <= 39) {
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DIR));
			tmp &= ~(1L << (idx-24));
		}
		else {
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DIR));
			tmp &= ~(1L << (idx-40));
		}
	}
	else
		return -1;

	if (idx <= 23) {
		*(volatile u32 *)(RALINK_REG_PIODIR) = cpu_to_le32(tmp);
	}
	else if (idx <= 39) {
		*(volatile u32 *)(RALINK_REG_PIO3924DIR) = cpu_to_le32(tmp);
	}
	else {
		*(volatile u32 *)(RALINK_REG_PIO5140DIR) = cpu_to_le32(tmp);
	}

	return 0;
}

#define PIODIR3924_R  (RALINK_PIO_BASE + 0x4c)
#define PIODATA3924_R (RALINK_PIO_BASE + 0x48)

void enable_adsl_bridge(void)
{
	rtk_api_ret_t retVal;

	// enable adsl-lan bridge
	
	// reset_phy_gpio_realtek()
	{
		unsigned long value;
	
		printk("\n GPIO pin 24 reset to switch\n");
	
		//Set Gpio pin 24 to output
		value = le32_to_cpu(*(volatile u_long *)PIODIR3924_R);
		value |= 1;
		*(volatile u_long *)(PIODIR3924_R) = cpu_to_le32(value);
	
		//Set Gpio pin 24 to low
		value = le32_to_cpu(*(volatile u_long *)PIODATA3924_R);
		value &= ~1;
		*(volatile u_long *)(PIODATA3924_R) = cpu_to_le32(value);
	
		udelay(50000);
		
		//Set Gpio pin 24 to high
		value = le32_to_cpu(*(volatile u_long *)PIODATA3924_R);
		value |= 1;
		*(volatile u_long *)(PIODATA3924_R) = cpu_to_le32(value);
	
		udelay(50000);
	
		//Set Gpio pin 24 to low
		value = le32_to_cpu(*(volatile u_long *)PIODATA3924_R);
		value &= ~1;
		*(volatile u_long *)(PIODATA3924_R) = cpu_to_le32(value);
	
		udelay(50000);
		
		//Set Gpio pin 24 to high
		value = le32_to_cpu(*(volatile u_long *)PIODATA3924_R);
		value |= 1;
		*(volatile u_long *)(PIODATA3924_R) = cpu_to_le32(value);
		
		udelay(50000);
	
	}
	
	// after reset, switch need to delay 1 ms
	// if not, SMI may send out unknown data
	udelay(1000);
	
	
	// test_smi_signal_and_wait()
	{
			int i;
			for (i=0; i<200; i++)
			{
					rtk_uint32 data;
					rtk_api_ret_t retVal;
	
				if((retVal = rtl8367b_getAsicReg(0x1202, &data)) != RT_ERR_OK)
				{
							printk("error = %d\n", retVal);
					}
					printk("data = %x\n", data);
					if (data == 0x88a8) break;
	
				CLK_DURATION(50000);
			}
	}
	
	retVal = rtk_switch_init();
	printk("rtk_switch_init(): return %d\n", retVal);
	if (retVal != RT_ERR_OK) return retVal;
	
	
	rtk_cpu_enable_set(ENABLE);
	rtk_cpu_tagPort_set(RTK_EXT_1_MAC,CPU_INSERT_TO_NONE);
}

rtk_uint32 port_mask(rtk_uint32 port, rtk_uint32 zero)
{
	switch (port) {
	case 0: return zero ? 1 << 0 : 0;
	case 1: return 1 << 1;
	case 2: return 1 << 2;
	case 3: return 1 << 3;
	case 4: return 1 << 4;
	case 5: return port_mask(1, 0) | port_mask(2, 0);
	case 6: return port_mask(3, 0) | port_mask(4, 0);
	}

	return 0;
}

rtk_api_ret_t configure_dslwan_vlan(
	rtk_uint32 svid,
	rtk_uint32 portmask,
	rtk_uint32 untagmask,
	rtk_uint32 outmask)
{
	rtk_api_ret_t retVal = RT_ERR_OK;
	rtk_svlan_memberCfg_t svlan_cfg;
	rtk_portmask_t out_mask;
	rtk_uint32 port;

	printk("%s(%d, %02x, %02x, %02x)\n", __FUNCTION__, svid, portmask, untagmask, outmask);


	/* Configure system SVLAN member content */
	memset(&svlan_cfg, 0, sizeof(rtk_svlan_memberCfg_t));
	svlan_cfg.svid = svid;
	svlan_cfg.memberport = 0x20 | portmask;
	svlan_cfg.untagport = svlan_cfg.memberport & untagmask;
	svlan_cfg.efiden = 1;
	svlan_cfg.efid = svid;
	if ((retVal = rtk_svlan_memberPortEntry_set(svid, &svlan_cfg)) != RT_ERR_OK)
	{
		printk("error set svid %d port membership\n", svid);
		return retVal;
	}

	/* Process 0-4 ports only */
	for (port = 0; (port < 5) && (portmask >> port); port++)
	{
		if (((portmask >> port) & 1) == 0)
			continue;

		/* Configure default egress SVLAN */
		if ((retVal = rtk_svlan_defaultSvlan_set(port, svid)) != RT_ERR_OK)
		{
			printk("error set port %d default svlan %d\n", port, svid);
			break;
		}

		/* Configure ports isolation */
		out_mask.bits[0] = 0x20 | outmask;
		if ((retVal = rtk_port_isolation_set(port, out_mask))!= RT_ERR_OK) {
			printk("error set port %d isolation mask %x\n", port, out_mask.bits[0]);
			break;
		}

		/* Configure port EFID */
		if ((retVal = rtk_port_efid_set(port, svid)) != RT_ERR_OK) {
			printk("error set port %d efid %d\n", port, svid);
			break;
		}
	}

	return retVal;
}

void configure_dslwan_switch(int wan_port_num, int stb_port_num)
{
	rtk_uint32 dsl_portmask = port_mask(0, 1);
	rtk_uint32 stb_portmask = port_mask(stb_port_num, 0);
	rtk_uint32 eth_portmask = port_mask(wan_port_num, 0);
	rtk_uint32 lan_portmask = 0x1f & ~(dsl_portmask | eth_portmask | stb_portmask);

	/* DSL WAN default SVID 1 */
	configure_dslwan_vlan(1, dsl_portmask, dsl_portmask, dsl_portmask);

	/* LAN default SVID 2 */
	configure_dslwan_vlan(2, lan_portmask, lan_portmask, lan_portmask);

	/* IPTV STB default SVID 3 */
	configure_dslwan_vlan(3, stb_portmask, stb_portmask, stb_portmask | eth_portmask);

	/* Ethernet WAN default SVID 4 */
	configure_dslwan_vlan(4, eth_portmask, eth_portmask, stb_portmask | eth_portmask);
}

void LANWANPartition(int wan_stb_x, int eth_wan_x)
{
	rtk_api_ret_t retVal;

	if (wan_stb_x == 100)
	{
		enable_adsl_bridge();
		return;
	}

	printk("set switch ic to SVLAN\n");
	if ((retVal = rtk_svlan_init()) != RT_ERR_OK)
	{
		printk("error 0\n");
	}
 
	/* Change TPID from 0x88a8 to 0x8100 */
	if ((retVal = rtk_svlan_tpidEntry_set(0x8100)) != RT_ERR_OK)
	{
		printk("error 1\n");
	}

	// Add MAC5 to Uplink Port, can be changed
	if ((retVal = rtk_svlan_servicePort_add(5)) != RT_ERR_OK)
	{
		printk("error 2\n");
	}

	configure_dslwan_switch(eth_wan_x, wan_stb_x);
}

#if 0

rtk_api_ret_t rtk_port_linkStatus_get(rtk_port_t port, rtk_port_linkStatus_t *pLinkStatus)
{

    rtk_api_ret_t retVal;
    uint32 phyData;

    if (port > RTK_PORT_ID_MAX)
        return RT_ERR_PORT_ID; 

    if ((retVal = rtl8370_setAsicPHYReg(port,RTL8370_PHY_PAGE_ADDRESS,0))!=RT_ERR_OK)
        return retVal;  

    /*Get PHY status register*/
    if (RT_ERR_OK != rtl8370_getAsicPHYReg(port,PHY_STATUS_REG,&phyData))
        return RT_ERR_FAILED;

    /*check link status*/
    if (phyData & (1<<2))
    {
        *pLinkStatus = 1;
    }
    else
    {
        *pLinkStatus = 0;
    }

    return RT_ERR_OK;
}

#endif

typedef struct {
	unsigned int idx;
	unsigned int value;
} asus_gpio_info;

typedef struct {
	unsigned int link[5];
	unsigned int speed[5];
} phyState;



#if 0

void ShowRtkCounter(rtk_port_t port, rtk_stat_port_type_t mibType, char* mibTypeName)
{
	rtk_api_ret_t retVal;
	rtk_stat_counter_t cntrs;	
	retVal = rtk_stat_port_get(port, mibType, &cntrs);
	if (retVal == RT_ERR_OK)
	{
		printk(mibTypeName);
		printk(":%d\n",cntrs);
	}
	else
	{
		printk("error ShowRtkCounter\n");
	}
}


// copy from SDK
typedef enum RTL8367B_MIBCOUNTER_E{

    /* RX */
	ifInOctets = 0,

	dot3StatsFCSErrors,
	dot3StatsSymbolErrors,
	dot3InPauseFrames,
	dot3ControlInUnknownOpcodes,	
	
	etherStatsFragments,
	etherStatsJabbers,
	ifInUcastPkts,
	etherStatsDropEvents,

    ifInMulticastPkts,
    ifInBroadcastPkts,
    inMldChecksumError,
    inIgmpChecksumError,
    inMldSpecificQuery,
    inMldGeneralQuery,
    inIgmpSpecificQuery,
    inIgmpGeneralQuery,
    inMldLeaves,
    inIgmpLeaves,

    /* TX/RX */
	etherStatsOctets,

	etherStatsUnderSizePkts,
	etherOversizeStats,
	etherStatsPkts64Octets,
	etherStatsPkts65to127Octets,
	etherStatsPkts128to255Octets,
	etherStatsPkts256to511Octets,
	etherStatsPkts512to1023Octets,
	etherStatsPkts1024to1518Octets,

    /* TX */
	ifOutOctets,

	dot3StatsSingleCollisionFrames,
	dot3StatMultipleCollisionFrames,
	dot3sDeferredTransmissions,
	dot3StatsLateCollisions,
	etherStatsCollisions,
	dot3StatsExcessiveCollisions,
	dot3OutPauseFrames,
    ifOutDiscards,

    /* ALE */
	dot1dTpPortInDiscards,
	ifOutUcastPkts,
	ifOutMulticastPkts,
	ifOutBroadcastPkts,
	outOampduPkts,
	inOampduPkts,

    inIgmpJoinsSuccess,
    inIgmpJoinsFail,
    inMldJoinsSuccess,
    inMldJoinsFail,
    inReportSuppressionDrop,
    inLeaveSuppressionDrop,
    outIgmpReports,
    outIgmpLeaves,
    outIgmpGeneralQuery,
    outIgmpSpecificQuery,
    outMldReports,
    outMldLeaves,
    outMldGeneralQuery,
    outMldSpecificQuery,
    inKnownMulticastPkts,

	/*Device only */	
	dot1dTpLearnedEntryDiscards,
	RTL8367B_MIBS_NUMBER,
	
}RTL8367B_MIBCOUNTER;	

#endif

unsigned int txDelay_user = 1;
unsigned int rxDelay_user = 2;


char* APIMIBString[61];

void set_mib_str_table()
{
	APIMIBString[0] = "ifInOctets";
	APIMIBString[1] = "dot3StatsFCSErrors";
	APIMIBString[2] = "dot3StatsSymbolErrors";
	APIMIBString[3] = "dot3InPauseFrames";
	APIMIBString[4] = "dot3ControlInUnknownOpcodes";
	APIMIBString[5] = "etherStatsFragments";
	APIMIBString[6] = "etherStatsJabbers";
	APIMIBString[7] = "ifInUcastPkts";
	APIMIBString[8] = "etherStatsDropEvents";
	APIMIBString[9] = "etherStatsOctets";
	APIMIBString[10] = "etherStatsUndersizePkts";
	APIMIBString[11] = "etherStatsOversizePkts";
	APIMIBString[12] = "etherStatsPkts64Octets";
	APIMIBString[13] = "etherStatsPkts65to127Octets";
	APIMIBString[14] = "etherStatsPkts128to255Octets";
	APIMIBString[15] = "etherStatsPkts256to511Octets";
	APIMIBString[16] = "etherStatsPkts512to1023Octets";
	APIMIBString[17] = "etherStatsPkts1024toMaxOctets";
	APIMIBString[18] = "etherStatsMcastPkts";
	APIMIBString[19] = "etherStatsBcastPkts";
	APIMIBString[20] = "ifOutOctets";
	APIMIBString[21] = "dot3StatsSingleCollisionFrames";
	APIMIBString[22] = "dot3StatsMultipleCollisionFrames";
	APIMIBString[23] = "dot3StatsDeferredTransmissions";
	APIMIBString[24] = "dot3StatsLateCollisions";
	APIMIBString[25] = "etherStatsCollisions";
	APIMIBString[26] = "dot3StatsExcessiveCollisions";
	APIMIBString[27] = "dot3OutPauseFrames";
	APIMIBString[28] = "dot1dBasePortDelayExceededDiscards";
	APIMIBString[29] = "dot1dTpPortInDiscards";
	APIMIBString[30] = "ifOutUcastPkts";
	APIMIBString[31] = "ifOutMulticastPkts";
	APIMIBString[32] = "ifOutBrocastPkts";
	APIMIBString[33] = "outOampduPkts";
	APIMIBString[34] = "inOampduPkts";
	APIMIBString[35] = "pktgenPkts";
	APIMIBString[36] = "inMldChecksumError";
	APIMIBString[37] = "inIgmpChecksumError";
	APIMIBString[38] = "inMldSpecificQuery";
	APIMIBString[39] = "inMldGeneralQuery";
	APIMIBString[40] = "inIgmpSpecificQuery";
	APIMIBString[41] = "inIgmpGeneralQuery";
	APIMIBString[42] = "inMldLeaves";
	APIMIBString[43] = "inIgmpLeaves";
	APIMIBString[44] = "inIgmpJoinsSuccess";
	APIMIBString[45] = "inIgmpJoinsFail";
	APIMIBString[46] = "inMldJoinsSuccess";
	APIMIBString[47] = "inMldJoinsFail";
	APIMIBString[48] = "inReportSuppressionDrop";
	APIMIBString[49] = "inLeaveSuppressionDrop";
	APIMIBString[50] = "outIgmpReports";
	APIMIBString[51] = "outIgmpLeaves";
	APIMIBString[52] = "outIgmpGeneralQuery";
	APIMIBString[53] = "outIgmpSpecificQuery";
	APIMIBString[54] = "outMldReports";
	APIMIBString[55] = "outMldLeaves";
	APIMIBString[56] = "outMldGeneralQuery";
	APIMIBString[57] = "outMldSpecificQuery";
	APIMIBString[58] = "inKnownMulticastPkts";
	APIMIBString[59] = "ifInMulticastPkts";
	APIMIBString[60] = "ifInBroadcastPkts";
}

void show_mib_counter(rtk_port_t port, rtk_stat_port_cntr_t* portCntrsPtr)
{	
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[0],0,portCntrsPtr->ifInOctets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[1],1,portCntrsPtr->dot3StatsFCSErrors);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[2],2,portCntrsPtr->dot3StatsSymbolErrors);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[3],3,portCntrsPtr->dot3InPauseFrames);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[4],4,portCntrsPtr->dot3ControlInUnknownOpcodes);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[5],5,portCntrsPtr->etherStatsFragments);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[6],6,portCntrsPtr->etherStatsJabbers);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[7],7,portCntrsPtr->ifInUcastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[8],8,portCntrsPtr->etherStatsDropEvents);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[9],9,portCntrsPtr->etherStatsOctets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[10],10,portCntrsPtr->etherStatsUndersizePkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[11],11,portCntrsPtr->etherStatsOversizePkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[12],12,portCntrsPtr->etherStatsPkts64Octets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[13],13,portCntrsPtr->etherStatsPkts65to127Octets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[14],14,portCntrsPtr->etherStatsPkts128to255Octets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[15],15,portCntrsPtr->etherStatsPkts256to511Octets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[16],16,portCntrsPtr->etherStatsPkts512to1023Octets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[17],17,portCntrsPtr->etherStatsPkts1024toMaxOctets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[18],18,portCntrsPtr->etherStatsMcastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[19],19,portCntrsPtr->etherStatsBcastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[20],20,portCntrsPtr->ifOutOctets);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[21],21,portCntrsPtr->dot3StatsSingleCollisionFrames);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[22],22,portCntrsPtr->dot3StatsMultipleCollisionFrames);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[23],23,portCntrsPtr->dot3StatsDeferredTransmissions);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[24],24,portCntrsPtr->dot3StatsLateCollisions);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[25],25,portCntrsPtr->etherStatsCollisions);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[26],26,portCntrsPtr->dot3StatsExcessiveCollisions);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[27],27,portCntrsPtr->dot3OutPauseFrames);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[28],28,portCntrsPtr->dot1dBasePortDelayExceededDiscards);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[29],29,portCntrsPtr->dot1dTpPortInDiscards);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[30],30,portCntrsPtr->ifOutUcastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[31],31,portCntrsPtr->ifOutMulticastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[32],32,portCntrsPtr->ifOutBrocastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[33],33,portCntrsPtr->outOampduPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[34],34,portCntrsPtr->inOampduPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[35],35,portCntrsPtr->pktgenPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[36],36,portCntrsPtr->inMldChecksumError);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[37],37,portCntrsPtr->inIgmpChecksumError);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[38],38,portCntrsPtr->inMldSpecificQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[39],39,portCntrsPtr->inMldGeneralQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[40],40,portCntrsPtr->inIgmpSpecificQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[41],41,portCntrsPtr->inIgmpGeneralQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[42],42,portCntrsPtr->inMldLeaves);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[43],43,portCntrsPtr->inIgmpLeaves);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[44],44,portCntrsPtr->inIgmpJoinsSuccess);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[45],45,portCntrsPtr->inIgmpJoinsFail);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[46],46,portCntrsPtr->inMldJoinsSuccess);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[47],47,portCntrsPtr->inMldJoinsFail);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[48],48,portCntrsPtr->inReportSuppressionDrop);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[49],49,portCntrsPtr->inLeaveSuppressionDrop);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[50],50,portCntrsPtr->outIgmpReports);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[51],51,portCntrsPtr->outIgmpLeaves);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[52],52,portCntrsPtr->outIgmpGeneralQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[53],53,portCntrsPtr->outIgmpSpecificQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[54],54,portCntrsPtr->outMldReports);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[55],55,portCntrsPtr->outMldLeaves);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[56],56,portCntrsPtr->outMldGeneralQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[57],57,portCntrsPtr->outMldSpecificQuery);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[58],58,portCntrsPtr->inKnownMulticastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[59],59,portCntrsPtr->ifInMulticastPkts);
	printk("\r\nport %d: %-35s[%02d]:  %25lld",port,APIMIBString[60],60,portCntrsPtr->ifInBroadcastPkts);
	printk("\n");
}


static int g_wan_stb = 0;
static int g_ethernet_wan = 0;

int rtl8367r_ioctl(struct inode *inode, struct file *file, unsigned int req,
		unsigned long arg)
{
	rtk_api_ret_t retVal;
	rtk_port_t port;
	rtk_stat_port_cntr_t portCntrs;
	rtk_port_linkStatus_t pLinkStatus = 0;
	rtk_data_t pSpeed;
	rtk_data_t pDuplex;
	unsigned long tmp;
	asus_gpio_info info;
	phyState pS;
//	unsigned long gpiomode;
	int port_user = 0;
	int port_cfg = 0;	
	int wan_stb_x = 0;
	int ethernet_wan_x = 0;	
	rtk_data_t txDelay_ro, rxDelay_ro;
	unsigned int regData;
	rtk_port_phy_data_t pData;
	unsigned int control_rate;
	unsigned int port_nr, mask, wan_port_mask = port_mask(0, 1);

	wan_port_mask |= port_mask(g_wan_stb, 0) | port_mask(g_ethernet_wan, 0);

/*
	if (req > 5)
	{
		printk("********** rtl8367r_ioctl = %d\n",req);
	}
*/	
	switch(req) {
	case 0:					// check WAN port phy status
		if (g_ethernet_wan == 1)
		{
			port = LAN_PORT_1;
			rtk_port_phyStatus_get(port, &pLinkStatus, &pSpeed, &pDuplex);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (g_ethernet_wan == 2)
		{
			port = LAN_PORT_2;
			rtk_port_phyStatus_get(port, &pLinkStatus, &pSpeed, &pDuplex);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (g_ethernet_wan == 3)
		{
			port = LAN_PORT_3;
			rtk_port_phyStatus_get(port, &pLinkStatus, &pSpeed, &pDuplex);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (g_ethernet_wan == 4)
		{
			port = LAN_PORT_4;
			rtk_port_phyStatus_get(port, &pLinkStatus, &pSpeed, &pDuplex);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}

		pLinkStatus = 0;
		put_user(pLinkStatus, (unsigned int __user *)arg);
		break;
#if 0	
	case 3:					// check LAN ports phy status
		if (wan_stb_g == 0)		// P0,P1,P2,P3
		{
			port = LAN_PORT_4;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_3;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_2;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_1;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (wan_stb_g == 1)	// P0,P1,P2
		{
			port = LAN_PORT_4;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_3;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_2;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (wan_stb_g == 2)	// P0,P1,P3
		{
			port = LAN_PORT_4;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_3;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_1;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (wan_stb_g == 3)	// P0,P2,P3
		{
			port = LAN_PORT_4;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_2;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_1;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
 		else if (wan_stb_g == 4)	// P1,P2,P3
 		{
			port = LAN_PORT_3;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_2;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_1;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (wan_stb_g == 5)	// P2,P3
		{
			port = LAN_PORT_2;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_1;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}
		else if (wan_stb_g == 6)	// P0,P1
		{
			port = LAN_PORT_4;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			if (pLinkStatus)
			{
				put_user(pLinkStatus, (unsigned int __user *)arg);
				break;
			}

			port = LAN_PORT_3;
			rtk_port_linkStatus_get(port, &pLinkStatus);
			put_user(pLinkStatus, (unsigned int __user *)arg);
			break;
		}

		pLinkStatus = 0;
		put_user(pLinkStatus, (unsigned int __user *)arg);
		break;

#endif		

	case 2:			// show state of RTL8367M GMAC1
		set_mib_str_table();
		for (port = 0; port <=5; port++)
		{

			if(RT_ERR_OK != rtk_stat_port_getAll(port, &portCntrs))
				return RT_ERR_FAILED;

			show_mib_counter(port, &portCntrs);
		}

		break;

#if 0		

	case 6:
		copy_from_user(&port_user, (int __user *)arg, sizeof(int));
		port = port_user;
		printk("rtk_stat_port_getAll(%d...)\n", port);
		retVal = rtk_stat_port_getAll(port, &pPort_cntrs);

		if (retVal == RT_ERR_OK)
		{
			printk("ifInOctets: %lld \ndot3StatsFCSErrors: %d \ndot3StatsSymbolErrors: %d \ndot3InPauseFrames: %d \ndot3ControlInUnknownOpcodes: %d \netherStatsFragments: %d \netherStatsJabbers: %d \nifInUcastPkts: %d \netherStatsDropEvents: %d \netherStatsOctets: %lld \netherStatsUndersizePkts: %d \netherStatsOversizePkts: %d \netherStatsPkts64Octets: %d \netherStatsPkts65to127Octets: %d \netherStatsPkts128to255Octets: %d \netherStatsPkts256to511Octets: %d \netherStatsPkts512to1023Octets: %d \netherStatsPkts1024toMaxOctets: %d \netherStatsMcastPkts: %d \netherStatsBcastPkts: %d \nifOutOctets: %lld \ndot3StatsSingleCollisionFrames: %d \ndot3StatsMultipleCollisionFrames: %d \ndot3StatsDeferredTransmissions: %d \ndot3StatsLateCollisions: %d \netherStatsCollisions: %d \ndot3StatsExcessiveCollisions: %d \ndot3OutPauseFrames: %d \ndot1dBasePortDelayExceededDiscards: %d \ndot1dTpPortInDiscards: %d \nifOutUcastPkts: %d \nifOutMulticastPkts: %d \nifOutBrocastPkts: %d \noutOampduPkts: %d \ninOampduPkts: %d \npktgenPkts: %d\n\n",
				pPort_cntrs.ifInOctets,
				pPort_cntrs.dot3StatsFCSErrors,
				pPort_cntrs.dot3StatsSymbolErrors,
				pPort_cntrs.dot3InPauseFrames,
				pPort_cntrs.dot3ControlInUnknownOpcodes,
				pPort_cntrs.etherStatsFragments,
				pPort_cntrs.etherStatsJabbers,
				pPort_cntrs.ifInUcastPkts,
				pPort_cntrs.etherStatsDropEvents,
				pPort_cntrs.etherStatsOctets,
				pPort_cntrs.etherStatsUndersizePkts,
				pPort_cntrs.etherStatsOversizePkts,
				pPort_cntrs.etherStatsPkts64Octets,
				pPort_cntrs.etherStatsPkts65to127Octets,
				pPort_cntrs.etherStatsPkts128to255Octets,
				pPort_cntrs.etherStatsPkts256to511Octets,
				pPort_cntrs.etherStatsPkts512to1023Octets,
				pPort_cntrs.etherStatsPkts1024toMaxOctets,
				pPort_cntrs.etherStatsMcastPkts,
				pPort_cntrs.etherStatsBcastPkts,	
				pPort_cntrs.ifOutOctets,
				pPort_cntrs.dot3StatsSingleCollisionFrames,
				pPort_cntrs.dot3StatsMultipleCollisionFrames,
				pPort_cntrs.dot3StatsDeferredTransmissions,
				pPort_cntrs.dot3StatsLateCollisions,
				pPort_cntrs.etherStatsCollisions,
				pPort_cntrs.dot3StatsExcessiveCollisions,
				pPort_cntrs.dot3OutPauseFrames,
				pPort_cntrs.dot1dBasePortDelayExceededDiscards,
				pPort_cntrs.dot1dTpPortInDiscards,
				pPort_cntrs.ifOutUcastPkts,
				pPort_cntrs.ifOutMulticastPkts,
				pPort_cntrs.ifOutBrocastPkts,
				pPort_cntrs.outOampduPkts,
				pPort_cntrs.inOampduPkts,
				pPort_cntrs.pktgenPkts
			);
		}
		else
			printk("rtk_stat_port_getAll() return %d\n", retVal);
		
		break;

#endif


	case RALINK_GPIO_SET_DIR:	// 0x01
		copy_from_user(&info, (asus_gpio_info *)arg, sizeof(info));
		if (info.idx < 0 || RALINK_GPIO_NUMBER <= info.idx)
			return -1;
/*
		gpiomode = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
		printk("GPIOMODE: %x\n",  gpiomode);
*/
		if (info.value == GPIO_DIR_OUT)
		{
			if (info.idx <= 23) {
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
				tmp |= (1L << info.idx);
			}
			else if (info.idx <= 39) {
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DIR));
				tmp |= (1L << (info.idx-24));
			}
			else {
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DIR));
				tmp |= (1L << (info.idx-40));
			}
		}
		else if (info.value == GPIO_DIR_IN)
		{		
			if (info.idx <= 23) {
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODIR));
				tmp &= ~(1L << info.idx);
			}
			else if (info.idx <= 39) {
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DIR));
				tmp &= ~(1L << (info.idx-24));
			}
			else {
				tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DIR));
				tmp &= ~(1L << (info.idx-40));
			}
		}
		else
			return -1;
	
		if (info.idx <= 23) {
			*(volatile u32 *)(RALINK_REG_PIODIR) = cpu_to_le32(tmp);
		}
		else if (info.idx <= 39) {
			*(volatile u32 *)(RALINK_REG_PIO3924DIR) = cpu_to_le32(tmp);
		}
		else {
			*(volatile u32 *)(RALINK_REG_PIO5140DIR) = cpu_to_le32(tmp);
		}
	
		return 0;

		break;
		

	case RALINK_GPIO_READ_BIT:	// 0x04
//		copy_from_user(&info, (asus_gpio_info *)arg, sizeof(info));
		copy_from_user(&info.idx, (int __user *)arg, sizeof(int));

		if (info.idx <= 23)
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
		else if (info.idx <= 39)
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DATA));
		else
			tmp = le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DATA));

		if (info.idx <= 23)
			tmp = (tmp >> info.idx) & 1L;
		else if (info.idx <= 39)
			tmp = (tmp >> (info.idx-24)) & 1L;
		else
			tmp = (tmp >> (info.idx-40)) & 1L;
//		return tmp;
//		put_user(tmp, (int __user *)&((*(asus_gpio_info *)arg).value));
		put_user(tmp, (int __user *)arg);

		break;

	case RALINK_GPIO_WRITE_BIT:	// 0x05
		copy_from_user(&info, (asus_gpio_info *)arg, sizeof(info));
		if (info.idx <= 23) {
			tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIODATA));
			if (info.value & 1L)
				tmp |= (1L << info.idx);
			else
				tmp &= ~(1L << info.idx);
			*(volatile u32 *)(RALINK_REG_PIODATA)= cpu_to_le32(tmp);
		}
		else if (info.idx <= 39) {
			tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO3924DATA));
			if (info.value & 1L)
				tmp |= (1L << (info.idx-24));
			else
				tmp &= ~(1L << (info.idx-24));
			*(volatile u32 *)(RALINK_REG_PIO3924DATA)= cpu_to_le32(tmp);
		}
		else {
			tmp =le32_to_cpu(*(volatile u32 *)(RALINK_REG_PIO5140DATA));
			if (info.value & 1L)
				tmp |= (1L << (info.idx-40));
			else
				tmp &= ~(1L << (info.idx-40));
			*(volatile u32 *)(RALINK_REG_PIO5140DATA)= cpu_to_le32(tmp);
		}

		break;

	case 8:

		copy_from_user(&port_cfg, (int __user *)arg, sizeof(int));

		wan_stb_x = (port_cfg & 0xff);
		ethernet_wan_x = port_cfg >> 8;

		printk("wan_stb_x=%d\n",wan_stb_x);
		printk("ethernet_wan_x=%d\n",ethernet_wan_x);		

		g_wan_stb = wan_stb_x;
		g_ethernet_wan = ethernet_wan_x;

		if (wan_stb_x == 100)
		{
			printk("ADSL bridge mode , no auto learning\n");		
			LANWANPartition(wan_stb_x, 0);
			break;
		}

		if (ethernet_wan_x == 0)
		{
			if (wan_stb_x == 0)
			{
				printk("LAN: P1,P2,P3,P4 DSLWAN: P0\n");
			}
			else
			{
				printk("IPTV port enabled, %d\n",wan_stb_x);		
			}
			LANWANPartition(wan_stb_x, 0);
		}
		else
		{
			printk("ethernet WAN port enabled, always use LAN port 1\n");

			if (wan_stb_x == 0)
			{
				printk("LAN: P2,P3,P4 DSLWAN: P0 ethernetWAN P1\n");
			}
			else
			{
				printk("Dual WAN mode , IPTV port enabled, %d\n",wan_stb_x);		
			}
			LANWANPartition(wan_stb_x, ethernet_wan_x);			
		}

		break;


	case 9:
		copy_from_user(&txDelay_user, (unsigned int __user *)arg, sizeof(unsigned int));
		printk("txDelay_user: %d\n", txDelay_user);

		printk("new txDelay: %d, rxDelay: %d\n", txDelay_user, rxDelay_user);
		retVal = rtk_port_rgmiiDelayExt_set(1, txDelay_user, rxDelay_user);
		printk("rtk_port_rgmiiDelayExt_set(): return %d\n", retVal);

		rtk_port_rgmiiDelayExt_get(1, &txDelay_ro, &rxDelay_ro);
		printk("current txDelay: %d, rxDelay: %d\n", txDelay_ro, rxDelay_ro);

		break;

	case 10:
		copy_from_user(&rxDelay_user, (unsigned int __user *)arg, sizeof(unsigned int));
		printk("rxDelay_user: %d\n", rxDelay_user);

		printk("new txDelay: %d, rxDelay: %d\n", txDelay_user, rxDelay_user);
		retVal = rtk_port_rgmiiDelayExt_set(1, txDelay_user, rxDelay_user);
		printk("rtk_port_rgmiiDelayExt_set(): return %d\n", retVal);

		rtk_port_rgmiiDelayExt_get(1, &txDelay_ro, &rxDelay_ro);
		printk("current txDelay: %d, rxDelay: %d\n", txDelay_ro, rxDelay_ro);

		break;

#if 0				

	case 11:
		regData = le32_to_cpu(*(volatile u32 *)(RALINK_REG_GPIOMODE));
		printk("GPIOMODE before: %x\n",  regData);
		regData &= ~RALINK_GPIOMODE_JTAG;
		regData |= RALINK_GPIOMODE_JTAG;
		printk("GPIOMODE writing: %x\n", regData);
		*(volatile u32 *)(RALINK_REG_GPIOMODE) = cpu_to_le32(regData);

		break;


	case 13:		// check WAN port phy speed
                port = 4;	// port 4 is WAN port
		retVal = rtk_port_phyStatus_get(port, &pLinkStatus, &pSpeed, &pDuplex);
//		printk("rtk_port_phyStatus_get() : %d\n", pSpeed);
		put_user(pSpeed, (unsigned int __user *)arg);

		break;

#endif

	case 14:		// power up LAN port(s)

		rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);

		break;

	case 15:		// power down LAN port(s)		

		rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);

		break;

	case 16:		// power up all ports
		printk("power up all ports\n");

		rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);

		rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
		pData &= ~CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);

		break;

	case 17:		// power down all ports
		printk("power down all ports\n");
                        
		rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);
                        
		rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
		pData |= CONTROL_REG_PORT_POWER_BIT;
		rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);

		break;


	case 18:		// phy status for ATE command
		copy_from_user(&pS, (phyState __user *)arg, sizeof(pS));

		for (port = 0; port < 5; port++)
		{
                	retVal = rtk_port_phyStatus_get(port, &pLinkStatus, &pSpeed, &pDuplex);
			pS.link[port] = pLinkStatus;
			pS.speed[port] = pSpeed;
		}                

		copy_to_user((phyState __user *)arg, &pS, sizeof(pS));

		break;

#if 0
        case 19:
                copy_from_user(&txDelay_user, (unsigned int __user *)arg, sizeof(unsigned int));
                printk("WAN port txDelay: %d\n", txDelay_user);

                printk("new txDelay: %d, rxDelay: %d\n", txDelay_user, rxDelay_user);
                retVal = rtk_port_rgmiiDelayExt0_set(txDelay_user, rxDelay_user);
                printk("rtk_port_rgmiiDelayExt0_set(): return %d\n", retVal);

                rtk_port_rgmiiDelayExt0_get(&txDelay_ro, &rxDelay_ro);
                printk("current Ext0 txDelay: %d, rxDelay: %d\n", txDelay_ro, rxDelay_ro);

                break;

        case 20:
		copy_from_user(&rxDelay_user, (unsigned int __user *)arg, sizeof(unsigned int));
		printk("WAN port rxDelay: %d\n", rxDelay_user);

		printk("new txDelay: %d, rxDelay: %d\n", txDelay_user, rxDelay_user);
		retVal = rtk_port_rgmiiDelayExt0_set(txDelay_user, rxDelay_user);
		printk("rtk_port_rgmiiDelayExt0_set(): return %d\n", retVal);

		rtk_port_rgmiiDelayExt0_get(&txDelay_ro, &rxDelay_ro);
		printk("current Ext0 txDelay: %d, rxDelay: %d\n", txDelay_ro, rxDelay_ro);

		break;


	case 21:
		printk("reset strom control\n");

		rtk_storm_controlRate_set(0, STORM_GROUP_UNKNOWN_UNICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(0, STORM_GROUP_UNKNOWN_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(0, STORM_GROUP_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(0, STORM_GROUP_BROADCAST, 1048568, 1, 0);
	
		rtk_storm_controlRate_set(1, STORM_GROUP_UNKNOWN_UNICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_UNKNOWN_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_BROADCAST, 1048568, 1, 0);
	
		rtk_storm_controlRate_set(2, STORM_GROUP_UNKNOWN_UNICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_UNKNOWN_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_BROADCAST, 1048568, 1, 0);
	
		rtk_storm_controlRate_set(3, STORM_GROUP_UNKNOWN_UNICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_UNKNOWN_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_BROADCAST, 1048568, 1, 0);
	
		rtk_storm_controlRate_set(4, STORM_GROUP_UNKNOWN_UNICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_UNKNOWN_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_MULTICAST, 1048568, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_BROADCAST, 1048568, 1, 0);

		break;

        case 22:
		copy_from_user(&control_rate, (unsigned int __user *)arg, sizeof(unsigned int));
		if ((control_rate < 1) || (control_rate > 1024))
			break;
		printk("set unknown unicast strom control as: %d\n", control_rate);
		rtk_storm_controlRate_set(0, STORM_GROUP_UNKNOWN_UNICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_UNKNOWN_UNICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_UNKNOWN_UNICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_UNKNOWN_UNICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_UNKNOWN_UNICAST, control_rate*1024, 1, 0);
		
		break;

        case 23:
		copy_from_user(&control_rate, (unsigned int __user *)arg, sizeof(unsigned int));
		if ((control_rate < 1) || (control_rate > 1024))
			break;
		printk("set unknown multicast strom control as: %d\n", control_rate);
		rtk_storm_controlRate_set(0, STORM_GROUP_UNKNOWN_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_UNKNOWN_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_UNKNOWN_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_UNKNOWN_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_UNKNOWN_MULTICAST, control_rate*1024, 1, 0);
		
		break;

        case 24:
		copy_from_user(&control_rate, (unsigned int __user *)arg, sizeof(unsigned int));
		if ((control_rate < 1) || (control_rate > 1024))
			break;
		printk("set multicast strom control as: %d\n", control_rate);
		rtk_storm_controlRate_set(0, STORM_GROUP_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_MULTICAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_MULTICAST, control_rate*1024, 1, 0);
		
		break;

        case 25:
		copy_from_user(&control_rate, (unsigned int __user *)arg, sizeof(unsigned int));
		if ((control_rate < 1) || (control_rate > 1024))
			break;
		printk("set broadcast strom control as: %d\n", control_rate);
		rtk_storm_controlRate_set(0, STORM_GROUP_BROADCAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(1, STORM_GROUP_BROADCAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(2, STORM_GROUP_BROADCAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(3, STORM_GROUP_BROADCAST, control_rate*1024, 1, 0);
		rtk_storm_controlRate_set(4, STORM_GROUP_BROADCAST, control_rate*1024, 1, 0);
		
		break;
#endif	/* #if 0 */

	case 99:
		/* FIXME:
		 * This ioctl is occupied by RT-N65U.
		 * See src-ra-3.0/linux/linux-3.x/drivers/char/rtl8367b/rtl8367rb_drv.c
		 */
		return -ENOIOCTLCMD;
		break;

	case 100:	/* Set SwitchPort LED mode */
		/* FIXME: See rtl8367rb_drv.c */
		return -ENOIOCTLCMD;

	case 109:	/* Set specific ext port txDelay */
		/* FIXME: See rtl8367rb_drv.c */
		return -ENOIOCTLCMD;

	case 110:	/* Set specific ext port txDelay */
		/* FIXME: See rtl8367rb_drv.c */
		return -ENOIOCTLCMD;

	case 114:		// power up WAN port(s)
		for (port_nr = 0, mask = wan_port_mask; mask > 0; mask >>= 1, port_nr++) {
			if (!(mask & 1U))
				continue;

			rtk_port_phyReg_get(port_nr, PHY_CONTROL_REG, &pData);
			pData &= ~CONTROL_REG_PORT_POWER_BIT;
			rtk_port_phyReg_set(port_nr, PHY_CONTROL_REG, pData);
		}
		break;

	case 115:		// power down WAN port(s)
		for (port_nr = 0, mask = wan_port_mask; mask > 0; mask >>= 1, port_nr++) {
			if (!(mask & 1U))
				continue;

			rtk_port_phyReg_get(port_nr, PHY_CONTROL_REG, &pData);
			pData |= CONTROL_REG_PORT_POWER_BIT;
			rtk_port_phyReg_set(port_nr, PHY_CONTROL_REG, pData);
		}
		break;

#if 0
	default:
		return -ENOIOCTLCMD;

#endif		
	}

	return 0;
}

int rtl8367r_open(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
//	MOD_INC_USE_COUNT;
#else
//	try_module_get(THIS_MODULE);
#endif
	return 0;
}

int rtl8367r_release(struct inode *inode, struct file *file)
{
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
//	MOD_DEC_USE_COUNT;
#else
//	module_put(THIS_MODULE);
#endif
	return 0;
}

struct file_operations rtl8367r_fops =
{
	owner:		THIS_MODULE,
	ioctl:		rtl8367r_ioctl,
	open:		rtl8367r_open,
	release:	rtl8367r_release,
};

int __init rtl8367r_init(void)
{
	rtk_port_phy_data_t pData;
	rtk_portmask_t portmask;

#ifdef  CONFIG_DEVFS_FS
	if (devfs_register_chrdev(rtl8367r_major, RTL8367R_DEVNAME,
				&rtl8367r_fops)) {
		printk(KERN_ERR NAME ": unable to register character device\n");
		return -EIO;
	}
	devfs_handle = devfs_register(NULL, RTL8367R_DEVNAME,
			DEVFS_FL_DEFAULT, rtl8367r_major, 0,
			S_IFCHR | S_IRUGO | S_IWUGO, &rtl8367r_fops, NULL);
#else
	int r = 0;
	r = register_chrdev(rtl8367r_major, RTL8367R_DEVNAME,
			&rtl8367r_fops);
	if (r < 0) {
		printk(KERN_ERR NAME ": unable to register character device\n");
		return r;
	}
	if (rtl8367r_major == 0) {
		rtl8367r_major = r;
		printk(KERN_DEBUG NAME ": got dynamic major %d\n", r);
	}
#endif

	printk("rtl8367r_init...\n");
	printk("switch IC = CHIP_RTL8367R_VB, SDK = V1.2.6\n");
	
	portmask.bits[0]=0x1e;
	rtk_led_enable_set(LED_GROUP_0, portmask);	
	rtk_led_groupConfig_set(LED_GROUP_0, LED_CONFIG_LINK_ACT);
	rtk_led_operation_set(LED_OP_PARALLEL);


	rtk_switch_portMaxPktLen_set(LAN_PORT_1, 16000);
	rtk_switch_portMaxPktLen_set(LAN_PORT_2, 16000);
	rtk_switch_portMaxPktLen_set(LAN_PORT_3, 16000);
	rtk_switch_portMaxPktLen_set(LAN_PORT_4, 16000);


// turn on after everything is ready
/*
	rtk_port_phyReg_get(LAN_PORT_4, PHY_CONTROL_REG, &pData);
	pData &= ~CONTROL_REG_PORT_POWER_BIT;
	rtk_port_phyReg_set(LAN_PORT_4, PHY_CONTROL_REG, pData);
	
	rtk_port_phyReg_get(LAN_PORT_3, PHY_CONTROL_REG, &pData);
	pData &= ~CONTROL_REG_PORT_POWER_BIT;
	rtk_port_phyReg_set(LAN_PORT_3, PHY_CONTROL_REG, pData);
	
	rtk_port_phyReg_get(LAN_PORT_2, PHY_CONTROL_REG, &pData);
	pData &= ~CONTROL_REG_PORT_POWER_BIT;
	rtk_port_phyReg_set(LAN_PORT_2, PHY_CONTROL_REG, pData);
	
	rtk_port_phyReg_get(LAN_PORT_1, PHY_CONTROL_REG, &pData);
	pData &= ~CONTROL_REG_PORT_POWER_BIT;
	rtk_port_phyReg_set(LAN_PORT_1, PHY_CONTROL_REG, pData);
*/	


	return 0;
}

void __exit rtl8367r_exit(void)
{
#ifdef  CONFIG_DEVFS_FS
	devfs_unregister_chrdev(rtl8367r_major, RTL8367R_DEVNAME);
	devfs_unregister(devfs_handle);
#else
	unregister_chrdev(rtl8367r_major, RTL8367R_DEVNAME);
#endif

	printk("RTL8367R driver exited\n");
}

module_init(rtl8367r_init);
module_exit(rtl8367r_exit);
