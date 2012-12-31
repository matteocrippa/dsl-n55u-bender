/*
 * NVRAM variable manipulation (Linux kernel half)
 *
 * Copyright 2005, Broadcom Corporation
 * All Rights Reserved.
 * 
 * THIS SOFTWARE IS OFFERED "AS IS", AND BROADCOM GRANTS NO WARRANTIES OF ANY
 * KIND, EXPRESS OR IMPLIED, BY STATUTE, COMMUNICATION OR OTHERWISE. BROADCOM
 * SPECIFICALLY DISCLAIMS ANY IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A SPECIFIC PURPOSE OR NONINFRINGEMENT CONCERNING THIS SOFTWARE.
 *
 * $Id: nvram_linux.c,v 1.1.1.1 2010/12/02 04:36:19 walf_wu Exp $
 */
#define ASUS_NVRAM														//eric++

#include <linux/config.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/interrupt.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/bootmem.h>

#ifdef ASUS_NVRAM
#	include <linux/mm.h>
#	include <linux/version.h>
#	if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,0)
#		define mem_map_reserve(p)		set_bit(PG_reserved, &((p)->flags))
#		define mem_map_unreserve(p)		clear_bit(PG_reserved, &((p)->flags))
#	else
#	include <linux/wrapper.h>
#	endif
#	if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,10)
#	define MOD_INC_USE_COUNT
#	define MOD_DEC_USE_COUNT
#	endif
#include <asm/cacheflush.h>								// function decl. of flush_cache_all()
#include <linux/proc_fs.h>								// create_proc_read_entry()
#else	// !ASUS_NVRAM
#	include <linux/wrapper.h>
#endif	// ASUS_NVRAM

#include <linux/fs.h>
#include <linux/miscdevice.h>
#include <linux/mtd/mtd.h>
#include <linux/nls.h>

#ifdef ASUS_NVRAM
	// Only mips and sh architecture have this file
#else	// !ASUS_NVRAM
#include <asm/addrspace.h>
#endif	// ASUS_NVRAM

#include <asm/io.h>
#include <asm/uaccess.h>

#ifdef ASUS_NVRAM
#include <nvram/typedefs.h>
#include <nvram/bcmendian.h>
#include <nvram/bcmnvram.h>
#include <nvram/bcmutils.h>

#include "nvram.c"

#else	// !ASUS_NVRAM
#include <typedefs.h>
#include <bcmendian.h>
#include <bcmnvram.h>
#include <bcmutils.h>
#include <sbconfig.h>
#include <sbchipc.h>
#include <sbutils.h>
#include <sflash.h>
#endif	// ASUS_NVRAM

/* In BSS to minimize text size and page aligned so it can be mmap()-ed */
#ifdef ASUS_NVRAM
static char *nvram_buf;
#else	// !ASUS_NVRAM
static char nvram_buf[NVRAM_SPACE] __attribute__((aligned(PAGE_SIZE)));
#endif	// ASUS_NVRAM

#ifdef ASUS_NVRAM
static void *sbh;
#define MTD_NVRAM_NAME	"nvram"
#else	// !ASUS_NVRAM
#define CFE_UPDATE 1 // added by Chen-I for mac/regulation update
#endif	// ASUS_NVRAM

#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,18)
// Following macros are removed from linux/mtd/mtd.h since linux-2.6.0
#       define MTD_ERASE(mtd, args...) (*(mtd->erase))(mtd, args)
#       define MTD_POINT(mtd, a,b,c,d) (*(mtd->point))(mtd, a,b,c, (u_char **)(d))
#       define MTD_UNPOINT(mtd, arg) (*(mtd->unpoint))(mtd, (u_char *)arg)
#       define MTD_READ(mtd, args...) (*(mtd->read))(mtd, args)
#       define MTD_WRITE(mtd, args...) (*(mtd->write))(mtd, args)
#       define MTD_READV(mtd, args...) (*(mtd->readv))(mtd, args)
#       define MTD_WRITEV(mtd, args...) (*(mtd->writev))(mtd, args)
#       define MTD_READECC(mtd, args...) (*(mtd->read_ecc))(mtd, args)
#       define MTD_WRITEECC(mtd, args...) (*(mtd->write_ecc))(mtd, args)
#       define MTD_READOOB(mtd, args...) (*(mtd->read_oob))(mtd, args)
#       define MTD_WRITEOOB(mtd, args...) (*(mtd->write_oob))(mtd, args)
#       define MTD_SYNC(mtd) do { if (mtd->sync) (*(mtd->sync))(mtd);  } while (0)
#endif

#define flush_cache_all()                       do { } while (0)

#ifdef CFE_UPDATE
#include <sbextif.h>

extern void bcm947xx_watchdog_disable(void);

#define CFE_SPACE       4*1024
#define CFE_NVRAM_START 0x30000
#define CFE_NVRAM_END   0x30fff
#define CFE_NVRAM_SPACE 192*1024
static struct mtd_info *cfe_mtd = NULL;
static char *CFE_NVRAM_PREFIX="asuscfe";
static char *CFE_NVRAM_COMMIT="asuscfecommit";
static char *CFE_NVRAM_WATCHDOG="asuscfewatchdog";
char *cfe_buf;// = NULL;
struct nvram_header *cfe_nvram_header; // = NULL;
#endif

#ifdef MODULE

#ifdef ASUS_NVRAM
#else	// !ASUS_NVRAM
#define early_nvram_get(name) nvram_get(name)
#endif	// ASUS_NVRAM

#define NLS_XFR 1
#ifdef NLS_XFR
				/* added by Jiahao for WL500gP */
static char *NLS_NVRAM_U2C="asusnlsu2c";
static char *NLS_NVRAM_C2U="asusnlsc2u";
__u16 unibuf[1024];
char codebuf[1024];
char tmpbuf[1024];

