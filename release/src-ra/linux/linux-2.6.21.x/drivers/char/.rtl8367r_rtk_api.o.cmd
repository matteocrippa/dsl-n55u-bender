cmd_drivers/char/rtl8367r_rtk_api.o := /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-gcc -Wp,-MD,drivers/char/.rtl8367r_rtk_api.o.d  -nostdinc -isystem /root/asuswrt-bender/tools/brcm/K26/hndtools-mipsel-uclibc-4.2.4/bin/../lib/gcc/mipsel-linux-uclibc/4.2.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O2  -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips/rt2880 -Iinclude/asm-mips/mach-generic -fomit-frame-pointer -gdwarf-2  -fno-stack-protector -membedded-data -muninit-const-in-rodata -funit-at-a-time -Wdeclaration-after-statement -Wno-pointer-sign -Idrivers/char/RTL8367R -D_LITTLE_ENDIAN   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(rtl8367r_rtk_api)"  -D"KBUILD_MODNAME=KBUILD_STR(rtl8367r_rtk_api)" -c -o drivers/char/rtl8367r_rtk_api.o drivers/char/rtl8367r_rtk_api.c

deps_drivers/char/rtl8367r_rtk_api.o := \
  drivers/char/rtl8367r_rtk_api.c \
    $(wildcard include/config/end.h) \
  drivers/char/RTL8367R/rtl8367b_asicdrv_acl.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv.h \
  drivers/char/RTL8367R/rtk_types.h \
  drivers/char/RTL8367R/rtk_error.h \
  drivers/char/RTL8367R/rtl8367b_reg.h \
    $(wildcard include/config/rst/offset.h) \
    $(wildcard include/config/rst/mask.h) \
    $(wildcard include/config/dummy/15/offset.h) \
    $(wildcard include/config/dummy/15/mask.h) \
    $(wildcard include/config/sel/offset.h) \
    $(wildcard include/config/sel/mask.h) \
  drivers/char/RTL8367R/rtl8367b_base.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_dot1x.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_qos.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_scheduling.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_fc.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_port.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_unknownMulticast.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_phy.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_igmp.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_rma.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_vlan.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_lut.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_led.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_svlan.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_meter.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_inbwctrl.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_storm.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_misc.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_portIsolation.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_cputag.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_trunking.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_mirror.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_mib.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_interrupt.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_green.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_eee.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_eav.h \
  drivers/char/RTL8367R/rtl8367b_asicdrv_hsb.h \
  drivers/char/RTL8367R/myrtk.h \
  drivers/char/RTL8367R/rtk_api.h \
    $(wildcard include/config/ledoff.h) \
    $(wildcard include/config/dupcol.h) \
    $(wildcard include/config/link/act.h) \
    $(wildcard include/config/spd1000.h) \
    $(wildcard include/config/spd100.h) \
    $(wildcard include/config/spd10.h) \
    $(wildcard include/config/spd1000act.h) \
    $(wildcard include/config/spd100act.h) \
    $(wildcard include/config/spd10act.h) \
    $(wildcard include/config/spd10010act.h) \
    $(wildcard include/config/loopdetect.h) \
    $(wildcard include/config/eee.h) \
    $(wildcard include/config/linkrx.h) \
    $(wildcard include/config/linktx.h) \
    $(wildcard include/config/master.h) \
    $(wildcard include/config/act.h) \
  drivers/char/RTL8367R/rtk_api_ext.h \

drivers/char/rtl8367r_rtk_api.o: $(deps_drivers/char/rtl8367r_rtk_api.o)

$(deps_drivers/char/rtl8367r_rtk_api.o):
