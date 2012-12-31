#if source directory exists, then build from source, otherwise use pre-builts
REBUILD_MODULE=$(shell if [ -f "$(src)/ra_nat.c" ]; then echo 1; else echo 0; fi)

obj-$(CONFIG_RA_HW_NAT) += hw_nat.o

ifeq ($(REBUILD_MODULE),0)
hw_nat-objs += prebuilt/hw_nat.o
else
hw_nat-objs := ra_nat.o foe_fdb.o util.o hwnat_ioctl.o

ifeq ($(CONFIG_RALINK_RT3052),y)
hw_nat-objs += sys_rfrw.o
endif

ifneq ($(CONFIG_HNAT_V2),y)
hw_nat-objs += acl_policy.o acl_ioctl.o
hw_nat-objs += ac_policy.o ac_ioctl.o
hw_nat-objs += mtr_policy.o mtr_ioctl.o
endif

endif