void				/* added by Jiahao for WL500gP */
asusnls_u2c(char *name)
{
	char *codepage;
	char *xfrstr;
	struct nls_table *nls;
	int ret, len;
	
	strcpy(codebuf, name);
	codepage=codebuf+strlen(NLS_NVRAM_U2C);
	if((xfrstr=strchr(codepage, '_')))
	{
		*xfrstr=NULL;
		xfrstr++;
		/* debug message, start */
/*
		printk("%s, xfr from utf8 to %s\n", xfrstr, codepage);
		int j;
		printk("utf8:    %d, ", strlen(xfrstr));
		for(j=0;j<strlen(xfrstr);j++)
			printk("%X ", (unsigned char)xfrstr[j]);
		printk("\n");
*/
		/* debug message, end */
		nls=load_nls(codepage);
		if(!nls)
		{
			printk("NLS table is null!!\n");
		} else {
			if (ret=utf8_mbstowcs(unibuf, xfrstr, strlen(xfrstr)))
			{
				int i;
				len = 0;
				for (i = 0; (i < ret) && unibuf[i]; i++) {
					int charlen;
					charlen = nls->uni2char(unibuf[i], &name[len], NLS_MAX_CHARSET_SIZE);
					if (charlen > 0) {
						len += charlen;
					} else {
//						name[len++] = '?';
						strcpy(name, "");
						unload_nls(nls);
						return;
					}
				}
				name[len] = 0;
			}
			unload_nls(nls);
			/* debug message, start */
/*
			int i;
			printk("unicode: %d, ", ret);
			for (i=0;i<ret;i++)
				printk("%X ", unibuf[i]);
			printk("\n");
			printk("local:   %d, ", strlen(name));
			for (i=0;i<strlen(name);i++)
				printk("%X ", (unsigned char)name[i]);
			printk("\n");
			printk("local:   %s\n", name);
*/
			/* debug message, end */
			if(!len)
			{
				printk("can not xfr from utf8 to %s\n", codepage);
				strcpy(name, "");
			}
		}
	}
	else
	{
		strcpy(name, "");
	}
}

void					/* added by Jiahao for WL500gP */
asusnls_c2u(char *name)
{
	char *codepage;
	char *xfrstr;
	struct nls_table *nls;
	int ret;

	strcpy(codebuf, name);
	codepage=codebuf+strlen(NLS_NVRAM_C2U);
	if((xfrstr=strchr(codepage, '_')))
	{
		*xfrstr=NULL;
		xfrstr++;

		/* debug message, start */
/*
		printk("%s, xfr from %s to utf8\n", xfrstr, codepage);
		printk("local:   %d, ", strlen(xfrstr));
		int j;
		for (j=0;j<strlen(xfrstr);j++)
			printk("%X ", (unsigned char)xfrstr[j]);
		printk("\n");
		printk("local:   %s\n", xfrstr);
*/
		/* debug message, end */

		strcpy(name, "");
		nls=load_nls(codepage);
		if(!nls)
		{
			printk("NLS table is null!!\n");
		} else
		{
			int charlen;
			int i;
			int len = strlen(xfrstr);
			for (i = 0; len && *xfrstr; i++, xfrstr += charlen, len -= charlen) {	/* string to unicode */
				charlen = nls->char2uni(xfrstr, len, &unibuf[i]);
				if (charlen < 1) {
//					unibuf[i] = 0x003f;	/* a question mark */
//					charlen = 1;
					strcpy(name ,"");
					unload_nls(nls);
					return;
				}
			}
			unibuf[i] = 0;
			ret=utf8_wcstombs(name, unibuf, 1024);	/* unicode to utf-8, 1024 is size of array unibuf */
			name[ret]=NULL;
			unload_nls(nls);
			/* debug message, start */
/*
			int k;
			printk("unicode: %d, ", i);
			for(k=0;k<i;k++)
				printk("%X ", unibuf[k]);
			printk("\n");
			printk("utf-8:    %s, %d, ", name, strlen(name));
			for (i=0;i<strlen(name);i++)
				printk("%X ", (unsigned char)name[i]);
			printk("\n");
*/
			/* debug message, end */
			if(!ret)
			{
				printk("can not xfr from %s to utf8\n", codepage);
				strcpy(name, "");
			}
		}
	}
	else
	{
		strcpy(name, "");
	}
}

#endif	// NLS_XFR

#else /* !MODULE */

#ifdef ASUS_NVRAM
#error  This nvram driver only support kernel module.
#else   // !ASUS_NVRAM
/* Global SB handle */
extern void *bcm947xx_sbh;
extern spinlock_t bcm947xx_sbh_lock;

/* Convenience */
#define sbh bcm947xx_sbh
#define sbh_lock bcm947xx_sbh_lock
#define KB * 1024
#define MB * 1024 * 1024

