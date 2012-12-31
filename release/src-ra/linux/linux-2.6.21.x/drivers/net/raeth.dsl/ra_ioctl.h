#ifndef _RAETH_IOCTL_H
#define _RAETH_IOCTL_H

/* ioctl commands */
#define RAETH_ESW_REG_READ		0x89F1
#define RAETH_ESW_REG_WRITE		0x89F2
#define RAETH_MII_READ			0x89F3
#define RAETH_MII_WRITE			0x89F4
#define RAETH_ESW_INGRESS_RATE		0x89F5
#define RAETH_ESW_EGRESS_RATE		0x89F6

/* for tc resp */
#define RAETH_GET_TC_RESP 0x89F7
/* for internet led control */
#define RAETH_GET_OUT_PKT_CNT 0x89F8
/* for bridge mode */
#define RAETH_START_BRIDGE_MODE 0x89F9
/* for ethernet WAN mode */
#define RAETH_START_ETHERNET_WAN_MODE 0x89FA
/* for DSL WAN mode */
#define RAETH_START_DSL_WAN_MODE 0x89FB


/* rt3052 embedded ethernet switch registers */
#define REG_ESW_VLAN_ID_BASE		0x50
#define REG_ESW_VLAN_MEMB_BASE		0x70
#define REG_ESW_TABLE_SEARCH		0x24
#define REG_ESW_TABLE_STATUS0		0x28
#define REG_ESW_TABLE_STATUS1		0x2C
#define REG_ESW_TABLE_STATUS2		0x30
#define REG_ESW_WT_MAC_AD0		0x34
#define REG_ESW_WT_MAC_AD1		0x38
#define REG_ESW_WT_MAC_AD2		0x3C

#if defined(CONFIG_RALINK_RT3352)
#define REG_ESW_MAX			0x14C
#else //RT305x, RT3350
#define REG_ESW_MAX			0xFC
#endif


typedef struct rt3052_esw_reg {
	unsigned int off;
	unsigned int val;
} esw_reg;

typedef struct ralink_mii_ioctl_data {
	__u32	phy_id;
	__u32	reg_num;
	__u32	val_in;
	__u32	val_out;
} ra_mii_ioctl_data;

typedef struct rt335x_esw_reg {
	unsigned int on_off;
	unsigned int port;
	unsigned int bw;/*Mbps*/
} esw_rate;


#endif
