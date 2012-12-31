cmd_arch/mips/rt2880/mipsIRQ.o := /opt/buildroot-gcc342/bin/mipsel-linux-uclibc-gcc -Wp,-MD,arch/mips/rt2880/.mipsIRQ.o.d  -nostdinc -isystem /root/asuswrt-bender/tools/brcm/K26/hndtools-mipsel-uclibc-4.2.4/bin/../lib/gcc/mipsel-linux-uclibc/4.2.4/include -D__KERNEL__ -Iinclude  -include include/linux/autoconf.h -D__ASSEMBLY__  -mabi=32 -G 0 -mno-abicalls -fno-pic -pipe -msoft-float -ffreestanding  -march=mips32r2 -Wa,-mips32r2 -Wa,--trap -Iinclude/asm-mips/rt2880 -Iinclude/asm-mips/mach-generic    -c -o arch/mips/rt2880/mipsIRQ.o arch/mips/rt2880/mipsIRQ.S

deps_arch/mips/rt2880/mipsIRQ.o := \
  arch/mips/rt2880/mipsIRQ.S \
    $(wildcard include/config/ralink/rt2880.h) \
    $(wildcard include/config/ralink/rt3052.h) \
    $(wildcard include/config/ralink/rt3352.h) \
    $(wildcard include/config/ralink/rt2883.h) \
    $(wildcard include/config/ralink/rt3883.h) \
  include/asm/asm.h \
    $(wildcard include/config/printk.h) \
    $(wildcard include/config/cpu/has/prefetch.h) \
  include/asm/sgidefs.h \
  include/asm/mipsregs.h \
    $(wildcard include/config/cpu/vr41xx.h) \
    $(wildcard include/config/page/size/4kb.h) \
    $(wildcard include/config/page/size/16kb.h) \
    $(wildcard include/config/page/size/64kb.h) \
    $(wildcard include/config/mips/mt/smtc.h) \
  include/linux/linkage.h \
  include/asm/linkage.h \
  include/asm/hazards.h \
    $(wildcard include/config/cpu/mipsr2.h) \
    $(wildcard include/config/cpu/r10000.h) \
    $(wildcard include/config/cpu/rm9000.h) \
    $(wildcard include/config/cpu/sb1.h) \
  include/asm/regdef.h \
  include/asm/stackframe.h \
    $(wildcard include/config/cpu/has/smartmips.h) \
    $(wildcard include/config/32bit.h) \
    $(wildcard include/config/smp.h) \
    $(wildcard include/config/build/elf64.h) \
    $(wildcard include/config/64bit.h) \
    $(wildcard include/config/cpu/r3000.h) \
    $(wildcard include/config/cpu/tx39xx.h) \
  include/linux/threads.h \
    $(wildcard include/config/nr/cpus.h) \
    $(wildcard include/config/base/small.h) \
  include/asm/asmmacro.h \
  include/asm/asmmacro-32.h \
  include/asm/asm-offsets.h \
  include/asm/fpregdef.h \

arch/mips/rt2880/mipsIRQ.o: $(deps_arch/mips/rt2880/mipsIRQ.o)

$(deps_arch/mips/rt2880/mipsIRQ.o):