/* Probe for NVRAM header */
static void __init
early_nvram_init(void)
{
	struct nvram_header *header;
	chipcregs_t *cc;
	struct sflash *info = NULL;
	int i;
	uint32 base, off, lim;
	u32 *src, *dst;

	if ((cc = sb_setcore(sbh, SB_CC, 0)) != NULL) {
		base = KSEG1ADDR(SB_FLASH2);
		switch (readl(&cc->capabilities) & CAP_FLASH_MASK) {
		case PFLASH:
			lim = SB_FLASH2_SZ;
			break;

		case SFLASH_ST:
		case SFLASH_AT:
			if ((info = sflash_init(cc)) == NULL)
				return;
			lim = info->size;
			break;

		case FLASH_NONE:
		default:
			return;
		}
	} else {
		/* extif assumed, Stop at 4 MB */
		base = KSEG1ADDR(SB_FLASH1);
		lim = SB_FLASH1_SZ;
	}

	off = FLASH_MIN;
	while (off <= lim) {
		/* Windowed flash access */
		header = (struct nvram_header *) KSEG1ADDR(base + off - NVRAM_SPACE);
		if (header->magic == NVRAM_MAGIC)
			goto found;
		off <<= 1;
	}

	/* Try embedded NVRAM at 4 KB and 1 KB as last resorts */
	header = (struct nvram_header *) KSEG1ADDR(base + 4 KB);
	if (header->magic == NVRAM_MAGIC)
		goto found;
	
	header = (struct nvram_header *) KSEG1ADDR(base + 1 KB);
	if (header->magic == NVRAM_MAGIC)
		goto found;
	
	printk("early_nvram_init: NVRAM not found\n");
	return;

found:
	src = (u32 *) header;
	dst = (u32 *) nvram_buf;
	for (i = 0; i < sizeof(struct nvram_header); i += 4)
		*dst++ = *src++;
	for (; i < header->len && i < NVRAM_SPACE; i += 4)
		*dst++ = ltoh32(*src++);
}

/* Early (before mm or mtd) read-only access to NVRAM */
static char * __init
early_nvram_get(const char *name)
{
	char *var, *value, *end, *eq;

	if (!name)
		return NULL;

	/* Too early? */
	if (sbh == NULL)
		return NULL;

	if (!nvram_buf[0])
		early_nvram_init();

	/* Look for name=value and return value */
	var = &nvram_buf[sizeof(struct nvram_header)];
	end = nvram_buf + sizeof(nvram_buf) - 2;
	end[0] = end[1] = '\0';
	for (; *var; var = value + strlen(value) + 1) {
		if (!(eq = strchr(var, '=')))
			break;
		value = eq + 1;
		if ((eq - var) == strlen(name) && strncmp(var, name, (eq - var)) == 0)
			return value;
	}

	return NULL;
}

#endif	// ASUS_NVRAM
#endif /* !MODULE */

extern char * _nvram_get(const char *name);
extern int _nvram_set(const char *name, const char *value);
extern int _nvram_unset(const char *name);
extern int _nvram_getall(char *buf, int count);
extern int _nvram_commit(struct nvram_header *header);
extern int _nvram_init(void *sbh);
extern void _nvram_exit(void);

/* Globals */
static spinlock_t nvram_lock = SPIN_LOCK_UNLOCKED;
static struct semaphore nvram_sem;
static unsigned long nvram_offset = 0;
static int nvram_major = -1;

#ifdef ASUS_NVRAM
static struct proc_dir_entry *g_pdentry = NULL;
#if LINUX_VERSION_CODE < KERNEL_VERSION(2,6,0)
//static devfs_handle_t nvram_handle = NULL;
#endif
#else	// !ASUS_NVRAM
//static devfs_handle_t nvram_handle = NULL;
#endif	// ASUS_NVRAM

static struct mtd_info *nvram_mtd = NULL;


#ifdef ASUS_NVRAM
// from src/shared/bcmutils.c
/*******************************************************************************
 * crc8
 *
 * Computes a crc8 over the input data using the polynomial:
 *
 *       x^8 + x^7 +x^6 + x^4 + x^2 + 1
 *
 * The caller provides the initial value (either CRC8_INIT_VALUE
 * or the previous returned value) to allow for processing of
 * discontiguous blocks of data.  When generating the CRC the
 * caller is responsible for complementing the final return value
 * and inserting it into the byte stream.  When checking, a final
 * return value of CRC8_GOOD_VALUE indicates a valid CRC.
 *
 * Reference: Dallas Semiconductor Application Note 27
 *   Williams, Ross N., "A Painless Guide to CRC Error Detection Algorithms",
 *     ver 3, Aug 1993, ross@guest.adelaide.edu.au, Rocksoft Pty Ltd.,
 *     ftp://ftp.rocksoft.com/clients/rocksoft/papers/crc_v3.txt
 *
 * ****************************************************************************
 */

