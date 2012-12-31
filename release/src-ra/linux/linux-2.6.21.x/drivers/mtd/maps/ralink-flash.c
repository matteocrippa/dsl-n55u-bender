/* ralink-flash.c
 *
 * Steven Liu <steven_liu@ralinktech.com.tw>:
 *   - initial approach
 *
 * Winfred Lu <winfred_lu@ralinktech.com.tw>:
 *   - 32MB flash support for RT3052
 *   - flash API
 */

#include <linux/module.h>
#include <linux/types.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/err.h>

#include <asm/io.h>
#include <linux/mtd/mtd.h>
#include <linux/mtd/map.h>
#include <linux/mtd/concat.h>
#include <linux/mtd/partitions.h>
#include <asm/addrspace.h>
#include "ralink-flash.h"


#ifndef CONFIG_RT2880_FLASH_32M 
#define WINDOW_ADDR		CPHYSADDR(CONFIG_MTD_PHYSMAP_START)
#define WINDOW_SIZE		CONFIG_MTD_PHYSMAP_LEN
#define NUM_FLASH_BANKS		1
#else
#define WINDOW_ADDR_0		CPHYSADDR(CONFIG_MTD_PHYSMAP_START)
#define WINDOW_ADDR_1		CPHYSADDR(0xBB000000)
#define WINDOW_SIZE		(CONFIG_MTD_PHYSMAP_LEN / 2)
#define NUM_FLASH_BANKS		2
#endif

#define BUSWIDTH		CONFIG_MTD_PHYSMAP_BUSWIDTH


#if !defined (CONFIG_MTD_NAND_RALINK) && !defined (CONFIG_MTD_SPI_RALINK)

static struct mtd_info *ralink_mtd[NUM_FLASH_BANKS];
#ifndef CONFIG_RT2880_FLASH_32M 
static struct map_info ralink_map[] = {
	{
	.name = "Ralink SoC physically mapped flash",
	.bankwidth = BUSWIDTH,
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR
	}
};
#else
static struct mtd_info *merged_mtd;
static struct map_info ralink_map[] = {
	{
	.name = "Ralink SoC physically mapped flash bank 0",
	.bankwidth = BUSWIDTH,
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR_0
	},
	{
	.name = "Ralink SoC physically mapped flash bank 1",
	.bankwidth = BUSWIDTH,
	.size = WINDOW_SIZE,
	.phys = WINDOW_ADDR_1
	}
};
#endif

#if defined (CONFIG_RT2880_FLASH_32M)
static struct mtd_partition rt2880_partitions[] = {
        {
                name:           "Bootloader",  /* mtdblock0 */
                size:           MTD_BOOT_PART_SIZE,  /* 192K */
                offset:         0,
        }, {
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
                name:           "linux",
                size:           CONFIG_MTD_KERNEL_PART_SIZ,
                offset:         MTDPART_OFS_APPEND,
        }, {
                name:           "rootfs",
                size:           MTD_ROOTFS_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
        }, {
#else //CONFIG_RT2880_ROOTFS_IN_RAM
                name:           "linux",
                size:           MTD_KERN_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
        }, {
#endif
                name:           "nvram",
                size:           0x20000,  /* 128K */
                offset:         MTDPART_OFS_APPEND
        }, {
                name:           "Factory",
                size:           0x20000,  /* 128K */
                offset:         MTDPART_OFS_APPEND
#ifdef CONFIG_DUAL_IMAGE
	}, {
		name:		"Kernel2", /* mtdblock4 */
		size:		MTD_KERN2_PART_SIZE, 
		offset:		MTD_KERN2_PART_OFFSET,
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
	}, {
                name:           "RootFS2",
                size:           MTD_ROOTFS2_PART_SIZE,
                offset:         MTD_ROOTFS2_PART_OFFSET,
#endif
        }
#else
	}
#endif 
};
#else //not 32M flash
static struct mtd_partition rt2880_partitions[] = {
        {
                name:           "Bootloader",  /* mtdblock0 */
                size:           MTD_BOOT_PART_SIZE,  /* 192K */
                offset:         0,
        }, {
                name:           "nvram", /* mtdblock1 */
                size:           MTD_CONFIG_PART_SIZE,  /* 64K */
                offset:         MTD_BOOT_PART_SIZE
        }, {
                name:           "Factory", /* mtdblock2 */
                size:           MTD_FACTORY_PART_SIZE,  /* 64K */
                offset:         MTD_BOOT_PART_SIZE+MTD_CONFIG_PART_SIZE
        }, {
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
                name:           "linux", /* mtdblock3 */
                size:           CONFIG_MTD_KERNEL_PART_SIZ+MTD_ROOTFS_PART_SIZE,
                offset:         MTD_BOOT_PART_SIZE+MTD_CONFIG_PART_SIZE+MTD_FACTORY_PART_SIZE
        }, {
                name:           "rootfs", /* mtdblock4 */
                size:           MTD_ROOTFS_PART_SIZE,
                offset:         MTD_BOOT_PART_SIZE+MTD_CONFIG_PART_SIZE+MTD_FACTORY_PART_SIZE+CONFIG_MTD_KERNEL_PART_SIZ,
#else //CONFIG_RT2880_ROOTFS_IN_RAM
                name:           "linux", /* mtdblock3 */
                size:           MTD_KERN_PART_SIZE,
                offset:         MTDPART_OFS_APPEND,
#endif
#ifdef CONFIG_DUAL_IMAGE
	}, {
		name:		"Kernel2", /* mtdblock4 */
		size:		MTD_KERN2_PART_SIZE, 
		offset:		MTD_KERN2_PART_OFFSET,
#ifdef CONFIG_RT2880_ROOTFS_IN_FLASH
	}, {
                name:           "RootFS2",
                size:           MTD_ROOTFS2_PART_SIZE,
                offset:         MTD_ROOTFS2_PART_OFFSET,
#endif
        }
#else
	}
