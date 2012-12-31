cmd_drivers/char/rtl8367b_asicdrv_acl.o := /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-gcc -Wp,-MD,drivers/char/.rtl8367b_asicdrv_acl.o.d  -nostdinc -isystem /root/asuswrt-bender/tools/brcm/K26/hndtools-mipsel-uclibc-4.2.4/bin/../lib/gcc/mipsel-linux-uclibc/4.2.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -Wall -Wundef -Wstrict-prototypes -Wno-trigraphs -fno-strict-aliasing -fno-common -O2  -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips/rt2880 -Iinclude/asm-mips/mach-generic -fomit-frame-pointer -gdwarf-2  -fno-stack-protector -membedded-data -muninit-const-in-rodata -funit-at-a-time -Wdeclaration-after-statement -Wno-pointer-sign -Idrivers/char/RTL8367R -D_LITTLE_ENDIAN   -D"KBUILD_STR(s)=\#s" -D"KBUILD_BASENAME=KBUILD_STR(rtl8367b_asicdrv_acl)"  -D"KBUILD_MODNAME=KBUILD_STR(rtl8367b_asicdrv_acl)" -c -o drivers/char/rtl8367b_asicdrv_acl.o drivers/char/rtl8367b_asicdrv_acl.c

deps_drivers/char/rtl8367b_asicdrv_acl.o := \
  drivers/char/rtl8367b_asicdrv_acl.c \
    $(wildcard include/config/rtl8367b/asicdrv/test.h) \
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

drivers/char/rtl8367b_asicdrv_acl.o: $(deps_drivers/char/rtl8367b_asicdrv_acl.o)

$(deps_drivers/char/rtl8367b_asicdrv_acl.o):