static uint8 crc8_table[256] = {
    0x00, 0xF7, 0xB9, 0x4E, 0x25, 0xD2, 0x9C, 0x6B,
    0x4A, 0xBD, 0xF3, 0x04, 0x6F, 0x98, 0xD6, 0x21,
    0x94, 0x63, 0x2D, 0xDA, 0xB1, 0x46, 0x08, 0xFF,
    0xDE, 0x29, 0x67, 0x90, 0xFB, 0x0C, 0x42, 0xB5,
    0x7F, 0x88, 0xC6, 0x31, 0x5A, 0xAD, 0xE3, 0x14,
    0x35, 0xC2, 0x8C, 0x7B, 0x10, 0xE7, 0xA9, 0x5E,
    0xEB, 0x1C, 0x52, 0xA5, 0xCE, 0x39, 0x77, 0x80,
    0xA1, 0x56, 0x18, 0xEF, 0x84, 0x73, 0x3D, 0xCA,
    0xFE, 0x09, 0x47, 0xB0, 0xDB, 0x2C, 0x62, 0x95,
    0xB4, 0x43, 0x0D, 0xFA, 0x91, 0x66, 0x28, 0xDF,
    0x6A, 0x9D, 0xD3, 0x24, 0x4F, 0xB8, 0xF6, 0x01,
    0x20, 0xD7, 0x99, 0x6E, 0x05, 0xF2, 0xBC, 0x4B,
    0x81, 0x76, 0x38, 0xCF, 0xA4, 0x53, 0x1D, 0xEA,
    0xCB, 0x3C, 0x72, 0x85, 0xEE, 0x19, 0x57, 0xA0,
    0x15, 0xE2, 0xAC, 0x5B, 0x30, 0xC7, 0x89, 0x7E,
    0x5F, 0xA8, 0xE6, 0x11, 0x7A, 0x8D, 0xC3, 0x34,
    0xAB, 0x5C, 0x12, 0xE5, 0x8E, 0x79, 0x37, 0xC0,
    0xE1, 0x16, 0x58, 0xAF, 0xC4, 0x33, 0x7D, 0x8A,
    0x3F, 0xC8, 0x86, 0x71, 0x1A, 0xED, 0xA3, 0x54,
    0x75, 0x82, 0xCC, 0x3B, 0x50, 0xA7, 0xE9, 0x1E,
    0xD4, 0x23, 0x6D, 0x9A, 0xF1, 0x06, 0x48, 0xBF,
    0x9E, 0x69, 0x27, 0xD0, 0xBB, 0x4C, 0x02, 0xF5,
    0x40, 0xB7, 0xF9, 0x0E, 0x65, 0x92, 0xDC, 0x2B,
    0x0A, 0xFD, 0xB3, 0x44, 0x2F, 0xD8, 0x96, 0x61,
    0x55, 0xA2, 0xEC, 0x1B, 0x70, 0x87, 0xC9, 0x3E,
    0x1F, 0xE8, 0xA6, 0x51, 0x3A, 0xCD, 0x83, 0x74,
    0xC1, 0x36, 0x78, 0x8F, 0xE4, 0x13, 0x5D, 0xAA,
    0x8B, 0x7C, 0x32, 0xC5, 0xAE, 0x59, 0x17, 0xE0,
    0x2A, 0xDD, 0x93, 0x64, 0x0F, 0xF8, 0xB6, 0x41,
    0x60, 0x97, 0xD9, 0x2E, 0x45, 0xB2, 0xFC, 0x0B,
    0xBE, 0x49, 0x07, 0xF0, 0x9B, 0x6C, 0x22, 0xD5,
    0xF4, 0x03, 0x4D, 0xBA, 0xD1, 0x26, 0x68, 0x9F
};

uint8
hndcrc8(
	uint8 *pdata,	/* pointer to array of data to process */
	uint  nbytes,	/* number of input data bytes to process */
	uint8 crc	/* either CRC8_INIT_VALUE or previous return value */
)
{
	/* hard code the crc loop instead of using CRC_INNER_LOOP macro
	 * to avoid the undefined and unnecessary (uint8 >> 8) operation.
	 */
	while (nbytes-- > 0)
		crc = crc8_table[(crc ^ *pdata++) & 0xff];

	return crc;
}

#define NVRAM_DRIVER_VERSION	"0.01"

static int 
nvram_proc_version_read(char *buf, char **start, off_t offset, int count, int *eof, void *data)
{
    int len = 0;

	len += snprintf (buf+len, count-len, "nvram driver : v" NVRAM_DRIVER_VERSION "\n");
	len += snprintf (buf+len, count-len, "nvram space  : 0x%x\n", NVRAM_SPACE);
	len += snprintf (buf+len, count-len, "major number : %d\n", nvram_major);
	len += snprintf (buf+len, count-len, "MTD            \n");
	len += snprintf (buf+len, count-len, "  name       : %s\n", nvram_mtd->name);
	len += snprintf (buf+len, count-len, "  index      : %d\n", nvram_mtd->index);
   	len += snprintf (buf+len, count-len, "  flags      : 0x%x\n", nvram_mtd->flags);
   	len += snprintf (buf+len, count-len, "  size       : 0x%x\n", nvram_mtd->size);
   	len += snprintf (buf+len, count-len, "  erasesize  : 0x%x\n", nvram_mtd->erasesize);

    *eof = 1;
    return len;
}

#endif	// ASUS_NVRAM


int
_nvram_read(char *buf)
{
	struct nvram_header *header = (struct nvram_header *) buf;
	size_t len;

	if (!nvram_mtd ||
	    MTD_READ(nvram_mtd, nvram_mtd->size - NVRAM_SPACE, NVRAM_SPACE, &len, buf) ||
	    len != NVRAM_SPACE ||
	    header->magic != NVRAM_MAGIC) {
		/* Maybe we can recover some data from early initialization */
		memcpy(buf, nvram_buf, NVRAM_SPACE);
	}

	return 0;
}

struct nvram_tuple *
_nvram_realloc(struct nvram_tuple *t, const char *name, const char *value)
{
	if ((nvram_offset + strlen(value) + 1) > NVRAM_SPACE)	{
		return NULL;
	}

	if (!t) {
		if (!(t = kmalloc(sizeof(struct nvram_tuple) + strlen(name) + 1, GFP_ATOMIC)))	{
			return NULL;
		}

		/* Copy name */
		t->name = (char *) &t[1];
		strcpy(t->name, name);

		t->value = NULL;
	}

	/* Copy value */
	if (!t->value || strcmp(t->value, value)) {
		t->value = &nvram_buf[nvram_offset];
		strcpy(t->value, value);
		nvram_offset += strlen(value) + 1;
	}

	return t;
}

void
_nvram_free(struct nvram_tuple *t)
{
	if (!t)
		nvram_offset = 0;
	else
		kfree(t);
}