#endif 
};
#endif

#endif // !defined (CONFIG_MTD_NAND_RALINK) && !defined (CONFIG_MTD_SPI_RALINK)

#if !defined (CONFIG_MTD_NAND_RALINK) && !defined (CONFIG_MTD_SPI_RALINK)
static int ralink_lock(struct mtd_info *mtd, loff_t ofs, size_t len)
{
	return 0;
}

static int ralink_unlock(struct mtd_info *mtd, loff_t ofs, size_t len)
{
	return 0;
}
#endif

#ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
typedef struct __image_header {
	uint8_t unused[60];
	uint32_t ih_ksz;
} _ihdr_t;
#endif

int __init rt2880_mtd_init(void)
{
#if !defined (CONFIG_MTD_NAND_RALINK) && !defined (CONFIG_MTD_SPI_RALINK)
	int ret = -ENXIO;
	int i, found = 0;

	for (i = 0; i < NUM_FLASH_BANKS; i++) {
		printk(KERN_NOTICE "ralink flash device: 0x%x at 0x%x\n",
				ralink_map[i].size, ralink_map[i].phys);

		ralink_map[i].virt = ioremap_nocache(ralink_map[i].phys, ralink_map[i].size);
		if (!ralink_map[i].virt) {
			printk("Failed to ioremap\n");
			return -EIO;
		}
		simple_map_init(&ralink_map[i]);

		ralink_mtd[i] = do_map_probe("cfi_probe", &ralink_map[i]);
		if (ralink_mtd[i]) {
			ralink_mtd[i]->owner = THIS_MODULE;
			ralink_mtd[i]->lock = ralink_lock;
			ralink_mtd[i]->unlock = ralink_unlock;
			++found;
		}
		else
			iounmap(ralink_map[i].virt);
	}

#ifdef CONFIG_ROOTFS_IN_FLASH_NO_PADDING
	if(found==NUM_FLASH_BANKS) {
#ifndef CONFIG_ROOTFS_FLASH_32M
		_ihdr_t hdr;
		unsigned char *ptr;
		ptr = (unsigned char *)(ralink_map[0].virt + MTD_BOOT_PART_SIZE +
			MTD_CONFIG_PART_SIZE + MTD_FACTORY_PART_SIZE);
		
		memcpy(&hdr, ptr, sizeof(_ihdr_t));

		if (((hdr.ih_ksz)&0x000000ff)==0xa9 && ((hdr.ih_ksz)&0xffffff00) != 0) {
		rt2880_partitions[3].size = IMAGE1_SIZE - (MTD_BOOT_PART_SIZE+MTD_CONFIG_PART_SIZE+MTD_FACTORY_PART_SIZE);
		rt2880_partitions[4].size = IMAGE1_SIZE - (MTD_BOOT_PART_SIZE +
				MTD_CONFIG_PART_SIZE + MTD_FACTORY_PART_SIZE +
				ntohl(hdr.ih_ksz&0xffffff00));
		rt2880_partitions[4].offset = rt2880_partitions[3].offset + ntohl(hdr.ih_ksz&0xffffff00);
		printk(KERN_NOTICE "partion 3: %x %x\n", rt2880_partitions[3].offset, rt2880_partitions[3].size);
		printk(KERN_NOTICE "partion 4: %x %x\n", rt2880_partitions[4].offset, rt2880_partitions[4].size);
		}
#endif
	}
#endif




	if (found == NUM_FLASH_BANKS) {
#ifdef CONFIG_RT2880_FLASH_32M
		merged_mtd = mtd_concat_create(ralink_mtd, NUM_FLASH_BANKS,
				"Ralink Merged Flash");
		ret = add_mtd_partitions(merged_mtd, rt2880_partitions,
				ARRAY_SIZE(rt2880_partitions));
#else
		ret = add_mtd_partitions(ralink_mtd[0], rt2880_partitions,
				ARRAY_SIZE(rt2880_partitions));
#endif
		if (ret) {
			for (i = 0; i < NUM_FLASH_BANKS; i++)
				iounmap(ralink_map[i].virt);
			return ret;
		}
	}
	else {
		printk("Error: %d flash device was found\n", found);
		return -ENXIO;
	}
#endif
	return 0;
}

