/* $Id: sun3-head.h,v 1.1.1.1 2010/12/02 04:12:40 walf_wu Exp $ */
#ifndef __SUN3_HEAD_H
#define __SUN3_HEAD_H

#define KERNBASE        0xE000000  /* First address the kernel will eventually be */
#define LOAD_ADDR       0x4000      /* prom jumps to us here unless this is elf /boot */
#define FC_CONTROL  3
#define FC_SUPERD    5
#define FC_CPU      7

#endif /* __SUN3_HEAD_H */