int
nvram_set(const char *name, const char *value)
{
	unsigned long flags;
	int ret;
	struct nvram_header *header;

	spin_lock_irqsave(&nvram_lock, flags);

#ifdef CFE_UPDATE //write back to default sector as well, Chen-I
        if(strncmp(name, CFE_NVRAM_PREFIX, strlen(CFE_NVRAM_PREFIX))==0)
        {
                if(strcmp(name, CFE_NVRAM_COMMIT)==0)
                        cfe_commit();
                else if(strcmp(name, CFE_NVRAM_WATCHDOG)==0)
                {
                        bcm947xx_watchdog_disable();
                }
                else
                {
                        cfe_update(name+strlen(CFE_NVRAM_PREFIX), value);
                        _nvram_set(name+strlen(CFE_NVRAM_PREFIX), value);
                }
        }
        else
#endif


	if ((ret = _nvram_set(name, value))) {
		/* Consolidate space and try again */
		if ((header = kmalloc(NVRAM_SPACE, GFP_ATOMIC))) {
			if (_nvram_commit(header) == 0)		{
				ret = _nvram_set(name, value);
			}
			kfree(header);
		}
	}
	spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

char *
real_nvram_get(const char *name)
{
	unsigned long flags;
	char *value;

	spin_lock_irqsave(&nvram_lock, flags);
	value = _nvram_get(name);
	spin_unlock_irqrestore(&nvram_lock, flags);

	return value;
}

char *
nvram_get(const char *name)
{	
	if (nvram_major >= 0)
		return real_nvram_get(name);
	else
#ifdef ASUS_NVRAM
		return NULL;
#else	// !ASUS_NVRAM
		return early_nvram_get(name);
#endif	// ASUS_NVRAM
}

int
nvram_unset(const char *name)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_unset(name);
	spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

static void
erase_callback(struct erase_info *done)
{
	wait_queue_head_t *wait_q = (wait_queue_head_t *) done->priv;
	wake_up(wait_q);
}

int
nvram_commit(void)
{
	char *buf;
	size_t erasesize, len;
	unsigned int i;
	int ret;
	struct nvram_header *header;
	unsigned long flags;
	u_int32_t offset;
	DECLARE_WAITQUEUE(wait, current);
	wait_queue_head_t wait_q;
	struct erase_info erase;

	if (!nvram_mtd) {
		printk("nvram_commit: NVRAM not found\n");
		return -ENODEV;
	}

	if (in_interrupt()) {
		printk("nvram_commit: not committing in interrupt\n");
		return -EINVAL;
	}

	/* Backup sector blocks to be erased */
	erasesize = ROUNDUP(NVRAM_SPACE, nvram_mtd->erasesize);
	if (!(buf = kmalloc(erasesize, GFP_KERNEL))) {
		printk("nvram_commit: out of memory\n");
		return -ENOMEM;
	}

	down(&nvram_sem);

	if ((i = erasesize - NVRAM_SPACE) > 0) {
		offset = nvram_mtd->size - erasesize;
		len = 0;
		ret = MTD_READ(nvram_mtd, offset, i, &len, buf);
		if (ret || len != i) {
			printk("nvram_commit: read error ret = %d, len = %d/%d\n", ret, len, i);
			ret = -EIO;
			goto done;
		}
		header = (struct nvram_header *)(buf + i);
	} else {
		offset = nvram_mtd->size - NVRAM_SPACE;
		header = (struct nvram_header *)buf;
	}

	/* Regenerate NVRAM */
	spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_commit(header);
	spin_unlock_irqrestore(&nvram_lock, flags);
	if (ret)
		goto done;

	/* Erase sector blocks */
	init_waitqueue_head(&wait_q);
	for (; offset < nvram_mtd->size - NVRAM_SPACE + header->len; offset += nvram_mtd->erasesize) {
		erase.mtd = nvram_mtd;
		erase.addr = offset;
		erase.len = nvram_mtd->erasesize;
		erase.callback = erase_callback;
		erase.priv = (u_long) &wait_q;

		set_current_state(TASK_INTERRUPTIBLE);
		add_wait_queue(&wait_q, &wait);

		/* Unlock sector blocks */
		if (nvram_mtd->unlock)
			nvram_mtd->unlock(nvram_mtd, offset, nvram_mtd->erasesize);

		if ((ret = MTD_ERASE(nvram_mtd, &erase))) {
			set_current_state(TASK_RUNNING);
			remove_wait_queue(&wait_q, &wait);
			printk("nvram_commit: erase error\n");
			goto done;
		}

		/* Wait for erase to finish */
		schedule();
		remove_wait_queue(&wait_q, &wait);
	}

	/* Write partition up to end of data area */
	offset = nvram_mtd->size - erasesize;
	i = erasesize - NVRAM_SPACE + header->len;
	ret = MTD_WRITE(nvram_mtd, offset, i, &len, buf);
	if (ret || len != i) {
		printk("nvram_commit: write error\n");
		ret = -EIO;
		goto done;
	}

	offset = nvram_mtd->size - erasesize;
	ret = MTD_READ(nvram_mtd, offset, 4, &len, buf);

 done:
	up(&nvram_sem);
	kfree(buf);
	return ret;
}

int
nvram_getall(char *buf, int count)
{
	unsigned long flags;
	int ret;

	spin_lock_irqsave(&nvram_lock, flags);
	ret = _nvram_getall(buf, count);
	spin_unlock_irqrestore(&nvram_lock, flags);

	return ret;
}

EXPORT_SYMBOL(nvram_get);
EXPORT_SYMBOL(nvram_getall);
EXPORT_SYMBOL(nvram_set);
EXPORT_SYMBOL(nvram_unset);
EXPORT_SYMBOL(nvram_commit);

/* User mode interface below */

static ssize_t
dev_nvram_read(struct file *file, char *buf, size_t count, loff_t *ppos)
{
	char tmp[100], *name = tmp, *value;
	ssize_t ret;
	unsigned long off;
	
	if (count > sizeof(tmp)) {
		if (!(name = kmalloc(count, GFP_KERNEL)))
			return -ENOMEM;
	}

	if (copy_from_user(name, buf, count)) {
		ret = -EFAULT;
		goto done;
	}

	if (*name == '\0') {
		/* Get all variables */
		ret = nvram_getall(name, count);
		if (ret == 0) {
			if (copy_to_user(buf, name, count)) {
				ret = -EFAULT;
				goto done;
			}
			ret = count;
		}
	} else {
		if (!(value = nvram_get(name))) {
			ret = 0;
			goto done;
		}

		/* Provide the offset into mmap() space */
		off = (unsigned long) value - (unsigned long) nvram_buf;

		if (put_user(off, (unsigned long *) buf)) {
			ret = -EFAULT;
			goto done;
		}

		ret = sizeof(unsigned long);
	}

	flush_cache_all();	
 
done:
	if (name != tmp)
		kfree(name);

	return ret;
}

static ssize_t
dev_nvram_write(struct file *file, const char *buf, size_t count, loff_t *ppos)
{
	char tmp[100], *name = tmp, *value;
	ssize_t ret;

	if (count > sizeof(tmp)) {
		if (!(name = kmalloc(count, GFP_KERNEL)))
			return -ENOMEM;
	}

	if (copy_from_user(name, buf, count)) {
		ret = -EFAULT;
		goto done;
	}

	value = name;
	name = strsep(&value, "=");

	if (value)
		ret = nvram_set(name, value) ? : count;
	else
		ret = nvram_unset(name) ? : count;

 done:
	if (name != tmp)
		kfree(name);

	return ret;
}	

char *
nvram_xfr(const char *buf)
{
	char *name = tmpbuf;
	ssize_t ret=0;

	//printk("nvram xfr 1: %s\n", buf);	// tmp test
	if (copy_from_user(name, buf, strlen(buf)+1)) {
		ret = -EFAULT;
		goto done;
	}

	if (strncmp(tmpbuf, NLS_NVRAM_U2C, strlen(NLS_NVRAM_U2C))==0)
	{
		asusnls_u2c(tmpbuf);
	}
	else if (strncmp(buf, NLS_NVRAM_C2U, strlen(NLS_NVRAM_C2U))==0)
	{
		asusnls_c2u(tmpbuf);
	}
	else
	{
		strcpy(tmpbuf, "");
		//printk("nvram xfr 2: %s\n", tmpbuf);	// tmp test
	}
	
	if (copy_to_user(buf, tmpbuf, strlen(tmpbuf)+1)) 
	{
		ret = -EFAULT;
		goto done;
	}
	//printk("nvram xfr 3: %s\n", tmpbuf);	// tmp test

done:
	if(ret==0) return tmpbuf;
	else return NULL;
}

static int
dev_nvram_ioctl(struct inode *inode, struct file *file, unsigned int cmd, unsigned long arg)
{
	if (cmd != NVRAM_MAGIC)
		return -EINVAL;
	if(arg==0)
		return nvram_commit();
#ifdef NLS_XFR
	else {
		if(nvram_xfr((char *)arg)!=NULL) return 0;
		else return -EFAULT;
	}
#endif
}


#if defined (ASUS_NVRAM) && !defined (CONFIG_MMU)
static unsigned long 
dev_nvram_get_unmapped_area (struct file *file, unsigned long addr, unsigned long len, unsigned long pgoff, unsigned long flags)
{
	/* If driver need shared mmap, it have to provide an get_unmapped_area method on MMU-less system.
	 * get_unmapped_area method is responsible to validation addr, len, pgoff, and flags
	 * if these parameters are invalid, return -ENOSYS.
	 * if these parameters are valid, return an address that would be apply to vma->vm_start.
	 * You may have a look at Documentation/nommu-mmap.txt
	 */
	if (len > NVRAM_SPACE || (addr + (pgoff << PAGE_SHIFT) + len) > NVRAM_SPACE)	{
		return -ENOSYS;
	}

	/* The vma->vm_start is overwrite by remap_pfn_range() on MMU-less system.
	 * So, we don't have to take care it.
	 */
	return 0;
}
#endif	// ASUS_NVRAM && !CONFIG_MMU


static int
dev_nvram_mmap(struct file *file, struct vm_area_struct *vma)
{
	unsigned long offset = virt_to_phys(nvram_buf);

#ifdef ASUS_NVRAM
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,8)

#ifdef CONFIG_MMU
//error Eric didn't test this situations.
	int ret;
	if ((ret = remap_pfn_range(vma, vma->vm_start, offset >> PAGE_SHIFT, vma->vm_end-vma->vm_start, vma->vm_page_prot)))
#else	// !CONFIG_MMU
	vma->vm_pgoff = ((vma->vm_pgoff << PAGE_SHIFT) + offset) >> PAGE_SHIFT;
	vma->vm_end = (vma->vm_pgoff << PAGE_SHIFT) + (vma->vm_end - vma->vm_start);
	vma->vm_start = vma->vm_pgoff << PAGE_SHIFT;
	if (0)
#endif	// CONFIG_MMU

#else	// !(LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,8))
	int ret;

	if ((ret = remap_page_range(vma->vm_start, offset, vma->vm_end-vma->vm_start, vma->vm_page_prot)))
#endif	// LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,8)
		
#else	// !ASUS_NVRAM
	int ret;

	if (((ret = remap_page_range(vma->vm_start, offset, vma->vm_end-vma->vm_start, vma->vm_page_prot))))
#endif	// ASUS_NVRAM
	{
		return -EAGAIN;
	}

	return 0;
}