static void __exit rt2880_mtd_cleanup(void)
{
#if !defined (CONFIG_MTD_NAND_RALINK) && !defined (CONFIG_MTD_SPI_RALINK)
	int i;

#ifdef CONFIG_RT2880_FLASH_32M 
	if (merged_mtd) {
		del_mtd_device(merged_mtd);
		mtd_concat_destroy(merged_mtd);
	}
#endif
	for (i = 0; i < NUM_FLASH_BANKS; i++) {
		if (ralink_mtd[i])
			map_destroy(ralink_mtd[i]);
		if (ralink_map[i].virt) {
			iounmap(ralink_map[i].virt);
			ralink_map[i].virt = NULL;
		}
	}
#endif
}

rootfs_initcall(rt2880_mtd_init);
module_exit(rt2880_mtd_cleanup);

MODULE_AUTHOR("Steven Liu <steven_liu@ralinktech.com.tw>");
MODULE_DESCRIPTION("Ralink APSoC Flash Map");
MODULE_LICENSE("GPL");

/*
 * Flash API: ra_mtd_read, ra_mtd_write
 * Arguments:
 *   - num: specific the mtd number
 *   - to/from: the offset to read from or written to
 *   - len: length
 *   - buf: data to be read/written
 * Returns:
 *   - return -errno if failed
 *   - return the number of bytes read/written if successed
 */
#ifdef RA_MTD_RW_BY_NUM
int ra_mtd_write(int num, loff_t to, size_t len, const u_char *buf)
{
	int ret = -1;
	size_t rdlen, wrlen;
	struct mtd_info *mtd;
	struct erase_info ei;
	u_char *bak = NULL;

	mtd = get_mtd_device(NULL, num);
	if (IS_ERR(mtd))
		return (int)mtd;
	if (len > mtd->erasesize) {
		put_mtd_device(mtd);
		return -E2BIG;
	}

	bak = kmalloc(mtd->erasesize, GFP_KERNEL);
	if (bak == NULL) {
		put_mtd_device(mtd);
		return -ENOMEM;
	}

	ret = mtd->read(mtd, 0, mtd->erasesize, &rdlen, bak);
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
	if (rdlen != mtd->erasesize)
		printk("warning: ra_mtd_write: rdlen is not equal to erasesize\n");

	memcpy(bak + to, buf, len);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	ret = mtd->erase(mtd, &ei);
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}

	ret = mtd->write(mtd, 0, mtd->erasesize, &wrlen, bak);

	put_mtd_device(mtd);
	kfree(bak);
	return ret;
}
#endif

int ra_mtd_write_nm(char *name, loff_t to, size_t len, const u_char *buf)
{
	int ret = -1;
	size_t rdlen, wrlen;
	struct mtd_info *mtd;
	struct erase_info ei;
	u_char *bak = NULL;

	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd))
		return (int)mtd;
	if (len > mtd->erasesize) {
		put_mtd_device(mtd);
		return -E2BIG;
	}

	bak = kmalloc(mtd->erasesize, GFP_KERNEL);
	if (bak == NULL) {
		put_mtd_device(mtd);
		return -ENOMEM;
	}

	ret = mtd->read(mtd, 0, mtd->erasesize, &rdlen, bak);
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}
	if (rdlen != mtd->erasesize)
		printk("warning: ra_mtd_write: rdlen is not equal to erasesize\n");

	memcpy(bak + to, buf, len);

	ei.mtd = mtd;
	ei.callback = NULL;
	ei.addr = 0;
	ei.len = mtd->erasesize;
	ei.priv = 0;
	ret = mtd->erase(mtd, &ei);
	if (ret != 0) {
		put_mtd_device(mtd);
		kfree(bak);
		return ret;
	}

	ret = mtd->write(mtd, 0, mtd->erasesize, &wrlen, bak);

	put_mtd_device(mtd);
	kfree(bak);
	return ret;
}

#ifdef RA_MTD_RW_BY_NUM
int ra_mtd_read(int num, loff_t from, size_t len, u_char *buf)
{
	int ret;
	size_t rdlen;
	struct mtd_info *mtd;

	mtd = get_mtd_device(NULL, num);
	if (IS_ERR(mtd))
		return (int)mtd;

	ret = mtd->read(mtd, from, len, &rdlen, buf);
	if (rdlen != len)
		printk("warning: ra_mtd_read: rdlen is not equal to len\n");

	put_mtd_device(mtd);
	return ret;
}
#endif

int ra_mtd_read_nm(char *name, loff_t from, size_t len, u_char *buf)
{
	int ret;
	size_t rdlen;
	struct mtd_info *mtd;

	mtd = get_mtd_device_nm(name);
	if (IS_ERR(mtd))
		return (int)mtd;

	ret = mtd->read(mtd, from, len, &rdlen, buf);
	if (rdlen != len)
		printk("warning: ra_mtd_read_nm: rdlen is not equal to len\n");

	put_mtd_device(mtd);
	return ret;
}

#ifdef RA_MTD_RW_BY_NUM
EXPORT_SYMBOL(ra_mtd_write);
EXPORT_SYMBOL(ra_mtd_read);
#endif
EXPORT_SYMBOL(ra_mtd_write_nm);
EXPORT_SYMBOL(ra_mtd_read_nm);
