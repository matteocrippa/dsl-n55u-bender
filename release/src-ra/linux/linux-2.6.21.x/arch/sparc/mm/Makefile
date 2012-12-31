# $Id: Makefile,v 1.1.1.1 2010/12/02 04:21:35 walf_wu Exp $
# Makefile for the linux Sparc-specific parts of the memory manager.
#

EXTRA_AFLAGS := -ansi

obj-y    := fault.o init.o loadmmu.o generic.o extable.o btfixup.o

ifeq ($(CONFIG_SUN4),y)
obj-y	 += nosrmmu.o
else
obj-y	 += srmmu.o iommu.o io-unit.o hypersparc.o viking.o tsunami.o swift.o
endif

ifdef CONFIG_HIGHMEM
obj-y	+= highmem.o
endif

ifdef CONFIG_SMP
obj-y   += nosun4c.o
else
obj-y   += sun4c.o
endif