static int
dev_nvram_open(struct inode *inode, struct file * file)
{
	MOD_INC_USE_COUNT;	// 1018 disable
	return 0;
}

static int
dev_nvram_release(struct inode *inode, struct file * file)
{
	MOD_DEC_USE_COUNT;	// 1018 disable
	return 0;
}
/*
static int
dev_nvram_owner(struct inode *inode, struct file * file)
{
        return 0;
}
*/
static struct file_operations dev_nvram_fops = {
	owner:		THIS_MODULE,
	//owner:		dev_nvram_owner,	// 1018 add
	open:		dev_nvram_open,
	release:	dev_nvram_release,
	read:		dev_nvram_read,
	write:		dev_nvram_write,
	ioctl:		dev_nvram_ioctl,
	mmap:		dev_nvram_mmap,
#if defined (ASUS_NVRAM) && !defined (CONFIG_MMU)
	get_unmapped_area:	dev_nvram_get_unmapped_area,
#endif	// ASUS_NVRAM && !CONFIG_MMU
};

static void
dev_nvram_exit(void)
{
	int order = 0;
	struct page *page, *end;

#ifdef ASUS_NVRAM
	if (nvram_major >= 0)
		unregister_chrdev(nvram_major, MTD_NVRAM_NAME);
#else	// !ASUS_NVRAM
	if (nvram_handle)
		devfs_unregister(nvram_handle);

	if (nvram_major >= 0)
		devfs_unregister_chrdev(nvram_major, MTD_NVRAM_NAME);
#endif	// ASUS_NVRAM

	if (nvram_mtd)
		put_mtd_device(nvram_mtd);

	while ((PAGE_SIZE << order) < NVRAM_SPACE)
		order++;
	end = virt_to_page(nvram_buf + (PAGE_SIZE << order) - 1);
	for (page = virt_to_page(nvram_buf); page <= end; page++)
		mem_map_unreserve(page);

#ifdef ASUS_NVRAM
	if (g_pdentry != NULL)	{
		remove_proc_entry(MTD_NVRAM_NAME, NULL);
	}
 
	kfree (nvram_buf);
#endif	// ASUS_NVRAM

	_nvram_exit();
}

static int __init
dev_nvram_init(void)
{
	int order = 0, ret = 0;
	struct page *page, *end;
	unsigned int i;

#ifdef ASUS_NVRAM
	nvram_buf = kmalloc (NVRAM_SPACE, GFP_ATOMIC);
	if (nvram_buf == NULL)
		return -ENOMEM;
#endif	// ASUS_NVRAM

	/* Allocate and reserve memory to mmap() */
	while ((PAGE_SIZE << order) < NVRAM_SPACE)
		order++;
	end = virt_to_page(nvram_buf + (PAGE_SIZE << order) - 1);
	for (page = virt_to_page(nvram_buf); page <= end; page++)	{
		mem_map_reserve(page);
	}

#ifdef CONFIG_MTD
	/* Find associated MTD device */
	for (i = 0; i < MAX_MTD_DEVICES; i++) {
		nvram_mtd = get_mtd_device(NULL, i);
		if (nvram_mtd) {
			if (!strcmp(nvram_mtd->name, MTD_NVRAM_NAME) &&
			    nvram_mtd->size >= NVRAM_SPACE)
			{
				break;
			}
			put_mtd_device(nvram_mtd);
		}
	}
	if (i >= MAX_MTD_DEVICES)
		nvram_mtd = NULL;
#endif

	/* Initialize hash table lock */
	spin_lock_init(&nvram_lock);

	/* Initialize commit semaphore */
	init_MUTEX(&nvram_sem);

	/* Register char device */
#ifdef ASUS_NVRAM
	if ((nvram_major = register_chrdev(NVRAM_MAJOR, MTD_NVRAM_NAME, &dev_nvram_fops)) < 0) {
		ret = nvram_major;
		goto err;
	}
#else	// !ASUS_NVRAM
	if ((nvram_major = devfs_register_chrdev(NVRAM_MAJOR, MTD_NVRAM_NAME, &dev_nvram_fops)) < 0) {
		ret = nvram_major;
		goto err;
	}
#endif	// ASUS_NVRAM

	/* Initialize hash table */
	_nvram_init(sbh);

	/* Create /dev/nvram handle */
#ifdef ASUS_NVRAM
#else	// !ASUS_NVRAM
	nvram_handle = devfs_register(NULL, MTD_NVRAM_NAME, DEVFS_FL_NONE, nvram_major, 0,
				      S_IFCHR | S_IRUSR | S_IWUSR | S_IRGRP, &dev_nvram_fops, NULL);
#endif	// ASUS_NVRAM

#ifdef ASUS_NVRAM
	// We don't need sdram_XXX
#else	// !ASUS_NVRAM
	/* Set the SDRAM NCDL value into NVRAM if not already done */
	if (getintvar(NULL, "sdram_ncdl") == 0) {
		unsigned int ncdl;
		char buf[] = "0x00000000";

		if ((ncdl = sb_memc_get_ncdl(sbh))) {
			sprintf(buf, "0x%08x", ncdl);
			nvram_set("sdram_ncdl", buf);
			nvram_commit();
		}
	}
#endif	// ASUS_NVRAM

#ifdef ASUS_NVRAM
	g_pdentry = create_proc_read_entry(MTD_NVRAM_NAME, 0444, NULL, nvram_proc_version_read, NULL);
	if (g_pdentry == NULL) {
		ret  = -ENOMEM;
		goto err;
	}

	g_pdentry->owner = THIS_MODULE;
#endif	// ASUS_NVRAM

//	extern int (*nvram_set_p)(char *, char *);
//	nvram_set_p = nvram_set;

	return 0;

err:
	dev_nvram_exit();
	return ret;
}
#ifdef CFE_UPDATE
void cfe_init(void)
{
        size_t erasesize, len;
        int i;

        /* Find associated MTD device */
        for (i = 0; i < MAX_MTD_DEVICES; i++) {
                cfe_mtd = get_mtd_device(NULL, i);
                if (cfe_mtd) {
                        printk("CFE MTD: %x %s %x\n", i, cfe_mtd->name, cfe_mtd->size);
                        if (!strcmp(cfe_mtd->name, "boot"))
                                break;
                        put_mtd_device(cfe_mtd);
                }
        }
        if (i >= MAX_MTD_DEVICES)
        {
                printk("No CFE MTD\n");
                cfe_mtd = NULL;
        }

        if(!cfe_mtd) goto fail;

        /* sector blocks to be erased and backup */
        erasesize = ROUNDUP(CFE_NVRAM_SPACE, cfe_mtd->erasesize);

        //printk("block size %d\n", erasesize);

        cfe_buf = kmalloc(erasesize, GFP_KERNEL);

        if(!cfe_buf)
        {
                //printk("No CFE Memory\n");
                goto fail;
        }
        MTD_READ(cfe_mtd, CFE_NVRAM_START, erasesize, &len, cfe_buf);

        // find nvram header
        for(i=0;i<len;i+=4)
        {
                cfe_nvram_header=(struct nvram_header *)&cfe_buf[i];
                if (cfe_nvram_header->magic==NVRAM_MAGIC) break;
        }

        bcm947xx_watchdog_disable(); //disable watchdog as well

        //printf("read from nvram %d %s\n", i, cfe_buf);
        //for(i=0;i<CFE_SPACE;i++)
        //{
        //      if(i%16) printk("\n");
        //      printk("%02x ", (unsigned char)cfe_buf[i]);
        //}
        return;
fail:
        if (cfe_mtd)
        {
                put_mtd_device(cfe_mtd);
                cfe_mtd=NULL;
        }
        if(cfe_buf)
        {
                kfree(cfe_buf);
                cfe_buf=NULL;
        }
        return;
}

void cfe_update(char *keyword, char *value)
{
        unsigned long i, offset;
        struct nvram_header tmp, *header;
        uint8 crc;
        int ret;
        int found = 0;

        if(!cfe_buf||!cfe_mtd)
                cfe_init();

        if (!cfe_buf||!cfe_mtd) return;

        header = cfe_nvram_header;

        //printk("before: %x %x\n", header->len,  cfe_nvram_header->crc_ver_init&0xff);

        for(i=CFE_NVRAM_START;i<=CFE_NVRAM_END;i++)
        {
                if(strncmp(&cfe_buf[i], keyword, strlen(keyword))==0)
                {
                        //printk("before: %s\n", cfe_buf+i);
                        offset=strlen(keyword);
                        memcpy(cfe_buf+i+offset+1, value, strlen(value));
                        //printk("after: %s\n", cfe_buf+i);
                        found = 1;
                }
        }

        if(!found)
        {
                char *tmp_buf = (char *)cfe_nvram_header;

                //printk("header len: %x\n", header->len);
                sprintf(tmp_buf+header->len, "%s=%s", keyword, value);
                header->len = header->len + strlen(keyword) + strlen(value) + 2;
                //printk("header len: %x\n", header->len);
        }

        tmp.crc_ver_init = htol32(header->crc_ver_init);
        tmp.config_refresh = htol32(header->config_refresh);
        tmp.config_ncdl = htol32(header->config_ncdl);
        crc = hndcrc8((char *) &tmp + 9, sizeof(struct nvram_header) - 9, CRC8_INIT_VALUE);

        /* Continue CRC8 over data bytes */
        crc = hndcrc8((char *) &header[1], header->len - sizeof(struct nvram_header), crc);
        header->crc_ver_init = (header->crc_ver_init&0xFFFFFF00)|crc;
        //printk("after: %x %x\n", header->crc_ver_init&0xFF, crc);
}

int cfe_commit(void)
{
        DECLARE_WAITQUEUE(wait, current);
        wait_queue_head_t wait_q;
        struct erase_info erase;
        unsigned int i;
        int ret;
        size_t erasesize, len;
        u_int32_t offset;
        char *buf;

        if(!cfe_buf||!cfe_mtd) cfe_init();

        if(!cfe_mtd||!cfe_buf)
        {
                ret = - ENOMEM;
                goto done;
        }

        /* Backup sector blocks to be erased */
        erasesize = ROUNDUP(CFE_NVRAM_SPACE, cfe_mtd->erasesize);

        /* Erase sector blocks */
        init_waitqueue_head(&wait_q);
        for (offset=CFE_NVRAM_START;offset <= CFE_NVRAM_END;offset += cfe_mtd->erasesize) {
           erase.mtd = cfe_mtd;
           erase.addr = offset;
           erase.len = cfe_mtd->erasesize;
           erase.callback = erase_callback;
           erase.priv = (u_long) &wait_q;

           set_current_state(TASK_INTERRUPTIBLE);
           add_wait_queue(&wait_q, &wait);
           /* Unlock sector blocks */
           if (cfe_mtd->unlock)
                   cfe_mtd->unlock(cfe_mtd, offset, cfe_mtd->erasesize);

           if ((ret = MTD_ERASE(cfe_mtd, &erase))) {
                set_current_state(TASK_RUNNING);
                remove_wait_queue(&wait_q, &wait);
                printk("cfe_commit: erase error\n");
                goto done;
           }

           /* Wait for erase to finish */
           schedule();
           remove_wait_queue(&wait_q, &wait);
        }

        ret = MTD_WRITE(cfe_mtd, CFE_NVRAM_START, erasesize, &len, cfe_buf);
        //printk("Write offset: %x %x %x\n", ret, len, erasesize);

        if (ret || len != erasesize) {
           printk("cfe_commit: write error\n");
           ret = -EIO;
        }

done:
        if (cfe_mtd)
        {
                put_mtd_device(cfe_mtd);
                cfe_mtd=NULL;
        }
        if(cfe_buf)
        {
                kfree(cfe_buf);
                cfe_buf=NULL;
        }
        //printk("commit: %d\n", ret);
        return ret;

}
#endif



module_init(dev_nvram_init);
module_exit(dev_nvram_exit);
#ifdef ASUS_NVRAM
MODULE_LICENSE("GPL");
#endif	// ASUS_NVRAM

MODULE_VERSION("V0.01");





